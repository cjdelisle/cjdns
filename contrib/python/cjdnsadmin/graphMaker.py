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
# along with this program.  If not, see <https://www.gnu.org/licenses/>.

def makeGraph():
    from . import adminTools as admin
    import networkx as nx
    from .publicToIp6 import PublicToIp6_convert
    from collections import deque

    cjdns=admin.connect()
    root=admin.whoami(cjdns)
    rootIP=root['IP']

    G=nx.Graph()

    nodes=deque()
    nodes.append(rootIP)
    table = admin.dumpTable(cjdns)

    i = 0
    while True:
        resp = cjdns.NodeStore_getLink(i)
        i += 1
        if resp['error'] == 'not_found':
            break
        link = resp['result']
        parentAddr=admin.parseAddr(link['parent'])
        parentIP=PublicToIp6_convert(parentAddr['publicKey'])
        childAddr=admin.parseAddr(link['child'])
        childIP=PublicToIp6_convert(childAddr['publicKey'])
        G.add_node(parentIP)
        G.add_node(childIP)
        G.add_edge(parentIP, childIP)

    return G
