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

Cjdns.connectWithAdminInfo(function (cjdns) {

    var nodes = [];
    var lags = [];
    nThen(function (waitFor) {

        var again = function (i) {
            cjdns.NodeStore_dumpTable(i, waitFor(function (err, table) {
                if (err) { throw err; }
                var j;
                for (j = 0; j < table.routingTable.length; j++) {
                    var r = table.routingTable[j];
                    nodes.push(r);
                }
                if (j) {
                    again(i+1);
                }
            }));
        };
        again(0);

    }).nThen(function (waitFor) {

        nodes.sort(function (a,b) { return (Number(a.link) < Number(b.link)) ? 1 : -1 });

        var ips = [];
        var uniques = [];
        nodes.forEach(function (node) {
            if (node.path !== '0000.0000.0000.0001' && ips.indexOf(node.ip) === -1) {
                uniques.push(node);
                ips.push(node.ip);
            }
        });

        var nt = nThen;
        uniques.forEach(function (node) {
            nt = nt(function (waitFor) {
                cjdns.NodeStore_nodeForAddr(node.ip, waitFor(function (err, ret) {
                    if (err) { throw err; }
                    process.stdout.write(node.ip + '@' + ret.result.routeLabel);
                }));
            }).nThen(function (waitFor) {
                cjdns.RouterModule_pingNode(node.ip, 3000, waitFor(function (err, ret) {
                    if (err) { throw err; }
                    if (ret.result === 'pong') {
                        process.stdout.write('  ' + ret.ms + 'ms  linkq:' + node.link);
                        lags.push(Number(ret.ms));
                    } else if (ret.error === 'not_found') {
                        process.stdout.write('  not_found');
                    } else {
                        process.stdout.write('  ' + JSON.stringify(ret));
                        lags.push(3000);
                    }
                    process.stdout.write('\n');
                }));
            }).nThen;
        });

        nt(waitFor());

    }).nThen(function (waitFor) {
        var totalLag = 0;
        for (var i = 0; i < lags.length; i++) { totalLag += lags[i]; }
        console.log("Average lag: " + (totalLag / lags.length));
        cjdns.disconnect();
    });

});
