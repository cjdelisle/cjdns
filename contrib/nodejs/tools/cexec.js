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

Cjdns.connectWithAdminInfo(function (cjdns) {
    var code = process.argv[process.argv.length-1].replace(/\).*$/, ',cb);');
    code = code.replace('(,cb);', '(cb);');
    var f = new Function('x', 'cb', 'x.' + code);
    f(cjdns, function (err, ret) {
        if (err) { throw err; }
        console.log(JSON.stringify(ret, null, '  '));
        cjdns.disconnect();
    });
});
