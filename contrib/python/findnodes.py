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
import os;
from cjdnsadmin.cjdnsadmin import connectWithAdminInfo;
import json;
from pprint import pprint;
from time import sleep;

cjdns = connectWithAdminInfo();

nodes = {};
while (1):
    i = 0;
    newNodes = [];
    while True:
        table = cjdns.NodeStore_dumpTable(i);
        routes = table['routingTable'];
        for entry in routes:
            if (not entry['ip'] in nodes):
                nodes[entry['ip']] = 1;
                newNodes.append(entry);
        if (not 'more' in table):
            break;
        i += 1;
    for entry in newNodes:
        print json.dumps(entry);
    sleep(30);
