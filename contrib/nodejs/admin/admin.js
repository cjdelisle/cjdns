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
    fs = require('fs'),
    express = require('express'),
    path = require('path'),
    app = express(),
    CJDNS = require('./cjdns.js');

var PATH = path.dirname(process.argv[1]),
    config = JSON.parse(fs.readFileSync(process.env['HOME'] + '/.cjdnsadmin')),
    cjdns = new CJDNS(config.port, config.addr, config.password);

app.use(express.static(PATH + '/www'));
app.use(express.bodyParser());

app.post('/api', function (req, res, next) {
    if (req.body.lorem === 'ipsum') {
        //Kinda handshake
        res.send({hello: 'world!'});
    } else if (req.body.q) {
        sys.log('Sending ' + sys.inspect(req.body));
        cjdns.sendAuth(req.body, function (err, msg) {
            if (err) {
                res.send('502', 'Something happened!');
            }

            res.send(msg);
        });
    } else {
        next();
    }
});

app.get('/map', function (req, res) {
    res.redirect('/map.html');
});

app.get('*', function (req, res) {
    res.send(404, 'Not found!');
});

app.post('*', function (req, res) {
    res.send(404, {error: 'Not found!'});
});

app.listen(8084);