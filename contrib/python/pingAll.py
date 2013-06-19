#!/usr/bin/env python
# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

import sys;
from cjdnsadmin.cjdnsadmin import connectWithAdminInfo;

cjdns = connectWithAdminInfo();
addresses = {};
allRoutes = [];

i = 0;
while True:
    table = cjdns.NodeStore_dumpTable(i);
    routes = table['routingTable'];
    allRoutes += routes;
    for entry in routes:
        if (entry['link'] != 0):
            addresses[entry['ip']] = entry['path'];
        #print entry['ip'] + '@' + entry['path'] + ' - ' + str(entry['link']);
    if (not 'more' in table):
        break;
    i += 1;

i = 0;
responses = 0;
for addr in addresses:
    if (len(sys.argv) > 4 and addr != sys.argv[4]):
        continue;
    path = cjdns.RouterModule_lookup(addr);
    if (len(path['result']) != 19):
        continue;
    result = cjdns.RouterModule_pingNode(addr, 2000);
    if ('result' in result and result['result'] == 'pong'):
        responses += 1;
    print(addr + '@' + path['result'] + ' - ' + str(result));
    for entry in allRoutes:
        if (entry['ip'] == addr):
            print(entry['path'] + '  ' + str(entry['link']));
    i += 1;

print(str(i) + ' total ' + str(responses) + ' respond.');
