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
var Fs = require("fs");
var nThen = require("nthen");

var Tests = module.exports;

var getTests = function (file, tests, callback) {
    if (/\/(.git|buildjs|node_build|contrib|windows)\//.test(file)) { callback(); return; }
    Fs.stat(file, function (err, stat) {
        if (err) { throw err; }
        if (stat.isDirectory()) {
            nThen(function (waitFor) {
                Fs.readdir(file, waitFor(function (err, list) {
                    if (err) { throw err; }
                    list.forEach(function (subFile) {
                        getTests(file + '/' + subFile, tests, waitFor());
                    });
                }));
            }).nThen(function (waitFor) {
                callback();
            });
            return;
        } else if (/_test\.c$/.test(file)) {
            tests.push(file);
        }
        callback();
    });
};

var get = Tests.get = function (callback) {
    var tests = [];
    getTests('.', tests, function () {
        callback(tests);
    });
};
