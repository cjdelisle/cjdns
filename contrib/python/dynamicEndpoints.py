#!/usr/bin/env python2
"""
dynamicEndpoints.py: make cjdns reliably connect to remote nodes with dynamic IP
addresses, identified by a DNS name.

Requires a config file with a section for each dynamic-IP node, like this:

    [lhjs0njqtvh1z4p2922bbyp2mksmyzf5lb63kvs3ppy78y1dj130.k]
    hostname: verge.info.tm
    port: 6324
    password: ns6vn00hw0buhrtc4wbk8sv230

The section name (in square brackets) is the public key of the node. Then the
hostname, port, and peering password for the node are given.

By default, this program looks up the current Internet IP of each node defined
in the config file, and add that node at that IP to the local cjdns instance.
Unless the --noWait option is given, or the $nowait environment variable is
true, the program then continues running, waiting for cjdns to log messages
about those peers being unresponsive and updating the peers' Internet IP
addresses as needed.

If cjdns dies while the program is monitoring for messages, the program will
hang indefinitely.

Requires that the $HOME/.cjdnsadmin file be correctly set up. See
cjdnsadminmaker.py if that is not the case.

"""

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
from cjdnsadmin.publicToIp6 import PublicToIp6_convert;
from cjdnsadmin.bencode import *
import sys
import socket, re
import select
import time
import os
import pwd
import grp
import logging
import argparse
import atexit
import ConfigParser

# This holds a regex that matches the message we get from the roiuter when it
# sees an unresponsive peer.
IS_UNRESPONSIVE = re.compile(
    "Pinging unresponsive peer \\[(.*\\.k)\\] lag \\[.*\\]")

# Make sure that it works
assert(IS_UNRESPONSIVE.match("Pinging unresponsive peer " +
    "[6fmmn3qurcjg6d8hplq1rrcsspfhvm1900s13f3p5bv2bb4f4mm0.k] lag [207147]"))

# What file do these messages come from? TODO: don't depend so tightly on the
# other end of the codebase. Use the API to watch peers.
MESSAGE_FILE = "InterfaceController.c"
MESSAGE_LINE = 0 # All lines


class Node(object):
    """
    Represents a remote peer. A remoter peer has:

        - A hostname to repeatedly look up
        - A port to connect to
        - A password to connect with
        - A public key to authenticate the remote peer with
        - A last known Internet IP address.

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

    def __init__(self, cjdns, configuration):
        """
        Set up a new DynamicEndpointWatcher operating on the given CJDNS admin
        connection, using the specified ConfigParser parsed configuration.

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
        # nodes.
        self.sub = self.cjdns.AdminLog_subscribe(MESSAGE_LINE, MESSAGE_FILE,
            'DEBUG')

        # Add nodes from the given ConfigParser parser.
        for section in configuration.sections():
            # Each section is named with a node key, and contains a
            # hostname, port, and password.
            peerHostname = configuration.get(section, "hostname")
            peerPort = configuration.get(section, "port")
            peerPassword = configuration.get(section, "password")

            # Add the node
            self.addNode(peerHostname, peerPort, peerPassword, section)

        if self.sub['error'] == 'none':
            # We successfully subscribed to messages.
            
            # When we die, try to unsubscribe
            atexit.register(self.stop)
            
            # Add all the nodes we're supposed to watch.
            for node in self.nodes.values():
                self.lookup(node)
            logging.info("{} peers added!".format(len(self.nodes)))
        else:
            logging.error(self.sub)

    def run(self):
        """
        Run forever, monitoring the peers we are responsible for.
        """

        logging.info("Watching for unresponsive peers")

        # Watch for any messages from our log message subscription.
        self.recieve(self.sub['txid'])
        
    def stop(self):
        """
        Unsubscribe from the admin log and close the connection to cjdns because
        we are shutting down the program. If we don't do this, cjdns might
        crash. If we do do it, cjdns might still crash.
        """

        # Unsubscribe cleanly
        logging.info("Unsubscribing from stream {}".format(
            self.sub['streamId']))
        unsub =  self.cjdns.AdminLog_unsubscribe(self.sub['streamId'])
        if unsub['error'] != 'none':
            logging.error(unsub['error'])
        
        # Close the connection
        logging.info("Closing admin connection")
        self.cjdns.disconnect()


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


        try:

            # Use AF_INET here to make sure we don't get an IPv6 address and try
            # to connect to it when the cjdns UDPInterface is using only IPv4.
            # TODO: Make cjdns bind its UDPInterface to IPv6 as well as IPv4.
            for info in socket.getaddrinfo(node.host,node.port,
                                           socket.AF_INET,socket.SOCK_DGRAM):

                # For every IP address the node has in DNS, with the port we
                # wanted attached...



                # Save the address we get in a field in the node.
                sockaddr = info[-1]
                node.lastAddr = sockaddr

                # Grab the IP:port string
                sockaddr = sockaddr[0] + ":" + str(sockaddr[1])

                # Announce we are going to connect
                logging.info("Connecting to {} at {}".format(
                    PublicToIp6_convert(node.key), sockaddr))

                # Tell CJDNS to begin a UDPInterface connection to the given
                # IP:port, with the given public key and password. Always use
                # the 0th UDPInterface, which is the default.
                reply = self.cjdns.UDPInterface_beginConnection(
                    password=node.password, publicKey=node.key,
                    address=sockaddr)

                if reply["error"] != "none":
                    # The router didn't like our request. Complain.
                    logging.error(
                        "Router refuses to connect to remote peer. {}".format(
                        reply["error"]))

                    # Maybe try the next address?
                    break

                # Mark this node as no longer unresponsive
                try: del self.unresponsive[node.key]
                except KeyError: pass

                # Don't try any more addresses. Stop after the first.
                return

        except socket.gaierror as e:
            # The lookup failed at the OS level. Did we put in a bad hostname?
            logging.error("Could not resolve DNS name {}: {}".format(
                node.host, e))

        # If we get here, we found no addresses that worked.
        logging.error("No working addresses found for node {}".format(
            PublicToIp6_convert(node.key)))


    def doLog(self, message):
        """
        Process a log line from cjdns to see if it indicates that a peer we are
        responsible for is unresponsive.

        """

        logging.debug(message)

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
            logging.debug("Unmonitored neighbor {} is down".format(
                PublicToIp6_convert(downKey)))
            return

        # Otherwise, announce we are doing our job.
        logging.warning("Monitored neighbor {} is down.".format(
             PublicToIp6_convert(downKey)))

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

