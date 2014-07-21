#!/usr/bin/python2
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

# This holds a regex that matches the message we get from the roiuter when it
# sees an unresponsive peer.
IS_UNRESPONSIVE = re.compile(
    "Pinging unresponsive peer \\[(.*\\.k)\\] lag \\[.*\\]")
    
# Make sure that it works
assert(IS_UNRESPONSIVE.match("Pinging unresponsive peer " +
    "[6fmmn3qurcjg6d8hplq1rrcsspfhvm1900s13f3p5bv2bb4f4mm0.k] lag [207147]"))


class Node(object):
    """
    Represents a remote peer.
    """
    __slots__ = ("host","port","password","key","lastAddr")
    def __init__(self,host,port,password,key):
        self.host = host
        self.port = port
        self.password = password
        self.key = key
        self.lastAddr = None
        
class DynamicEndpointWatcher(object):
    """
    Encapsulates all the stuff we need to actually keep an eye on our remote
    nodes and see if they change IPs. When a node with a dynamic IP is
    unresponsive, we look up its IP address and tell cjdns to go connect to it.
    """
    
    def __init__(self, cjdns, configFile = None):
        """
        Set up a new DynamicEndpointWatcher operating on the given CJDNS admin
        connection, using the specified config file name.
        
        """
        
        # Keep the cjdns admin connection
        self.cjdns = cjdns

        # Holds a dict from public key string to Node object for the remote
        # peer, for all known nodes.
        self.nodes = dict()
        
        # Holds a dict from public key to Node object for those nodes which are
        # unresponsive.
        self.unresponsive = dict()
        
        # Holds a cjdns log message subscription to messages about unresponsive
        # nodes. Note that this points specifically to a source line number in
        # the cjdns C code and is thus going to break whenever anyone touches
        # that file. TODO: check node responsiveness through the API.
        self.sub = self.cjdns.AdminLog_subscribe(252, 'InterfaceController.c',
            'DEBUG')
            
        # This is just an example... should use a configuration file for this
        # probably.
        self.addNode("verge.info.tm",6324,"ns6vn00hw0buhrtc4wbk8sv230",
                "lhjs0njqtvh1z4p2922bbyp2mksmyzf5lb63kvs3ppy78y1dj130.k")
            
        if (self.sub['error'] == 'none'):
            for node in self.nodes.values():
                self.lookup(node)
            print("Peers added!")
            # This is not not not the wrong way to do things.
            if os.environ.get('nowait',False): raise SystemExit
            print("Watching for unresponsive peers")
            self.recieve(self.sub['txid'])
        else:
            print(self.sub)
        
        
        
    def addNode(self, host, port, password, key):
        """
        Add a new remote peer with the given hostname, port, password, and
        public key. Does not automatically try to connect to the remote node.
        
        """
        
        self.nodes[key] = Node(host, port, password, key)
        
    def lookup(self, node):
        """
        Look up the current IP address for the given Node object, and tell the
        cjdns router to try to connect to it.
        
        """
        
        for info in socket.getaddrinfo(node.host,node.port,
                                       socket.AF_UNSPEC,socket.SOCK_DGRAM):
            
            # For every IP address the node has in DNS, with the port we wanted
            # attached...
            
            # Save the last address in the list (which must exist) in a field in
            # the node.
            sockaddr = info[-1]
            node.lastAddr = sockaddr
            
            # Grab the IP:port string
            sockaddr = sockaddr[0] + ":" + str(sockaddr[1])
            
            # Announce we are going to connect
            print("Connecting to {} {}".format(node.key,sockaddr))
            
            # Tell CJDNS to begin a UDPInterface connection to the given
            # IP:port, with the given public key and password.
            self.cjdns.UDPInterface_beginConnection(password=node.password,
                publicKey=node.key, address=sockaddr)
            
            # Mark this node as no longer unresponsive
            try: del self.unresponsive[node.key]
            except KeyError: pass
            
            # Don't try any more addresses. Stop after the first.
            break
            
    def doLog(self, message):
        """
        Process a log line from cjdns to see if it indicates that a peer we are
        responsible for is unresponsive.
        
        """
        
        print(message)
        
        # Short-circuit messages that start with the wrong l;etter and can't
        # possibly match.
        if message[0]!='P': return;
        
        # Test plausible messages against the regex
        p = IS_UNRESPONSIVE.match(message)
        
        # If they don't match, ignore them.
        if not p: return
        
        # Otherwise, get the key of the unresponsive node from the regex match
        # group.
        downKey = p.group(1)
        
        # And get the nodfe for that key
        node = self.nodes.get(downKey,None)
        
        if not node:
            # Complain we aren't responsible for that node.
            print("Unknown neighbor {} is down".format(node))
            return
            
        # If we are responsible for it, register it as unresponsive.
        self.unresponsive[downKey] = node
        
        # Go get its address and try reconnecting.
        self.lookup(node)

    def recieve(self, txid):
        """
        Loop forever porcessing messages from the cjdns router. Takes a txid
        pointing to the subscription to such messages.
        
        """
        
        while True:
            # Repeatedly get and process log messages.
            self.doLog(self.cjdns.getMessage(txid)["message"])

# Connect to the router
cjdns = connectWithAdminInfo()

# Start a new monitor on that connection. It will loop forever, checking
# whatever nodes it wants to check.
DynamicEndpointWatcher(cjdns)




