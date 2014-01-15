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
var nThen = require('../cjdnsadmin/nthen');

var getKey = function (nodeId) {
    return nodeId.replace(/v[0-9]*\.[0-9a-f\.]{20}/, '');
};

var print = function (map) {
    var done = [];
    p = function (key, id, spaces) {
        var peers = map[key];
        if (!peers || done.indexOf(key) > -1) { return; }
        done.push(key);
        console.log(spaces + id);
        for (var i = 0; i < peers.length; i++) {
            var k = getKey(peers[i]);
            if (k === key) { continue; }
            p(k, peers[i], spaces + '  ');
        }
    };
    p(getKey(map.selfNode), map.selfNode, '')
};

Cjdns.connectWithAdminInfo(function (cjdns) {

    var addrs = [];
    var queried = [];

    var map = {};

    nThen(function (waitFor) {

        cjdns.RouterModule_getPeers('0000.0000.0000.0001', waitFor(function (err, ret) {
            if (err) { throw err; }
            if (ret.error !== 'none') { throw new Error(ret.error); }
            for (var i = 0; i < ret.peers.length; i++) {
                if (ret.peers[i].indexOf('0000.0000.0000.0001') > -1) {
                    addrs.push(ret.peers[i]);
                    map.selfNode = ret.peers[i];
                    return;
                }
            }
            throw new Error(JSON.stringify(ret, null, '  '));
        }));

    }).nThen(function (waitFor) {
        var more = function () {
            if (addrs.length === 0) { return; }
            var addr = addrs.shift();
            var key = getKey(addr);
            if (queried.indexOf(key) > -1) {
                more();
                return;
            }
            cjdns.RouterModule_getPeers(addr, waitFor(function (err, ret) {
                if (err) { throw err; }
                if (ret.error !== 'none') { throw new Error(ret.error); }
                for (var i = 0; i < ret.peers.length; i++) {
                    addrs.push(ret.peers[i]);
                }
                map[key] = ret.peers;
                queried.push(key);
                more();
            }));
        };
        more();

    }).nThen(function (waitFor) {
        //console.log(JSON.stringify(map, null, '  '));
        print(map);
        cjdns.disconnect();
    });

});
