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

// THIS SCRIPT IS ESSENTIALLY A BACKDOOR
// DON'T RUN IT ON ANYTHING YOU CARE ABOUT

// This script binds port 8084 and allows to use admin methods of cjdns

var sys = require('util'),
    http = require('http'),
    express = require('express'),
    path = require('path'),
    app = express();

var PATH = path.dirname(process.argv[1]);

app.use(express.static(PATH + '/www'));
app.use(express.bodyParser());

app.post('/api', function (req, res, next) {
    //Kinda handshake
    if (req.body.lorem === 'ipsum') {
        res.send({hello: 'world!'});
    } else {
        next();
    }
});

app.get('*', function (req, res) {
    res.send(404, 'Not found!');
});

app.post('*', function (req, res) {
    res.send(404, {error: 'Not found!'});
});

app.listen(8084);