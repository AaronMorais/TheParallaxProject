# TheParallaxProject

# Leogizer
The awesomesauce of the parallax project.
It does it all.

Usage
```
  cd legoizer
  make run
```

# Web Server
The web server serves our client page.
It listens to 'job' web socket messages from the client. It broadcasts 'complete' messages to the client.
It places jobs onto the SQS 'input-queue' and pulls completed jobs from the SQS 'output-queue'

Initial setup
```
  cd web-server
  npm install
```

To start a worker instance
```
  DEBUG=1 node server.js
```

# Workers
Workers pull from the SQS 'input-queue' and place results on a SQS 'output-queue'

Initial setup
```
  cd worker
  virtualenv venv
  source venv/bin/activate
  pip install -r requirements.txt
```

To start a worker instance
```
  cd worker
  source venv/bin/activate
  DEBUG=1 python worker.py
```
Caveat: This assumes that your are running from the 'worker' folder and that there is an executable at '../legoizer/build/legoizer'

# S3
We are using S3 to upload files.
When passing messages from web server to worker or vice-versa, we pass sha-1 hashes in the message body.
These hashes corespond to file names in the 'team-parallax' s3 bucket.

# SQS
We are using SQS to pass messages.
We have an 'input-queue' and an 'output-queue'. It is available in two regions.
'us-west-1' is our dev region. 'us-west-1' is our production region.

