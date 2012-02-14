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

function Cjdns(url, password)
{
    this.url = url;
    this.password = password;
};

Cjdns.prototype.
var xmlhttp = new XMLHttpRequest();
xmlhttp.open("GET", "/api/?q=d1:q19:NodeStore_dumpTablee", true);
xmlhttp.onreadystatechange = function() {
    if (xmlhttp.readyState==4) {
       var table = bdecode(xmlhttp.responseText).routingTable;

       if (window.location.href.indexOf('#listNodes') != -1) {
           table.sort(function(a, b){ return a.link - b.link; });
           document.body.innerHTML = JSON.stringify(table);
           return;
       }

       routes = parseRoutes(table);
       var tree = buildTree(routes);
       pairs = getPairs(tree);

       doLayout(1);
       //document.body.innerHTML = JSON.stringify(routes) + '<br/><br/><br/><br/><br/><br/><br/>' + JSON.stringify(pairs);
    }
};
xmlhttp.send(null);
