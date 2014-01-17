var express = require('express');

var app = express();

app.configure(function() {
    // To handle post data
    app.use(express.bodyParser());
});

app.listen(8000, process.env.IP);

// Function that gets post data from Google upon device registration
app.post('/', function(req, res) {
    if(req.body) {
        res.json({'reply': 'hi'});
    }

});