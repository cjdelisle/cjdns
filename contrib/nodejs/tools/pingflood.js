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

/*
 * test the admin port
 */


var Cjdns = require('../cjdnsadmin/cjdnsadmin');
var nThen = require('../cjdnsadmin/nthen');

Cjdns.connectWithAdminInfo(function (cjdns) {

    nThen(function (waitFor) {

        for (var i = 0; i < 10000; i++) {
            cjdns.RouterModule_pingNode('0000.0000.0000.0001', waitFor(function (err, ret) {
                if (err) { throw err; }
            }));
        }

    }).nThen(function (waitFor) {

        cjdns.disconnect();
    });

});
