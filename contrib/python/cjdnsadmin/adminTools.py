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

import os
import json

from time import sleep

def anonConnect(ip='127.0.0.1', port=11234):
    from cjdnsadmin import connect
    path = os.path.expanduser('~/.cjdnsadmin')
    try:
        with open(path, 'r') as adminInfo:
            data = json.load(adminInfo)
        return connect(data['addr'], data['port'], '')
    except IOError:
        return connect(ip, int(port), '')

def connect(ip='127.0.0.1', port=11234, password=''):
    from cjdnsadmin import connectWithAdminInfo
    return connectWithAdminInfo()

def disconnect(cjdns):
    cjdns.disconnect()

def whoami(cjdns):
    from publicToIp6 import PublicToIp6_convert;
    resp=cjdns.NodeStore_nodeForAddr(0)
    key=resp['result']['key']
    ver=resp['result']['protocolVersion']
    IP=PublicToIp6_convert(key)
    return {'IP':IP,'key':key,'version':ver}

def dumpTable(cjdns,verbose=False,unique_ip=False,nodes=[]):
    if nodes == []: nodes=[]
    rt = []
    i = 0;
    while True:
        table = cjdns.NodeStore_dumpTable(i)
        res=table['routingTable']
        for t in res:
            ip=t['ip']
            if (not ip in nodes) and unique_ip:
                nodes.append(ip)
                rt.append(t)
                if verbose:
                    print(t['ip'] + ' ' + t['path'] + ' ' + str(t['link']) + ' ' + str(t['version']));
            if not unique_ip:
                nodes.append(ip)
                rt.append(t)
                if verbose:
                    print(t['ip'] + ' ' + t['path'] + ' ' + str(t['link']) + ' ' + str(t['version']));
        if not 'more' in table:
            break
        i += 1

    return rt

def streamRoutingTable(cjdns, delay=10):
    known = []

    while True:
        i = 0
        while True:
            table = cjdns.NodeStore_dumpTable(i)
            routes = table['routingTable']
            for entry in routes:
                if entry['ip'] not in known:
                    known.append(entry['ip'])
                    yield entry

            if 'more' not in table:
                break

            i += 1

        sleep(delay)

def peerStats(cjdns,up=False,verbose=False,human_readable=False):
    from publicToIp6 import PublicToIp6_convert;

    allPeers = []

    i = 0;
    while True:
        ps = cjdns.InterfaceController_peerStats(i);
        peers = ps['peers']
        for p in peers:
            if p['state'] == 'UNRESPONSIVE' and up:
                continue
            allPeers.append(p)
        if (not 'more' in ps):
            break
        i += 1

    if verbose:
        STAT_FORMAT = '%s\tv%s\t%s\tin %s\tout %s\t%s\tdup %d los %d oor %d'

        for peer in allPeers:
            ip = PublicToIp6_convert(peer['publicKey'])
			
            b_in  = peer['bytesIn']
            b_out = peer['bytesOut']
            if human_readable:
				b_in  = sizeof_fmt(b_in)
				b_out = sizeof_fmt(b_out)
            
            p = STAT_FORMAT % (ip, peer['version'], peer['switchLabel'],
                               str(b_in), str(b_out), peer['state'],
                               peer['duplicates'], peer['lostPackets'],
                               peer['receivedOutOfRange'])

            if 'user' in peer:
                p += '\t%r' % peer['user']

            print p
    return allPeers

def sizeof_fmt(num):
    for x in ['B','KB','MB','GB','TB']:
        if num < 1024.0:
            return "%3.1f%s" % (num, x)
        num /= 1024.0

def parseLabel(route):
    route = route.replace('.','')
    broute= int('0x' + route, 16);
    route = route.replace('0','x')
    route = route.replace('1','y')
    route = route.replace('f','1111')
    route = route.replace('e','1110')
    route = route.replace('d','1101')
    route = route.replace('c','1100')
    route = route.replace('b','1011')
    route = route.replace('a','1010')
    route = route.replace('9','1001')
    route = route.replace('8','1000')
    route = route.replace('7','0111')
    route = route.replace('6','0110')
    route = route.replace('5','0101')
    route = route.replace('4','0100')
    route = route.replace('3','0011')
    route = route.replace('2','0010')
    route = route.replace('y','0001')
    route = route.replace('x','0000')
    # reverse the string, strip trailing zeros, then strip the trailing 1
    route = route[::-1].rstrip('0')[:-1]
    return {'route':route,'broute':broute}


