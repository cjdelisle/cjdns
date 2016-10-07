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

var Fs_stat = function (file, callback) {
    Fs.stat(file, function (err, ret) {
        if (err === 'ENOENT') {
            console.log("Magical ENOENT on a file which definitely does exist, good job Linus");
            setTimeout(function () {
                Fs_stat(file, callback);
            }, 1000);
        } else {
            callback(err, ret);
        }
    });
};

var getTests = function (file, tests, isSubnode, callback) {
    if (/\/(.git|build_.*|node_build|contrib)\//.test(file)) { callback(); return; }
    if (isSubnode && /\/dht\//.test(file)) { callback(); return; }
    Fs_stat(file, function (err, stat) {
        if (err) { throw err; }
        if (stat.isDirectory()) {
            nThen(function (waitFor) {
                Fs.readdir(file, waitFor(function (err, list) {
                    if (err) { throw err; }
                    list.forEach(function (subFile) {
                        getTests(file + '/' + subFile, tests, isSubnode, waitFor());
                    });
                }));
            }).nThen(function (waitFor) {
                callback();
            });
            return;
        } else if (/_test\.c$/.test(file) && tests.indexOf(file) === -1) {
            tests.push(file);
        }
        callback();
    });
};

var generate = module.exports.generate = function (file, builder, isSubnode, callback)
{
    var tests = [];
    getTests('.', tests, isSubnode, function () {
        var prototypes = [];
        var listContent = [];
        tests.forEach(function (test) {
            var main = /^.*\/([^\/]+)\.c$/.exec(test)[1] + '_main';
            (builder.config['cflags'+test] =
                builder.config['cflags'+test] || []).push('-D', 'main='+main);
            file.links.push(test);
            listContent.push('{ .func = '+main+', .name = "'+test.replace(/^.*\/|.c$/g, '')+'" },');
            prototypes.push('int '+main+'(int argc, char** argv);');
        });
        file.testcjdroute_tests = listContent.join('\n');
        file.testcjdroute_prototypes = prototypes.join('\n');
        callback();
    });
};
