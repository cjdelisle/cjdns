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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
'use strict';
var nThen = require('nthen');
var Spawn = require('child_process').spawn;
var Fs = require('fs');

// We're going to prefer python2.7 or python2 because we know they work
// and our test code is very strict.
// If neither of those work, we'll try python3.7 or python3 which we
// de-prioritize because the testing script accepts ANY python3 version
// (as of this writing, we don't know what python3 versions actually work)
// whereas we know that python2.7 is the only working python2 version.
var PYTHONS = ["python2", "python2.7", "python3.7", "python3.8",
               "python3.9", "python3", "python"];

var SCRIPT = [
    'import sys',
    'print(sys.version_info)',

    // we know <= 2.6 is no good
    'if sys.version_info[0] == 2 and sys.version_info[1] >= 7: exit(0)',

    // Lets hope that all python3 versions are ok...
    'if sys.version_info[0] == 3: exit(0)',
].join('\n');

var find = module.exports.find = function (tempFile, callback) {
    var nt = nThen(function (waitFor) {
        Fs.writeFile(tempFile, SCRIPT, waitFor(function (err) { if (err) { throw err; } }));
    }).nThen;
    PYTHONS.forEach(function (python) {
        nt = nt(function (waitFor) {
            console.log("testing python " + python);
            var py = Spawn(python, [tempFile]);
            var cont = waitFor();
            py.stdout.on('data', function (dat) { console.log(dat.toString('utf8')); });
            py.on('close', function(ret) {
                if (ret === 0) {
                    Fs.exists(tempFile, waitFor(function (exists) {
                        if (!exists) { return; }

                        Fs.unlink(tempFile, waitFor(function (err) {
                            if (err) { throw err; }
                        }));
                    }));
                    callback(undefined, python);
                    waitFor.abort();
                } else {
                    cont();
                }
            });
            py.on('error', function (err) {
                if (err !== 'ENOENT') { console.log('error starting python ' + err); }
            });
            // Don't worry about errors, try the next.
        }).nThen;
    });
    nt(function (waitFor) {
        callback(new Error("no sutible python2 or python3 executable found ( < 2.7 unsupported)"));
    });
};
