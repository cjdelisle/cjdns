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
var nThen = require('nthen');
var Spawn = require('child_process').spawn;
var Fs = require('fs');

var PYTHONS = ["python", "python2", "python2.7", "python2.6", "python2.5"];

var SCRIPT = [
    'import sys;',
    'print(sys.version_info);',
    'exit(sys.version_info[0] != 2 or sys.version_info[1] < 7);'
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
        callback(new Error("no sutible python2 executable found ( < 2.7 unsupported)"));
    });
};
