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

def makeGraph():
    import adminTools as admin
    import networkx as nx
    from publicToIp6 import PublicToIp6_convert
    from collections import deque

    cjdns=admin.connect()
    root=admin.whoami(cjdns)
    pstats=admin.peerStats(cjdns,up=True)
    peers=[PublicToIp6_convert(x['publicKey']) for x in pstats]

    G=nx.Graph()    
    G.add_node(root[-4:],ip=root)

    nodes=deque()
    for p in peers: 
        nodes.append(p)
        G.add_node(p[-4:],ip=p)
        G.add_edge(root[-4:],p[-4:])   

    while len(nodes) != 0:
        parentIP=nodes.popleft()
        resp=cjdns.NodeStore_nodeForAddr(parentIP)
        numLinks=0
	if 'result' in resp:
            link=resp['result']
            if 'linkCount' in link: numLinks=int(resp['result']['linkCount'])

        for i in range(0,numLinks):
            resp = cjdns.NodeStore_getLink(parentIP, i)
            childLink=resp['result']
            childIP=childLink['child']
            # Check to see if its one hop away from parent node
            if childLink['isOneHop'] != 1:
                continue
            # If its a new node then we want to follow it
            if not childIP[-4:] in G.nodes():
                G.add_node(childIP[-4:],ip=childIP)
                nodes.append(childIP)
            # If there is not a link between the nodes we should put one there
            if (not childIP[-4:] in G[parentIP[-4:]]):
                G.add_edge(parentIP[-4:],childIP[-4:])

    return G
