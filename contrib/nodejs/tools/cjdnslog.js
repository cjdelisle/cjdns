#!/usr/bin/env node
/* vim: set expandtab ts=4 sw=4: */
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

var Cjdns = require('../cjdnsadmin/cjdnsadmin');

var usage = function () {
    console.log("Usage: cjdnslog.js [-v <verbosity>] [-f <filename>] [-l <lineNum>]");
    console.log("cjdnslog.js -v INFO                 <-- log all INFO and higher (WARN, ERROR, or CRITICAL) messages.");
    console.log("cjdnslog.js                         <-- log everything");
    console.log("cjdnslog.js -f CryptoAuth.c         <-- log everything in CryptoAuth.c");
    console.log("cjdnslog.js -v INFO -f CryptoAuth.c <-- log INFO and higher in CryptoAuth.c");
    console.log("cjdnslog.js -f CryptoAuth.c -l 747  <-- print messages from log statement on line 747 of CryptoAuth.c");
    console.log("cjdnslog.js -l 747                  <-- print messages from log statements on line 747 of any file at all.");
};

if (process.argv[process.argv.length-1] === '--help') {
    usage();
    process.exit(0);
}

var printMsg = function (data) {
    console.log(data['time'] + ' ' + data['level'] + ' ' + data['file'] + ':' + data['line'] + ' ' + data['message']);
}

Cjdns.connectWithAdminInfo(function (cjdns) {

    var n;
    var verbosity;
    var file;
    var line;
    if ((n = process.argv.indexOf('-v')) !== -1) { verbosity = process.argv[n+1]; }
    if ((n = process.argv.indexOf('-f')) !== -1) { file = process.argv[n+1]; }
    if ((n = process.argv.indexOf('-l')) !== -1) { line = process.argv[n+1]; }

    cjdns.setDefaultHandler(function (err, msg) {
        if (err) { throw err; }
        printMsg(msg);
    });
    cjdns.AdminLog_subscribe(line, file, verbosity, function (err, ret) {
        if (err) { throw err; }

        // make sure cjdns doesn't think we've gone missing!
        setInterval(function () {
            cjdns.ping(function (err, ret) {
                if (err) { throw err; }
            });
        }, 10000);
    });
});
