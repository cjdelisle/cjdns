var dgram = require('dgram'),
    bencode = require('bencode'),
    sys = require('util');

var CJDNS = function (port, host, password) {
    this.port = port;
    this.host = host || 'localhost';
    this.password = password;

    this.send({q: 'ping'}, function (err, msg) {
        if (msg && msg.q === 'pong') {
            sys.log('CJDNS Admin backend found and ready to work!');
        } else {
            sys.log(msg);
        }
    });
};

CJDNS.prototype.send = function(data, callback) {
    var msg = new Buffer(bencode.encode(data)),
        socket = dgram.createSocket('udp4');

    socket.on('message', function (msg) {
        var response = bencode.decode(msg, 'utf8');
        callback(null, response);

        socket.close();
    });

    socket.send(msg, 0, msg.length, this.port, this.host, function(err, bytes) {
        if (err) {
            callback(err);
        }
    });
};

module.exports = CJDNS;