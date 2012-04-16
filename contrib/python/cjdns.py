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
import sys;
import socket;
import hashlib;
from bencode import *;

BUFFER_SIZE = 1024;

def callfunc(cjdns, funcName, password, args):
    sock = cjdns.socket;
    sock.send('d1:q6:cookiee');
    data = sock.recv(BUFFER_SIZE);
    benc = bdecode(data);
    cookie = benc['cookie'];
    req = {
        'q': 'auth',
        'aq': funcName,
        'hash': hashlib.sha256(password + cookie).hexdigest(),
        'cookie' : cookie,
        'args': args
    };
    reqBenc = bencode(req);
    req['hash'] = hashlib.sha256(reqBenc).hexdigest();
    reqBenc = bencode(req);
    sock.send(reqBenc);
    data = sock.recv(BUFFER_SIZE);
    return bdecode(data);

def cjdns_connect(ipAddr, port, password):
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM);
    sock.connect((ipAddr, port));

    # Make sure it pongs.
    sock.send('d1:q4:pinge');
    data = sock.recv(BUFFER_SIZE);
    if (data != 'd1:q4:ponge'):
        raise Exception("Looks like " + ipAddr + ":" + port + " is to a non-cjdns socket.");

    # Get the functions and make the object
    sock.send('d1:q7:invalide');
    data = sock.recv(BUFFER_SIZE);
    benc = bdecode(data);
    cc = ("class Cjdns:\n"
        + "    def __init__(self, socket):\n"
        + "        self.socket = socket;\n"
        + "    def disconnect(self):\n"
        + "        self.socket.close();\n");
    for func in benc['availableFunctions']:
        funcDict = benc['availableFunctions'][func];
        cc += "    def " + func + "(self";
        for arg in funcDict:
            cc += ", " + arg;
        cc += ("):\n"
             + "        args = {");
        for arg in funcDict:
           cc += "\"" + arg + "\": " + arg + ", ";
        cc += ("};\n"
             + "        return callfunc(self, \"" + func + "\", \"" + password + "\", args);\n");
    exec cc;

    cjdns = Cjdns(sock);

    # Check our password.
    ret = callfunc(cjdns, "ping", password, {});
    if ('error' in ret):
        raise Exception("Connect failed, incorrect admin password?\n" + ret);

    cjdns.functions = "";
    nl = "";
    for func in benc['availableFunctions']:
        funcDict = benc['availableFunctions'][func];
        cjdns.functions += nl + func + "(";
        nl = "\n";
        sep = "";
        for arg in funcDict:
            cjdns.functions += sep;
            sep = ", ";
            argDict = funcDict[arg];
            if (argDict['required'] == 1):
                cjdns.functions += "required ";
            cjdns.functions += argDict['type'] + " " + arg;
        cjdns.functions += ")";

    return cjdns;
