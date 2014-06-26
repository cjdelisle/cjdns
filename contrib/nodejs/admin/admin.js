/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

// THIS SCRIPT IS ESSENTIALLY A BACKDOOR
// DON'T RUN IT ON ANYTHING YOU CARE ABOUT

// This script binds port 8084 and allows to use admin methods of cjdns

var sys = require('util'),
    http = require('http'),
    fs = require('fs'),
    express = require('express'),
    path = require('path'),
    app = express(),
    CJDNS = require('./cjdns.js');

var PATH = path.dirname(process.argv[1]),
    config = JSON.parse(fs.readFileSync(process.env['HOME'] + '/.cjdnsadmin')),
    cjdns = new CJDNS(config),
    gLogs = [];

app.use(express.static(PATH + '/www'));
app.use(express.bodyParser());

app.post('/api', function (req, res, next) {
    if (req.body.lorem === 'ipsum') {
        //Kinda handshake
        res.send({hello: 'world!'});
    } else {
        next();
    }
});

app.post('/api/map', function (req, res, next) {
    var nodes = [];

    function getNodes (page) {
        cjdns.sendAuth({
            q: 'NodeStore_dumpTable',
            args: {
                page: page
            }
        }, function (err, data) {
            if (err) {
                res.send('502', 'Something happened!');
                return;
            }

            if (data.routingTable) {
                nodes = nodes.concat(data.routingTable);
            }

            if (data.more) {
                getNodes(page + 1);
            } else {
                checkNodes();
            }
        });
    }

    function checkNodes () {
        res.send({
            nodes: nodes,
            count: nodes.length
        });
    }

    getNodes(0);
});

app.post('/api/methods', function (req, res, next) {
    var methods = [];

    function getMethods (page) {
        cjdns.sendAuth({
            q: 'Admin_availableFunctions',
            args: {
                page: page
            }
        }, function (err, data) {
            var name;

            if (err) {
                res.send('502', 'Something happened!');
                return;
            }

            if (data.availableFunctions) {
                for (name in data.availableFunctions) {
                    if (data.availableFunctions.hasOwnProperty(name)) {
                        methods.push({
                            name: name,
                            params: data.availableFunctions[name]
                        });
                    }
                }
            }

            if (data.more) {
                getMethods(page + 1);
            } else {
                checkMethods();
            }
        });
    }

    function checkMethods () {
        res.send({
            methods: methods,
            count: methods.length
        });
    }

    getMethods(0);
});

app.post('/api/logs', function (req, res, next) {
    var ts = req.body.ts / 1000 || 0,
        logs = gLogs.filter(function (log) {
            if (log.time > ts) {
                return log;
            }
        }),
        response;

    logs.sort(function (a, b) {
        return a.time - b.time;
    });

    response = {
        logs: logs
    };

    if (logs.length > 0) {
        response.ts = logs[logs.length - 1].time * 1000;
    }

    res.send(response);
});

app.post('/api/cjdns', function (req, res, next) {
    if (req.body.q) {
        sys.log('Sending ' + sys.inspect(req.body));
        cjdns.sendAuth(req.body, function (err, msg) {
            if (err) {
                res.send('502', 'Something happened!');
                return;
            }

            res.send(msg);
        });
    } else {
        next();
    }
});

app.post('/api/config', function (req, res, next) {
    if (req.body.config) {
        cjdns.saveConfig(req.body.config, function (err, data) {
            if (err) {
                res.send('502', 'Something happened!');
                return;
            }

            res.send(data);
        });
    } else {
        next();
    }
});

app.post('/api/config', function (req, res, next) {
    //sys.log(sys.inspect(req.body));
    res.send(cjdns.config.interfaces);
});

app.get('/map', function (req, res) {
    res.redirect('/map.html');
});

app.get('/logs', function (req, res) {
    res.redirect('/logs.html');
});

app.get('/config', function (req, res) {
    res.redirect('/config.html');
});

app.get('*', function (req, res) {
    res.send(404, 'Not found!');
});

app.post('*', function (req, res) {
    res.send(404, {error: 'Not found!'});
});

cjdns.subscribe(function (err, resp) {
    if (resp.message) {
        gLogs.push(resp);
    }
});

app.listen(8084,'127.0.0.1',function(){
  console.log('\n****************\nCJDNS admin is started on http://localhost:8084/ \n****************\n ');
});