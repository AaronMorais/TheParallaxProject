var express = require('express')
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var AWS = require('aws-sdk');
var crypto = require('crypto')

var region = process.env.DEBUG ? "us-west-1" : "us-east-1"

input_queue_url = "https://sqs." + region + ".amazonaws.com/581926697193/input-queue";
output_queue_url = "https://sqs." + region + ".amazonaws.com/581926697193/output-queue";

var s3 = new AWS.S3({
    apiVersion: '2006-03-01',
    accessKeyId: 'AKIAJ3TDGLWZIYCFEZTA',
    secretAccessKey: '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',
});

var sqs = new AWS.SQS({
    apiVersion: '2012-11-05',
    accessKeyId: 'AKIAJ3TDGLWZIYCFEZTA',
    secretAccessKey: '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',
    region: region
});

var pending_requests = {}

function checkOutputMessages() {
    var params = {
      QueueUrl: output_queue_url,
      MaxNumberOfMessages: 1,
    };
    sqs.receiveMessage(params, function(err, data) {
        if (data && data.Messages) {
            var message = data.Messages[0];
            body = new Buffer(message.Body, 'base64').toString();
            console.log("Result received", body)
            result = JSON.parse(body)
            s3.getObject({Bucket: "team-parallax", Key: result.after}, function(err, data) {
              var socket = pending_requests[result.before]
              if (socket) {
                socket.emit('complete', data.Body.toString());
                delete pending_requests[result.before]
              } else {
                console.log("Can't emit result. Requester must have disconnected.")
              }
            });

            var deleteParams = {
                QueueUrl: output_queue_url,
                ReceiptHandle: message.ReceiptHandle
            };
            sqs.deleteMessage(deleteParams, function(err, data) {});
        }
        setTimeout(checkOutputMessages, 1000);
   });
}
checkOutputMessages()

app.use(express.static(__dirname + '/public'));

app.get('/', function(req,res){
    res.sendFile(__dirname + '/index.html');
});

io.on('connection', function (socket) {
  socket.on('job', function (data) {
    console.log("Job request received");

    shasum = crypto.createHash('sha1');
    shasum.update(data);
    fileName = shasum.digest('hex')

    s3.putObject({
      ACL: 'public-read',
      Bucket: 'team-parallax',
      Key: fileName,
      Body: data,
      ContentType: 'text/plain'
    }, function(error, response) {
      console.log('Uploaded file: ' + fileName);

      var params = {
        MessageBody: fileName,
        QueueUrl: input_queue_url,
        DelaySeconds: 0
      };
      pending_requests[fileName] = socket;
      sqs.sendMessage(params, function(err, data) {
        if (err) {
          console.log("Job failed to send.")
        } else {
          console.log("Job sent")
        }
      });
    });
  });
});

server.listen(process.env.PORT || 3000);
