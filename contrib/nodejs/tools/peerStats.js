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
var PublicToIp6 = require('./lib/publicToIp6');

Cjdns.connectWithAdminInfo(function (cjdns) {

    var again = function (i) {
        cjdns.InterfaceController_peerStats(i, function (err, ret) {
            if (err) { throw err; }
            ret.peers.forEach(function (peer) {
                p = (PublicToIp6.convert(peer['publicKey']) + '\t' + peer['switchLabel'] +
                     '\tin ' + peer['bytesIn'] + '\tout ' + peer['bytesOut'] + '\t' +
                     peer['state'] +
                     '\tdup ' + peer['duplicates'] +
                     ' los ' + peer['lostPackets'] +
                     ' oor ' + peer['receivedOutOfRange']);
                if (typeof(peer.user) === 'string') {
                    p += ' "' + peer['user'] + '"';
                }
                console.log(p);
            });
            if (typeof(ret.more) !== 'undefined') {
                again(i+1);
            } else {
                cjdns.disconnect();
            }
        });
    };
    again(0);

});
