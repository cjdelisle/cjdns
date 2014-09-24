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
var Spawn = require('child_process').spawn;
var Http = require("http");
var Fs = require("fs");

var TIMEOUT = 15000;

var parseURL = function (url)
{
    url = url.replace(/[a-zA-Z]*:\/\//, '');
    var path = url.split('/')[1] || '';
    url = url.split('/')[0];
    var port = url.split(':')[1];
    url = url.split(':')[0];
    port = Number(port) || 8083;
    var host = url || undefined;
    return {
        host: host,
        port: port,
        path: '/' + path
    };
};

var remote = module.exports.remote = function (url, argv) {
    var params = parseURL(url);
    if (!params.host) { throw new Error("For the client, the hostname is required"); }
    params.headers = { args: [ JSON.stringify(argv) ] };
    params.method = 'POST';

    return function (fileName, callback) {
        var out = [];
        out.push('Testing Remotely');
        var req = Http.request(params, function(res) {
            if (res.statusCode !== 200) {
                out.push('STATUS: ' + res.statusCode);
                out.push('HEADERS: ' + JSON.stringify(res.headers));
            }
            res.setEncoding('utf8');
            var body = '';
            res.on('data', function (chunk) { body += String(chunk); });
            res.on('end', function () {
                var ret = JSON.parse(body);
                if (ret.returnCode !== 0) {
                    output.push(ret.stdout);
                }
                out.push(ret.stderr);
                callback(out.join('\n'), (ret.returnCode !== 0));
            });
        });
        Fs.createReadStream(fileName).pipe(req);
        req.on('error', function(e) {
            console.log('problem with request: ' + e.message);
        });
    };
};

var client = function (url, fileName, argv) {
    var params = parseURL(url);

    if (!params.host) { throw new Error("For the client, the hostname is required"); }

    params.headers = { args: [ JSON.stringify(argv) ] };
    params.method = 'POST';

    var req = Http.request(params, function(res) {
        if (res.statusCode !== 200) {
            console.log('STATUS: ' + res.statusCode);
            console.log('HEADERS: ' + JSON.stringify(res.headers));
        }
        res.setEncoding('utf8');
        var body = '';
        res.on('data', function (chunk) { body += String(chunk); });
        res.on('end', function () {
            var ret = JSON.parse(body);
            process.stdout.write(ret.stdout);
            process.stderr.write(ret.stderr);
            process.exit(ret.returnCode);
        });
    });
    Fs.createReadStream(fileName).pipe(req);
    req.on('error', function(e) {
        console.log('problem with request: ' + e.message);
    });
};


/// Server

var spawnProc = function(file, args, callback, timeoutMilliseconds) {
    var child = Spawn(file, args);
    var out = '', err = '';
    var to = setTimeout(function() {
        child.kill('SIGKILL');
        err += "TIMEOUT\n";
        callback(1000, out, err);
    }, timeoutMilliseconds);
    child.stdout.on('data', function (data) { out += String(data); });
    child.stderr.on('data', function (data) { err += String(data); });
    child.on('close', function (code) {
        clearTimeout(to);
        callback(code, out, err);
    });
    child.on('error', function(err) {
        clearTimeout(to);
        callback(1, '', err.stack);
    });
};

var local = module.exports.local = function (argv) {
    return function (fileName, callback) {
        var output = [];
        spawnProc(fileName, argv, function (code, out, err) {
            if (code !== 0) {
                output.push(out);
            }
            output.push(err);
            callback(output.join('\n'), (code !== 0));
        }, TIMEOUT);
    };
};

var send = function (response, content) {
    response.writeHeader(200, {"Content-Type": "text/plain"});
    response.write(content);
    response.end();
};

var runTest = function (fileName, args, response, timeoutMilliseconds) {
    Fs.chmodSync(fileName, '755');
setTimeout(function () {
    spawnProc(fileName, args, function(code, out, err) {
        send(response, JSON.stringify({ returnCode: code, stdout: out, stderr: err }));
        //Fs.unlink(fileName);
    }, timeoutMilliseconds);
}, 100);
};

var server = function (url, tempDir) {
    var params = parseURL(url);
    tempDir = tempDir || '/tmp';

    console.log("Serving http://" + (params.host || '<any>') + ':' + params.port + params.path);

    Http.createServer(function(request, response) {
        if (request.method === 'POST' && request.url === params.path) {
            var args = JSON.parse(request.headers.args);
            var fileName = tempDir + "/test-" + String(Math.random()).substring(2) + ".exe";
            request.pipe(Fs.createWriteStream(fileName));
            request.on("end", function() {
                runTest(fileName, args, response, TIMEOUT);
            });
        } else {
            response.end();
        }
    }).listen(params.port, params.host);
};

/// Main

var usage = function ()
{
    console.log(
        "Usage:\n" +
        "  TestRunner server [http://][<hostname>][:<port>][/<password>] [tempDir]\n" +
        "    bind to <port> (default port is 8083) and allow remote execution by anyone\n" +
        "    providing <password>. <tempDir> will be used for temporary files (default is /tmp)\n" +
        "    http:// is optional, you can put https:// or penispenispenis:// or whatever you\n" +
        "    want but only http is supported.\n" +
        "    To bind to all interfaces, you can omit the hostname, eg: :3333/supersecret\n" +
        "    and to have the default port, you can provide only the password eg: /secret\n" +
        "    or the hostname and password, eg:  mycomputer/secret\n" +
        "    The server will print a line explaining exactly what hostname/port/pass it is\n" +
        "    serving.\n" +
        "\n" +
        "  TestRunner client [http://][<hostname>][:<port>][/<password>] <executable> [argv1, " +
            "[argv2, ...]]\n" +
        "    connect to the specified URL and upload <exectable> to be run with\n" +
        "    arguments <argv1> <argv2> ...\n" +
        "    the URL has the same semantics as that for the server except the hostname must be\n" +
        "    specified.");
};

var main = function (argv)
{
    var cli = argv.indexOf("client");
    if (cli !== -1) {
        argv.splice(0,cli+1);
        var cliUrl = argv.shift();
        var fileName = argv.shift();
        return client(cliUrl, fileName, argv);
    }
    var serv = argv.indexOf("server");
    if (serv !== -1) {
        argv.splice(0,serv+1);
        var servUrl = argv.shift();
        var tempDir = argv.shift();
        return server(servUrl, tempDir);
    }
    return usage();
};

if (!module.parent) {
    main(process.argv);
}
