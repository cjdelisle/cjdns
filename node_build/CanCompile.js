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
var nThen = require("nthen");
var Fs = require("fs");

module.exports.check = function (builder, code, cflags, callback) {

    var file = builder.tmpFile();
    var outputFile = builder.tmpFile();

    nThen(function (waitFor) {

        Fs.writeFile(file, code, waitFor(function (err, ret) {
            if (err) {
                waitFor.abort();
                callback(err);
            }
        }));

    }).nThen(function (waitFor) {

        var flags = [];
        flags.push.apply(flags, cflags);
        flags.push.apply(flags, ["-o", outputFile, file]);

        builder.cc([cflags, "-o", outputFile, file], waitFor(function (ret, out, err) {
            if (ret) {
                callback(err, false);
            } else {
                callback(undefined, true);
            }
        }));

    });
};
