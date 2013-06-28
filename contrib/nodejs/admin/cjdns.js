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

CJDNS.prototype.send = function(data, callback, otherSocket) {
    var msg = new Buffer(bencode.encode(data)),
        socket = otherSocket || dgram.createSocket('udp4');

    socket.on('message', function (msg) {
        var response = bencode.decode(msg, 'utf8');
        callback(null, response);

        if (!otherSocket) {
            socket.close();
        }
    });

    socket.send(msg, 0, msg.length, this.port, this.host, function(err, bytes) {
        if (err) {
            callback(err);
        }
    });
};

CJDNS.prototype.sendAuth = function(data, callback, otherSocket) {
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

        cjdns.send(request, callback, otherSocket);
    });
};

CJDNS.prototype.subscribe = function (callback) {
    var logsSocket = dgram.createSocket('udp4'),
        cjdns = this;

    if (this.logsId) {
        this.unsubscribe();
    }

    if (this.logsPing) {
        clearInterval(this.logsPing);
        this.logsPing = undefined;
    }

    this.sendAuth({
        q: 'AdminLog_subscribe',
        args: {
            level: 'KEYS'
        }
    }, callback, logsSocket);

    this.logsPing = setInterval(function () {
        cjdns.sendAuth({
            q: 'ping'
        }, function () {}, logsSocket);
    }, 9000);
};

CJDNS.prototype.unsubscribe = function (callback) {
    if (this.logsId) {
        this.sendAuth({
            q: 'AdminLog_unsubscribe',
            args: {
                streamId: this.logsId
            }
        });
    }
};

module.exports = CJDNS;