var w = 960;
var h = 600;
var fill = d3.scale.category20()
var DEBUGGING = false;

/** True if we don't want to map nodes with 0 reach because they might not actually be reachable. */
var EXCLUDE_ZERO_NODES = false;

var vis = d3.select("#viz").append("svg").attr("width", w).attr("height", h);

var json = {
  links: [],
  nodes: []
};

var nodes = {};

var routes;
var pairs;

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

var getData = function() {
    // return true if a runs through b
    var runsThrough = function(a, b)
    {
        var mask = (1 << parseInt(Math.log(b))) - 1;
        return (a & mask) == (b & mask);
    };

    var buildBranch = function(parsedRoutes, indexOfTip)
    {
        var branch = new Array();
        branch.push(parsedRoutes[indexOfTip]);
        var path = parsedRoutes[indexOfTip].path;
        var lastIp = parsedRoutes[indexOfTip].name;
        for (var i = indexOfTip + 1; i < parsedRoutes.length; i++) {
            if (runsThrough(path, parsedRoutes[i].path)) {
                branch.push(parsedRoutes[i]);
                path = parsedRoutes[i].path;
                lastIp = parsedRoutes[i].name;
            }
        }
        return branch;
    }

    var buildTree = function(parsedRoutes)
    {
        var tree = new Array();
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

    var getPairs = function(tree)
    {
        var pairs = {};
        for (var i = 0; i < tree.length; i++) {
            var branch = tree[i];
            for (var j = 0; j < branch.branch.length - 1; j++) {
                var ips = [branch.branch[j].name, branch.branch[j + 1].name];
                if (ips[0] == ips[1]) {
                    continue;
                }
                ips.sort();
                var p1 = pairs[ips[0]];
                if (!p1) {
                    p1 = pairs[ips[0]] = {};
                }
                if (!p1[ips[1]]) {
                    p1[ips[1]] = new Number(branch.value);
                } else {
                    p1[ips[1]] += new Number(branch.value);
                }
            }
        }
        return pairs;
    };

    var genNodeList = function(table) {
        var out = '<table style="border-collapse:collapse; width:800px" border="1">';
        out += '<tr>'
        out += '<th>IPv6 Address</th>';
        out += '<th>Path</th>';
        out += '<th>Link State</th>';
        out += '</tr>'
        for (var i = 0; i < table.length; i++) {
            out += '<tr>'
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
            if (firstRun) {
                routes = parseRoutes(table);
                var tree = buildTree(routes);
                pairs = getPairs(tree);

                doLayout(1);
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
    //checkMemory();
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

    var nodeByIp = {};
    for (var i = 0; i < routes.length; i++) {
        nodeByIp[routes[i].name] = routes[i];
    }

    if (DEBUGGING) {
        var noLinks = {};
    }
    for (var ip in nodeByIp) {
        var node = nodeByIp[ip];
        json.nodes.push(node);
        if (DEBUGGING) {
            noLinks[node] = 1;
        }
        for (var tar in pairs[node.name]) {
            json.links.push({
                source: node,
                target: nodeByIp[tar],
                distance: 200 / (pairs[node.name][tar] + 1)
            });
            if (DEBUGGING) {
                delete noLinks[node];
                delete noLinks[nodeByIp[tar]];
            }
        }
    }

    if (DEBUGGING) {
        for (var nl in noLinks) {
            console.log(nl.name + ' has no links!');
        }
    }

    var force = d3.layout.force()
        .charge(-300)
        .linkDistance(function(link){return link.distance})
        .nodes(json.nodes)
        .links(json.links)
        .size([w, h])
        .start();

    var link = vis.selectAll("line.link")
        .data(json.links)
        .enter()
        .append("line")
        .attr("class", "link")
        .style("stroke-width", function(d) { return Math.sqrt(d.value); })
        .attr("x1", function(d) { return d.source.x; })
        .attr("y1", function(d) { return d.source.y; })
        .attr("x2", function(d) { return d.target.x; })
        .attr("y2", function(d) { return d.target.y; });

    var node = vis.selectAll("circle.node")
        .data(json.nodes)
        .enter()
        .append("circle")
        .attr("class", "node")
        .attr("cx", function(d) { return d.x; })
        .attr("cy", function(d) { return d.y; })
        .attr("r", 5)
        .style("fill", function(d) { return fill(d.group); })
        .call(force.drag);

    node.append("title").text(function(d) {
        return d.name;
    });

    force.on("tick", function() {
        link.attr("x1", function(d) { return d.source.x; })
            .attr("y1", function(d) { return d.source.y; })
            .attr("x2", function(d) { return d.target.x; })
            .attr("y2", function(d) { return d.target.y; });

        node.attr("cx", function(d) { return d.x; })
            .attr("cy", function(d) { return d.y; });
    });
  }
