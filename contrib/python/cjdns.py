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
import os;
import socket;
import hashlib;
import json;
import threading;
import time;
import Queue;
import random;
import string;
from bencode import *;

BUFFER_SIZE = 69632;
KEEPALIVE_INTERVAL_SECONDS = 2;

def randStr():
    return ''.join(random.choice(string.ascii_uppercase + string.digits) for x in range(10));

def callfunc(cjdns, funcName, password, args):
    txid = randStr();
    sock = cjdns.socket;
    sock.send('d1:q6:cookie4:txid10:'+ txid +'e');
    msg = _getMessage(cjdns, txid);
    cookie = msg['cookie'];
    txid = randStr();
    req = {
        'q': 'auth',
        'aq': funcName,
        'hash': hashlib.sha256(password + cookie).hexdigest(),
        'cookie' : cookie,
        'args': args,
        'txid': txid
    };
    reqBenc = bencode(req);
    req['hash'] = hashlib.sha256(reqBenc).hexdigest();
    reqBenc = bencode(req);
    sock.send(reqBenc);
    return _getMessage(cjdns, txid);


def receiverThread(cjdns):
    timeOfLastSend = time.time();
    timeOfLastRecv = time.time();
    try:
        while True:
            if (timeOfLastSend + KEEPALIVE_INTERVAL_SECONDS < time.time()):
                if (timeOfLastRecv + 10 < time.time()):
                    raise Exception("ping timeout");
                cjdns.socket.send('d1:q18:Admin_asyncEnabled4:txid8:keepalive');
                timeOfLastSend = time.time();

            try:
                data = cjdns.socket.recv(BUFFER_SIZE);
            except (socket.timeout): continue;

            try:
                benc = bdecode(data);
            except (KeyError, ValueError):
                print("error decoding [" + data + "]");
                continue;

            if benc['txid'] == 'keepaliv':
                if benc['asyncEnabled'] == 0:
                    raise Exception("lost session");
                timeOfLastRecv = time.time();
            else:
                #print "putting to queue " + str(benc);
                cjdns.queue.put(benc);

    except KeyboardInterrupt:
        print("interrupted");
        import thread
        thread.interrupt_main();


def _getMessage(cjdns, txid):
    while True:
        if txid in cjdns.messages:
            msg = cjdns.messages[txid];
            del cjdns.messages[txid];
            return msg;
        else:
            #print "getting from queue";
            try:
                # apparently any timeout at all allows the thread to be
                # stopped but none make it unstoppable with ctrl+c
                next = cjdns.queue.get(timeout=100);
            except Queue.Empty: continue;
            if 'txid' in next:
                cjdns.messages[next['txid']] = next;
                #print "adding message [" + str(next) + "]";
            else:
                print "message with no txid: " + str(next);
    

def cjdns_connect(ipAddr, port, password):
    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM);
    sock.connect((ipAddr, port));
    sock.settimeout(2);

    # Make sure it pongs.
    sock.send('d1:q4:pinge');
    data = sock.recv(BUFFER_SIZE);
    if (data != 'd1:q4:ponge'):
        raise Exception("Looks like " + ipAddr + ":" + str(port) + " is to a non-cjdns socket.");

    # Get the functions and make the object
    page = 0;
    availableFunctions = {};
    while True:
        sock.send('d1:q24:Admin_availableFunctions4:argsd4:pagei' + str(page) + 'eee');
        data = sock.recv(BUFFER_SIZE);
        benc = bdecode(data);
        for func in benc['availableFunctions']:
            availableFunctions[func] = benc['availableFunctions'][func];
        if (not 'more' in benc):
            break;
        page = page+1;

    argLists = {};
    cc = ("class Cjdns:\n"
        + "    def __init__(self, socket):\n"
        + "        self.socket = socket;\n"
        + "        self.queue = Queue.Queue();\n"
        + "        self.messages = {};\n"
        + "    def disconnect(self):\n"
        + "        self.socket.close();\n"
        + "    def getMessage(self, txid):\n"
        + "        return _getMessage(self, txid);\n");

    for func in availableFunctions:
        argList = [];
        argLists[func] = argList;
        funcDict = availableFunctions[func];
        cc += "    def " + func + "(self";

        # If the arg is required, put it first,
        # otherwise put it after and use a default
        # value of a type which will be ignored by the core.
        for arg in funcDict:
            if (funcDict[arg]['required'] == 1):
                argList.append(arg);
                cc += ", " + arg;
        for arg in funcDict:
            argDict = funcDict[arg];
            if (argDict['required'] == 0):
                argList.append(arg);
                cc += ", " + arg + "=";
                if (argDict['type'] == 'Int'):
                    cc += "''";
                else:
                    cc += "0";

        cc += ("):\n"
             + "        args = {");
        for arg in argList:
           cc += "\"" + arg + "\": " + arg + ", ";
        cc += ("};\n"
             + "        return callfunc(self, \"" + func + "\", \"" + password + "\", args);\n");
    exec(cc);

    cjdns = Cjdns(sock);

    kat = threading.Thread(target=receiverThread, args=[cjdns]);
    kat.setDaemon(True);
    kat.start();

    # Check our password.
    ret = callfunc(cjdns, "ping", password, {});
    if ('error' in ret):
        raise Exception("Connect failed, incorrect admin password?\n" + str(ret))

    cjdns.functions = "";
    nl = "";
    for func in availableFunctions:
        funcDict = availableFunctions[func];
        cjdns.functions += nl + func + "(";
        nl = "\n";
        sep = "";
        for arg in argLists[func]:
            cjdns.functions += sep;
            sep = ", ";
            argDict = funcDict[arg];
            if (argDict['required'] == 1):
                cjdns.functions += "required ";
            cjdns.functions += argDict['type'] + " " + arg;
        cjdns.functions += ")";

    return cjdns;


def cjdns_connectWithAdminInfo():
    try:
        adminInfo = open(os.getenv("HOME") + '/.cjdnsadmin', 'r');
    except IOError:
        print('Please create a file named .cjdnsadmin in your home directory with');
        print('ip, port, and password of your cjdns engine in json.');
        print('for example:');
        print('{');
        print('    "addr": "127.0.0.1",');
        print('    "port": 11234,');
        print('    "password": "You tell me! (you\'ll find it in your cjdroute.conf)"');
        print('}');
        raise;

    data = json.load(adminInfo);
    adminInfo.close();
    return cjdns_connect(data['addr'], data['port'], data['password']);
