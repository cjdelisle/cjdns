var dgram = require('dgram'),
    bencode = require('bencode'),
    sys = require('util'),
    crypto = require('crypto');

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

CJDNS.prototype.sendAuth = function(data, callback) {
    var cjdns = this,
        request = {
            q: 'auth',
            aq: data.q
        };

    if (data.args) {
        request.args = data.args;
    }

    function makeHash (password, cookie, request) {
        var hash = password + '' + cookie,
            sha256 = crypto.createHash('sha256');

        sha256.update(hash);
        hash = sha256.digest('hex');

        request.hash = hash;

        hash = bencode.encode(request);
        sha256 = crypto.createHash('sha256');
        sha256.update(hash);

        hash = sha256.digest('hex');

        return hash;
    }

    this.send({q:'cookie'}, function (err, data) {
        if (err) {
            callback(err);
            return;
        }

        request.cookie = data.cookie;
        request.hash = makeHash(cjdns.password, request.cookie, request);

        cjdns.send(request, callback);
    });
};

module.exports = CJDNS;