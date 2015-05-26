"""
Scheduled/recurring tasks.
"""
from __future__ import absolute_import, print_function, unicode_literals
import json
import os

import boto.sqs

conn = boto.sqs.connect_to_region(
    "us-east-1",
    aws_access_key_id="AKIAJ3TDGLWZIYCFEZTA",
    aws_secret_access_key="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")

input_queue = conn.get_queue("input-queue")
input_queue.set_message_class(boto.sqs.message.RawMessage)
output_queue = conn.get_queue("output-queue")


def run_job(obj):
    print("Running Job", obj)
    job_file = open("job_data", "w")
    job_file.write(obj)
    job_file.close()
    os.system("~/legolizer/TheParallaxProject/legoizer/build/legoizer \
        ~/legolizer/TheParallaxProject/aaron/worker/job_data")
    result = open("~/legolizer/TheParallaxProject/legoizer/build/result.obj",
                  "r").read()
    return result


def process_jobs():
    jobs = input_queue.get_messages(num_messages=10)

    for job in jobs:
        job_body = job.get_body()
        print("Received job", job_body)
        result = run_job(job_body)

        new_message = boto.sqs.message.Message()
        new_message.set_body(result)
        output_queue.write(new_message)

        print("Sent Job", json.dumps(result))

        input_queue.delete_message(job)
    process_jobs()
