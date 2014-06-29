/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

var w = 800;
var h = 800;
var fill = d3.scale.category10()
var DEBUGGING = false;

/** True if we don't want to map nodes with 0 reach because they might not actually be reachable. */
var EXCLUDE_ZERO_NODES = false;

var vis = d3.select("#viz").append("svg").attr("width", w).attr("height", h);

var uniqueNodes;
var pairs;
var thisNode;
var force;

var doRequest = function(query, callback) {
    var url = '/api/?q=' + encodeURIComponent(bencode(query));
    var xmlhttp = new XMLHttpRequest();
    xmlhttp.open("GET", url, true);
    xmlhttp.onreadystatechange = function() {
        if (xmlhttp.readyState==4) {
           callback(bdecode(xmlhttp.responseText));
        }
    };
    xmlhttp.send(null);
};

var getData = function()
{
    var log2 = function(number)
    {
	    var out = 0;
	    while ((number >>= 1) > 0) {
		    out++;
	    }
	    return out;
    };

    // return true if a runs through b
    var runsThrough = function(a, b)
    {
        var mask = log2(b);
        return (a & mask) == (b & mask);
    };

    var buildBranch = function(parsedRoutes, indexOfTip)
    {
        var branch = [];
        branch.push(parsedRoutes[indexOfTip]);
        var path = parsedRoutes[indexOfTip].path;
        for (var i = indexOfTip + 1; i < parsedRoutes.length; i++) {
            if (runsThrough(path, parsedRoutes[i].path)) {
                branch.push(parsedRoutes[i]);
                path = parsedRoutes[i].path;
            }
        }
        return branch;
    }

    var buildTree = function(parsedRoutes)
    {
        var tree = [];
        for (var i = 0; i < parsedRoutes.length; i++) {
            tree.push({
                value: parsedRoutes[i].link,
                branch: buildBranch(parsedRoutes, i)
            });
        }
        return tree;
    };

    var parsePath = function(path) {
        return new Number('0x' + path.replace(/\./g, ''));
    };

    var parseRoutes = function(list)
    {
         var out = [];
         for (var i = 0; i < list.length; i++) {
             if (EXCLUDE_ZERO_NODES && list[i].link == 0) {
                 continue;
             }
             out.push({
                 name: list[i].ip,
                 path: parsePath(list[i].path),
                 link: list[i].link
             });
         }
         out.sort(function(a, b) { return b.path - a.path });
         return out;
    }

    /**
     * @param routes the full list of nodes.
     * @param nodesByIp an empty hash which will be populated with nodes.
     * @return a list of unique nodes.
     */
    var getUniqueNodes = function(routes, nodesByIp)
    {
        var out = [];
        for (var i = 0; i < routes.length; i++) {
            if (!nodesByIp[routes[i].name]) {
                out.push(nodesByIp[routes[i].name] = routes[i]);
            }
        }
        return out;
    };

    var getPairs = function(tree, nodesByIp)
    {
        var out = [];
        var pairs = {};
        for (var i = 0; i < tree.length; i++) {
            var branch = tree[i];
            for (var j = 0; j < branch.branch.length - 1; j++) {
                var nodes = [branch.branch[j], branch.branch[j + 1]];
                if (nodes[0].name == nodes[1].name) {
                    continue;
                }
                nodes.sort(function(a, b) { return a.name.localeCompare(b.name); });
                var p1 = pairs[nodes[0].name];
                if (!p1) {
                    p1 = pairs[nodes[0].name] = {};
                }
                if (!p1[nodes[1].name]) {
                    var link = {
                        source: nodesByIp[nodes[0].name],
                        target: nodesByIp[nodes[1].name],
                        value: new Number(branch.value)
                    };
                    p1[nodes[1].name] = link;
                    out.push(link);
                } else {
                    p1[nodes[1].name].value += new Number(branch.value);
                }
            }
        }
        return out;
    };

    var genNodeList = function(table) {
        var out = '<table border="1">';
        out += '<tr class="theader">'
        out += '<th>IPv6 Address</th>';
        out += '<th>Path</th>';
        out += '<th>Link State</th>';
        out += '</tr>'
        for (var i = 0; i < table.length; i++) {
            if (i % 2 == 0)
              out += '<tr class="trow1">';
            else
              out += '<tr class="trow2">';
            out += '<td>' + table[i].ip + '</td>';
            var p = table[i].path;
            out += '<td>'+p+' (<a id="ping-'+p+'" onclick="window.ping(\''+p+'\')" href="#runcmd">ping</a>)</td>';
            out += '<td>' + table[i].link + '<br/>';
            out += '</tr>'
        }
        return out;
    };

    var getTable = function(firstRun, more, start) {
        if (start && start > 20000) {
            // sanity limit
            return;
        }
        var q = {"q": "NodeStore_dumpTable"};
        if (start) {
            q.start = start;
        }
        doRequest(q, function(responseJSON) {
            var table = responseJSON.routingTable;
            if (more) {
                for (var i = 0; i < more.routingTable.length; i++) {
                    table.push(more.routingTable[i]);
                }
            }
            if (responseJSON.more == 1) {
                getTable(firstRun, responseJSON, ((start) ? start : 0) + 500);
                return;
            }

            table.sort(function(a, b) {
                if (b.link == a.link) {
                    return parsePath(a.path) - parsePath(b.path);
                }
                return b.link - a.link;
            });

            document.getElementById('nodes-list').innerHTML = genNodeList(table);

            var routes = parseRoutes(table);
            var tree = buildTree(routes);
            var nodeByIp = {};
            uniqueNodes = getUniqueNodes(routes, nodeByIp);
            thisNode = uniqueNodes[uniqueNodes.length - 1];
            thisNode.group = 0;
            pairs = getPairs(tree, nodeByIp);

            if (firstRun) {
                doLayout(1);
            } else {
                force.start();
            }
        });
        //setTimeout(function() { getTable(false) }, 10000);
    };
    getTable(1);

    var checkMemory = function() {
        doRequest({"q":"memory"}, function(responseJSON) {
            document.getElementById('memory-bytes').innerText = responseJSON.bytes;
        });
        setTimeout(checkMemory, 4000);
    }
    checkMemory();
};

