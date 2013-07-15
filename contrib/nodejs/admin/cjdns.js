var dgram = require('dgram'),
    bencode = require('bencode'),
    sys = require('util'),
    crypto = require('crypto'),
    path = require('path'),
    fs = require('fs');

var CJDNS = function (config) {
    this.configFile = path.resolve(config.config.replace(/^~\//, process.env.HOME + '/'));
    this.config = fs.readFileSync(this.configFile);

    if (!this.config) {
        throw 'Can\'t read config file! path: ' + this.configFile;
    }

    this.oldConfig = this.config;

    this.parseConfig();

    this.host = (this.config.admin.bind || 'localhost:11234').split(':');
    this.port = this.host[1] || '80';
    this.host = this.host[0];

    this.password = this.config.admin.password;

    this.send({q: 'ping'}, function (err, msg) {
        if (msg && msg.q === 'pong') {
            sys.log('CJDNS Admin backend found and ready to work!');
        } else {
            sys.log(msg);
        }
    });
};

CJDNS.prototype.parseConfig = function() {
    function removeComments(str) {
        /*
        http://james.padolsey.com/javascript/removing-comments-in-javascript/
         */
        str = ('__' + str + '__').split('');
        var mode = {
            singleQuote: false,
            doubleQuote: false,
            regex: false,
            blockComment: false,
            lineComment: false,
            condComp: false
        };
        for (var i = 0, l = str.length; i < l; i++) {

            if (mode.regex) {
                if (str[i] === '/' && str[i-1] !== '\\') {
                    mode.regex = false;
                }
                continue;
            }

            if (mode.singleQuote) {
                if (str[i] === "'" && str[i-1] !== '\\') {
                    mode.singleQuote = false;
                }
                continue;
            }

            if (mode.doubleQuote) {
                if (str[i] === '"' && str[i-1] !== '\\') {
                    mode.doubleQuote = false;
                }
                continue;
            }

            if (mode.blockComment) {
                if (str[i] === '*' && str[i+1] === '/') {
                    str[i+1] = '';
                    mode.blockComment = false;
                }
                str[i] = '';
                continue;
            }

            if (mode.lineComment) {
                if (str[i+1] === '\n' || str[i+1] === '\r') {
                    mode.lineComment = false;
                }
                str[i] = '';
                continue;
            }

            if (mode.condComp) {
                if (str[i-2] === '@' && str[i-1] === '*' && str[i] === '/') {
                    mode.condComp = false;
                }
                continue;
            }

            mode.doubleQuote = str[i] === '"';
            mode.singleQuote = str[i] === "'";

            if (str[i] === '/') {

                if (str[i+1] === '*' && str[i+2] === '@') {
                    mode.condComp = true;
                    continue;
                }
                if (str[i+1] === '*') {
                    str[i] = '';
                    mode.blockComment = true;
                    continue;
                }
                if (str[i+1] === '/') {
                    str[i] = '';
                    mode.lineComment = true;
                    continue;
                }
                mode.regex = true;

            }

        }
        return str.join('').slice(2, -2);
    }

    this.config = JSON.parse(removeComments(this.config));
};

CJDNS.prototype.checkConfig = function (config) {
    return config && config.UDPInterface && config.UDPInterface[0] && config.UDPInterface[0].connectTo;
};

CJDNS.prototype.saveConfig = function(newConfig, callback) {
    var conf,
        reserveConf;

    if (this.checkConfig(newConfig)) {
        conf = JSON.parse(JSON.stringify(this.config));
        conf.interfaces = newConfig;
        this.config = JSON.parse(JSON.stringify(conf));
        conf = JSON.stringify(conf, null, 4);

        reserveConf = path.dirname(this.configFile) + '/' + path.basename(this.configFile) + '.' + (new Date()).getTime() + path.extname(this.configFile);

        //save current version to yourConfDir/<confName>.<timestamp>.conf
        fs.writeFile(
            reserveConf,
            this.oldConfig
        );

        //save new config
        fs.writeFile(this.configFile, conf, function (err, data) {
            callback(err, {
                msg: 'Old config saved to "' + reserveConf + '"'
            });
        });
        this.oldConfig = conf;
    } else {
        callback({
            error: 'Config is not valid!'
        });
    }
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