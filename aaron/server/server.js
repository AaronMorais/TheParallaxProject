var express = require('express')
var app = express();
var server = require('http').Server(app);
var io = require('socket.io')(server);
var AWS = require('aws-sdk');
var crypto = require('crypto')

var s3 = new AWS.S3({apiVersion: '2006-03-01'});

var sqs = new AWS.SQS({
    apiVersion: '2012-11-05',
    accessKeyId: 'AKIAJ3TDGLWZIYCFEZTA',
    secretAccessKey: '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',
    region: "us-east-1"
});

// input_queue_url = ""
// output_queue_url = ""
// sqs.createQueue({ QueueName: 'input-queue' }, function(err, data) {
//     input_queue_url = data["QueueUrl"];
// });
// sqs.createQueue({ QueueName: 'output-queue' }, function(err, data) {
//     output_queue_url = data["QueueUrl"];
// });

input_queue_url = "https://sqs.us-east-1.amazonaws.com/581926697193/input-queue";
output_queue_url = "https://sqs.us-east-1.amazonaws.com/581926697193/output-queue";

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
            io.emit('complete', body);
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
      console.log('uploaded file[' + fileName + ']');
      console.log(arguments);

      var params = {
        MessageBody: fileName,
        QueueUrl: input_queue_url,
        DelaySeconds: 0
      };
      sqs.sendMessage(params, function(err, data) {
          console.log("Job sent:", err, data)
      });
    });
  });
});

server.listen(process.env.PORT || 3000);
