"""
Scheduled/recurring tasks.
"""
from __future__ import absolute_import, print_function, unicode_literals
import json
import logging
import os

from apscheduler.schedulers.blocking import BlockingScheduler
from apscheduler.jobstores.memory import MemoryJobStore
from apscheduler.executors.pool import ThreadPoolExecutor
from boto.s3.key import Key
import boto.sqs
import boto.s3
import hashlib
import pytz

logger = logging.basicConfig()
jobstores = {
    "default": MemoryJobStore()
}
executors = {
    "default": ThreadPoolExecutor(20)
}
job_defaults = {
    "coalesce": True,
    "max_instances": 1,
    "misfire_grace_time": None,  # always execute missed jobs
}
scheduler = BlockingScheduler(jobstores=jobstores, executors=executors,
                              timezone=pytz.utc, logger=logger)

conn = boto.sqs.connect_to_region(
    "us-east-1",
    aws_access_key_id="AKIAJ3TDGLWZIYCFEZTA",
    aws_secret_access_key="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")

input_queue = conn.get_queue("input-queue")
input_queue.set_message_class(boto.sqs.message.RawMessage)
output_queue = conn.get_queue("output-queue")

s3_conn = boto.connect_s3(
    aws_access_key_id="AKIAJ3TDGLWZIYCFEZTA",
    aws_secret_access_key="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")
bucket = s3_conn.create_bucket("team-parallax")
bucket_key = Key(bucket)


def run_job(obj):
    print("Running Job")
    bucket_key.key = obj
    obj = bucket_key.get_contents_as_string()

    job_file = open("job_data", "w")
    job_file.write(obj)
    job_file.close()
    os.system("build/legoizer job_data")
    result = open("build/result.obj", "r").read()
    return result


def process_jobs():
    jobs = input_queue.get_messages(num_messages=1, wait_time_seconds=20)

    for job in jobs:
        job_body = job.get_body()
        print("Received job")
        result = run_job(job_body)

        sha1 = hashlib.sha1()
        sha1.update(result)
        file_name = sha1.hexdigest()

        bucket_key.key = file_name
        bucket_key.set_contents_from_string(result)

        new_message = boto.sqs.message.Message()
        new_message.set_body(
            json.dumps({"before": job_body, "after": file_name})
        )
        output_queue.write(new_message)

        print("Sent Job")

        input_queue.delete_message(job)


scheduler.add_job(process_jobs, "interval", seconds=1)

scheduler.start()