def main(argv):
    """
    Run the program.
    """

    # Set up logging. See the logging module docs.
    logging.basicConfig(format="[%(asctime)s] %(levelname)s: %(message)s",
        level=logging.INFO)

    # Parse command-line arguments. Make sure to give our docstring as program
    # help.
    parser = argparse.ArgumentParser(description=__doc__,
        formatter_class=argparse.RawDescriptionHelpFormatter)

    parser.add_argument("configFile", type=argparse.FileType("r"),
        help="configuration file of hosts to read")
    parser.add_argument("--noWait", action="store_true",
        help="look up dynamic peers once and exit")
    parser.add_argument("--adminInfo",
        help="use this file to load the cjdns admin password")

    # Parse all the command-line arguments
    options = parser.parse_args(argv[1:])

    while True:
        try:
            # Connect to the router, using the specified admin info file, if
            # given.
            cjdns = connectWithAdminInfo(path=options.adminInfo)
            break
        except socket.error:
            # Connection probably refused. Retry in a bit
            logging.error("Error connecting to cjdns. Retrying in 1 minute...")
            time.sleep(60)
            
    # Drop root if we have it. We can't do it before we load the admin info
    # file, for the use case where that file is root-only.
    try:

        # Switch group to nogroup
        os.setgid(grp.getgrnam("nogroup").gr_gid)
        # Switch user to nobody
        os.setuid(pwd.getpwnam("nobody").pw_uid)

        # Announce we dropped privs
        logging.info("Dropped privileges: running as {}:{}".format(
            pwd.getpwuid(os.getuid())[0], grp.getgrgid(os.getgid())[0]))
    except OSError:
        # Complain we couldn't drop privs right
        logging.warning("Could not drop privileges: running as {}:{}".format(
            pwd.getpwuid(os.getuid())[0], grp.getgrgid(os.getgid())[0]))



    # Now we can load the config file. It is now required.

    # Maker a new parser to parse the config file
    parsedConfig = ConfigParser.SafeConfigParser()

    # Be case sensitive
    parsedConfig.optionxform = str

    # Read the config from the file
    parsedConfig.readfp(options.configFile)

    # Make a new watcher on the cjdroute connection, with the config from the
    # config file. This automatically looks up all the peers and tries to
    # connect to them once.
    watcher = DynamicEndpointWatcher(cjdns, parsedConfig)

    if options.noWait or os.environ.get('nowait', False):
        # We're not supposed to wait. Quit while we're ahead
        sys.exit(0)
    else:
        # Monitor for unresponsive nodes. This will loop until cjdns restarts,
        # at which point it will throw an exception.
        watcher.run()



if __name__ == "__main__":
    # Run our main method
    sys.exit(main(sys.argv))





