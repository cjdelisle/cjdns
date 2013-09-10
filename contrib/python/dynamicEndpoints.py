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

from cjdnsadmin.cjdnsadmin import connectWithAdminInfo;
from cjdnsadmin.bencode import *
import sys;
import socket,re
import select
import time
import os

class Node(object):
    __slots__ = ("host","port","password","key","lastAddr")
    def __init__(self,host,port,password,key):
        self.host = host
        self.port = port
        self.password = password
        self.key = key
        self.lastAddr = None

nodes = dict()
unresponsive = dict()
def addNode(host,port,password,key):
    nodes[key] = Node(host, port, password, key)

addNode("verge.info.tm",6324,"ns6vn00hw0buhrtc4wbk8sv230",
        "lhjs0njqtvh1z4p2922bbyp2mksmyzf5lb63kvs3ppy78y1dj130.k")

nowait = os.environ.get('nowait',False)

def lookup(node):
    for info in socket.getaddrinfo(node.host,node.port,
                                   socket.AF_UNSPEC,socket.SOCK_DGRAM):
        sockaddr = info[-1]
        #if node.lastAddr == sockaddr: return
        node.lastAddr = sockaddr
        sockaddr = sockaddr[0] + ":" + str(sockaddr[1])
        print("Connecting to {} {}".format(node.key,sockaddr))
        cjdns.UDPInterface_beginConnection(password=node.password,
                                           publicKey=node.key,
                                           address=sockaddr)
        try: del unresponsive[node.key]
        except KeyError: pass
        break

isUnresponsive = re.compile("Pinging unresponsive neighbor \\[(.*\\.k)\\]\\.")

assert(isUnresponsive.match("Pinging unresponsive neighbor [lhjs0njqtvh1z4p2922bbyp2mksmyzf5lb63kvs3.k]."))

def doLog(message):
    if message[0]!='P': return;
    p = isUnresponsive.match(message)
    if not p: return
    downKey = p.group(1)
    node = nodes.get(downKey,None)
    if not node:
        #print("Unknown neighbor {}".format(node))
        return
    unresponsive[downKey] = node
    lookup(node)

def recieve(sock):
    allData = '';
    error = 0;
    while True:
        data = sock.recv(8192);
        if not data:
            if data == "":
                time.sleep(1)
                select.select([sock],[],[],1)
                print("heartb8")
            continue
        allData = allData + data;
        try:
            output, index = bdecode_stream(allData);
        except (IndexError):
            print("bad dater '{}'".format(allData))
            continue;
        except (KeyError, ValueError) as er:
            print(er)
            if (error > 5):
                allData = "";
                error = 0;
            else:
                error = error + 1;

            continue;

        if 'message' in output:
            doLog(output['message']);
        allData = allData[index:];

cjdns = connectWithAdminInfo();

sub = cjdns.AdminLog_subscribe('', 'DefaultInterfaceController.c', 'DEBUG');
if (sub['error'] == 'none'):
    for node in nodes.values():
        lookup(node)
    if nowait: raise SystemExit
    recieve(cjdns.socket);
else:
    print(sub);
