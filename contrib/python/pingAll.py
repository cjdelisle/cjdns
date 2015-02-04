#!/usr/bin/env python2
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
allRoutes = [];

magicalLinkConstant = 5366870.0;

def pingNode(addr, path, link):
    addrAtPath = addr + '@' + path;
    result = cjdns.RouterModule_pingNode(path, 2000);
    res = '';
    if ('result' in result): res = result['result'];

    if (res == 'pong'):
        if (addrAtPath != result['from']): addrAtPath += ' mismatch ' + result['from'];
        print(addrAtPath +
            ' link ' + str(link) +
            ' proto ' + str(result['protocol']) +
            ' ' + str(result['ms']) + 'ms');
        return True;
    elif (res == 'timeout'):
        print(addrAtPath + ' link ' + str(link) + ' TIMEOUT ' + str(result['ms']) + 'ms');
    else:
        print(addrAtPath + str(result));
    return False;


i = 0;
while True:
    table = cjdns.NodeStore_dumpTable(i);
    routes = table['routingTable'];
    allRoutes += routes;
    if (not 'more' in table):
        break;
    i += 1;

if (len(sys.argv) > 4 and '-s' == sys.argv[4]):
    for route in allRoutes:
        i = 0;
        while i < 3:
            result = cjdns.SwitchPinger_ping(route['path'], route['path'], 10000);
            if (i > 0): print 'attempt ' + str(i) + ' ';
            print result;
            if (result['result'] != 'timeout'): break;
            i += 1;

else:
    i = 0;
    responses = 0;

    for entry in allRoutes:
        path = entry['path'];
        addr = entry['ip'];
#        if (len(sys.argv) > 4 and '-b' == sys.argv[4]):
        skip = False;
        for e in allRoutes:
            if e['ip'] == addr and e['link'] > entry['link']:
                skip = True;
                break;
        if skip == True: continue;

        link = int(entry['link'] / magicalLinkConstant);
        if (entry['link'] == 0): continue;

        for x in range(0, 3):
            if (pingNode(addr, path, link)):
                responses += 1;
                break;
        i += 1;


    print(str(i) + ' total ' + str(responses) + ' respond.');
