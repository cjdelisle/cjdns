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
        var entry = map[key];
        if (!entry || done.indexOf(key) > -1) { return; }
        done.push(key);
        console.log(spaces + id);
        for (var i = 0; i < entry.peers.length; i++) {
            var k = getKey(entry.peers[i]);
            if (k === key || map[k].bestParent != key) { continue; }
            p(k, entry.peers[i], spaces + '  ');
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
        var again = function (addr) {
            var key = getKey(addr);
            cjdns.RouterModule_getPeers(addr, 200, waitFor(function (err, ret) {
                if (err) { throw err; }
console.log(addr + ' -> ' + JSON.stringify(ret, null, '  '));
if (typeof(ret.peers) === 'undefined') { return; }
                if (ret.error !== 'none') { throw new Error(ret.error); }
                for (var i = 0; i < ret.peers.length; i++) {
                    var ckey = getKey(ret.peers[i]);
                    if (queried.indexOf(ckey) === -1) {
                        (map[ckey] = map[ckey] || {}).bestParent = key;
                        queried.push(ckey);
                        again(ret.peers[i]);
                    }
//console.log(ret.peers[i]);
                }
                (map[key] = map[key] || {}).peers = ret.peers;
            }));
        };
        again(addrs.pop());

    }).nThen(function (waitFor) {
        console.log(JSON.stringify(map, null, '  '));
        //var tree = buildTree(map);
        print(map);
        cjdns.disconnect();
    });

});
