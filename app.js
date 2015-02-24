var express = require('express');
var app = express();

app.get('/', function(req, res){
   // res.send('Hello World');
    res.sendFile( 'viewer/viewer.html', { root: __dirname } )
});

app.listen(3000);