var doAuthedRequest = function(request, password, callback)
{
    doRequest({"q":"cookie"}, function(cookieJSON) {
        var hashA = SHA256_hash('' + password + cookieJSON.cookie);
        request.cookie = cookieJSON.cookie;
        request.hash = hashA;
        request.aq = request.q;
        request.q = 'auth';
        var reqStr = bencode(request);
        var hashB = SHA256_hash(reqStr);
        request.hash = hashB
        doRequest(request, callback);
    });
};

window.ping = function(node) {
    var cmdInput = document.getElementById('runcmd-cmd');
    cmdInput.value = '{"q":"RouterModule_pingNode", "args":{"path":"' + node + '"}}';
};

var setupSubmitEvent = function() {
    document.removeEventListener('DOMContentLoaded', setupSubmitEvent, false);
    document.getElementById('runcmd-submit').addEventListener('mousedown', function(ev) {
        ev.stopPropagation();
        var cmdInput = document.getElementById('runcmd-cmd').value;
        var cmdPasswd = document.getElementById('runcmd-passwd').value;
        var output = document.getElementById('runcmd-output');
        output.innerText = '';
        var query = null;
        try {
            query = JSON.parse(cmdInput);
        } catch (e) {
            output.innerText = "failed to parse input\n\n" + JSON.stringify(e);
            return;
        }
        doAuthedRequest(query, cmdPasswd, function(response) {
            output.innerText = JSON.stringify(response);
        });
    }, false);
};
document.addEventListener("DOMContentLoaded", setupSubmitEvent, false);



var doLayout = function(step)
{
    if (!step) {
        getData();
        return;
    }

    force = d3.layout.force()
        .charge(-400)
        .linkDistance(100)//function(link){return 100 / (link.value + 1)})
        .nodes(uniqueNodes)
        .links(pairs)
        .size([w, h])
        .start();

    var link = vis.selectAll("line.link")
        .data(pairs)
        .enter()
        .append("line")
        .attr("class", "link")
        .style("stroke-width", function(d) { return Math.sqrt(d.value) / 300000; })
        .attr("x1", function(d) { return d.source.x; })
        .attr("y1", function(d) { return d.source.y; })
        .attr("x2", function(d) { return d.target.x; })
        .attr("y2", function(d) { return d.target.y; });

    var node = vis.selectAll("circle.node")
        .data(uniqueNodes)
        .enter()
        .append("circle")
        .attr("class", "node")
        .attr("cx", function(d) { return d.x; })
        .attr("cy", function(d) { return d.y; })
        .attr("r", function(d) { return Math.max(3, Math.min(Math.sqrt(d.link) * 1/4000, 8)) })
        .style("fill", function(d) { return fill(d.group); })
        .call(force.drag);

    node.append("title").text(function(d) { return d.name; });

    force.on("tick", function() {
        link.attr("x1", function(d) { return d.source.x; })
            .attr("y1", function(d) { return d.source.y; })
            .attr("x2", function(d) { return d.target.x; })
            .attr("y2", function(d) { return d.target.y; });

        node.attr("cx", function(d) { return d.x; })
            .attr("cy", function(d) { return d.y; });
    });
  }
