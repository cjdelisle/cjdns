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

var getAddresses = function (cjdns, callback) {
    var addresses = [];
    var again = function (i) {
        cjdns.NodeStore_dumpTable(i, function (err, table) {
            if (err) { throw err; }
            var j;
            for (j = 0; j < table.routingTable.length; j++) {
                var r = table.routingTable[j];
                if (addresses.indexOf(r.ip) === -1) { addresses.push(r.ip); }
            }
            if (j) {
                again(i+1);
            } else {
                callback(addresses);
            }
        });
    };
    again(0);
};

var buildTreeCycle = function (current, nodes) {
    current.peers = [];
    for (var i = nodes.length - 1; i >= 0; i--) {
        if (nodes[i].bestParent && nodes[i].bestParent.ip == current.ip) {
            current.peers.push(nodes[i]);
            nodes.splice(i, 1);
        }
    }
    for (var i = 0; i < current.peers.length; i++) {
        buildTreeCycle(current.peers[i], nodes);
    }
    return current;
};

var buildTree = function (origNodes) {
    var nodes = [];
    nodes.push.apply(nodes, origNodes);
    for (var i = 0; i < nodes.length; i++) {
        if (nodes[i].bestParent && nodes[i].ip === nodes[i].bestParent.ip) {
            var current = nodes[i];
            nodes.splice(i, 1);
            var out = buildTreeCycle(current, nodes);
            //if (nodes.length > 0) { throw new Error(); }
            return out;
        }
    }
    throw new Error();
};

var printTree = function (tree, spaces) {
    console.log(spaces + tree.ip + ' ' + tree.bestParent.parentChildLabel + '  ' + tree.routeLabel + '  ' + tree.reach);
    for (var i = 0; i < tree.peers.length; i++) {
        printTree(tree.peers[i], spaces + '  ');
    }
};

Cjdns.connectWithAdminInfo(function (cjdns) {

    var addrs;
    var nodes = [];
    nThen(function (waitFor) {
        getAddresses(cjdns, waitFor(function (addrs) { addresses = addrs; }));
    }).nThen(function (waitFor) {
        var nt = nThen;
        addresses.forEach(function (addr) {
            nt = nt(function (waitFor) {
                cjdns.NodeStore_nodeForAddr(addr, waitFor(function (err, ret) {
                    if (err) { throw err; }
                    if (ret.error !== 'none') { throw new Error(ret.error); }
                    ret.result.ip = addr;
                    nodes.push(ret.result);
                }));
            }).nThen;
        });
        nt(waitFor());
    }).nThen(function (waitFor) {
        //console.log(JSON.stringify(nodes, null, '  '));
        var tree = buildTree(nodes);
        printTree(tree, '');
        cjdns.disconnect();
    });

});
