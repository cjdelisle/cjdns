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

// This script binds port 8083 and allows remote testing on the system.

var spawn = require('child_process').spawn;
var http = require("http");
var qs = require('querystring');
var os = require("os");
var fs = require("fs");

var PATH = "/home/user/";

var spawnProc = function(file, callback, timeoutMilliseconds) {
    var child = spawn(file);
    var out = '', err = '';
    var to = setTimeout(function() {
        child.kill('SIGKILL');
        err += "TIMEOUT\n";
        callback(1000, out, err);
    }, timeoutMilliseconds);
    child.stdout.on('data', function (data) { out += data; });
    child.stderr.on('data', function (data) { err += data; });
    child.on('exit', function (code) {
        callback(code, out, err);
    });
};

var send = function(response, content) {
    response.writeHeader(200, {"Content-Type": "text/plain"});
    response.write(content);
    response.end();
};

var runTest = function(fileName, response, timeoutMilliseconds) {
    fs.chmodSync(fileName, '755');
    spawnProc(fileName, function(code, out, err) {
        send(response, "d4:codei" + code + "e6:stdout" + out.length + ":" + out + "6:stderr" +
                           err.length + ":" + err + "e");
        fs.unlink(fileName);
    }, timeoutMilliseconds);
};

http.createServer(function(request, response) {
    if (request.method == 'POST') {
        var fileName = PATH + (Math.random() * 0x100000000) + ".exe";
        var fsStream = fs.createWriteStream(fileName);
        request.on("data",  function(chunk) {
            fsStream.write(chunk);
        });
        request.on("end", function() {
            fsStream.on("close", function() {
                runTest(fileName, response, 10000);
            });
            fsStream.end();
        });
    } else {
        send(response, "ERROR\nNot a POST");
    }
}).listen(8083);
