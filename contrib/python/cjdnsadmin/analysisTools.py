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

def getNodes(cjdns,G,ip,parentRoute):
    import networkx as nx
    #ip becomes the parent node
    resp=cjdns.NodeStore_getNode(ip)
    parentNode=resp['result']
    numLinks=int(parentNode['linkCount'])

    route=''
    ##Dont add anymore nodes from this branch if label is too long
    if (parentRoute != 'ffff.ffff.ffff.ffff' and len(parentRoute) == 19):
        route = cjdns.NodeStore_getRouteLabel(parentRoute, ip);
        if route['error'] == 'none':
            route = route['result']
            nodes=[]
            #loop over number of the parent node's links 
            for i in range(0,numLinks):
                resp = cjdns.NodeStore_getLink(ip, i)
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
                if (not (childIP[-4:],ip[-4:]) in G.edges()) and (not (ip[-4:],childIP[-4:]) in G.edges()):
                    G.add_edge(ip[-4:],childIP[-4:])

            #Follow newly found nodes
            for n in nodes:
                G=getNodes(cjdns,G,n,route)
    return G

def makeGraph():
    import adminTools as admin
    import networkx as nx
    from publicToIp6 import PublicToIp6_convert

    cjdns=admin.connect()
    root=cjdns.NodeStore_getNode(0)
    root=PublicToIp6_convert(root['result']['key'])
    pstats=admin.peerStats(cjdns,up=True)
    peers=[PublicToIp6_convert(x['publicKey']) for x in pstats]
    G=nx.Graph()
    G.add_node(root[-4:],ip=root)
    for ip in peers:
        G.add_node(ip[-4:],ip=ip)
        G.add_edge(root[-4:],ip[-4:])
    for ip in peers:
        G=getNodes(cjdns,G,ip,'0000.0000.0000.0001')
    return G
