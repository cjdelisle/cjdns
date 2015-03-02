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

import sys
import os
import socket
import errno
import hashlib
import json
import threading
import time
import Queue
import random
import string
from bencode import *

BUFFER_SIZE = 69632
KEEPALIVE_INTERVAL_SECONDS = 2


class Session():
    """Current cjdns admin session"""

    def __init__(self, socket):
        self.socket = socket
        self.queue = Queue.Queue()
        self.messages = {}

    def disconnect(self):
        self.socket.close()

    def getMessage(self, txid):
        # print self, txid
        return _getMessage(self, txid)

    def functions(self):
        print(self._functions)


def _randomString():
    """Random string for message signing"""

    return ''.join(
        random.choice(string.ascii_uppercase + string.digits)
        for x in range(10))


def _callFunc(session, funcName, password, args):
    """Call custom cjdns admin function"""

    txid = _randomString()
    sock = session.socket
    sock.send('d1:q6:cookie4:txid10:' + txid + 'e')
    msg = _getMessage(session, txid)
    cookie = msg['cookie']
    txid = _randomString()
    req = {
        'q': funcName,
        'hash': hashlib.sha256(password + cookie).hexdigest(),
        'cookie': cookie,
        'args': args,
        'txid': txid
    }

    if password:
        req['aq'] = req['q']
        req['q'] = 'auth'
        reqBenc = bencode(req)
        req['hash'] = hashlib.sha256(reqBenc).hexdigest()

    reqBenc = bencode(req)
    sock.send(reqBenc)
    return _getMessage(session, txid)


def _receiverThread(session):
    """Receiving messages from cjdns admin server"""

    timeOfLastSend = time.time()
    timeOfLastRecv = time.time()
    try:
        while True:
            if (timeOfLastSend + KEEPALIVE_INTERVAL_SECONDS < time.time()):
                if (timeOfLastRecv + 10 < time.time()):
                    raise Exception("ping timeout")
                session.socket.send(
                    'd1:q18:Admin_asyncEnabled4:txid8:keepalive')
                timeOfLastSend = time.time()

            # Did we get data from the socket?
            got_data = False

            while True:
                # This can be interrupted and we need to loop it.

                try:
                    data = session.socket.recv(BUFFER_SIZE)
                except (socket.timeout):
                    # Stop retrying, but note we have no data
                    break
                except socket.error as e:
                    if e.errno != errno.EINTR:
                        # Forward errors that aren't being interrupted
                        raise
                    # Otherwise it was interrupted so we try again.
                else:
                    # Don't try again, we got data
                    got_data = True
                    break

            if not got_data:
                # Try asking again.
                continue


            try:
                benc = bdecode(data)
            except (KeyError, ValueError):
                print("error decoding [" + data + "]")
                continue

            if benc['txid'] == 'keepaliv':
                if benc['asyncEnabled'] == 0:
                    raise Exception("lost session")
                timeOfLastRecv = time.time()
            else:
                # print "putting to queue " + str(benc)
                session.queue.put(benc)

    except KeyboardInterrupt:
        print("interrupted")
        import thread
        thread.interrupt_main()
    except Exception as e:
        # Forward along any errors, before killing the thread.
        session.queue.put(e)


def _getMessage(session, txid):
    """Getting message associated with txid"""

    while True:
        if txid in session.messages:
            msg = session.messages[txid]
            del session.messages[txid]
            return msg
        else:
            # print "getting from queue"
            try:
                # apparently any timeout at all allows the thread to be
                # stopped but none make it unstoppable with ctrl+c
                next = session.queue.get(timeout=100)
            except Queue.Empty:
                continue

            if isinstance(next, Exception):
                # If the receiveing thread had an error, throw one here too.
                raise next

            if 'txid' in next:
                session.messages[next['txid']] = next
                # print "adding message [" + str(next) + "]"
            else:
                print "message with no txid: " + str(next)


def _functionFabric(func_name, argList, oargList, password):
    """Function fabric for Session class"""

    def functionHandler(self, *args, **kwargs):
        call_args = {}

        for (key, value) in oargList.items():
            call_args[key] = value

        for i, arg in enumerate(argList):
            if (i < len(args)):
                call_args[arg] = args[i]

        for (key, value) in kwargs.items():
            call_args[key] = value

        return _callFunc(self, func_name, password, call_args)

    functionHandler.__name__ = func_name
    return functionHandler


def connect(ipAddr, port, password):
    """Connect to cjdns admin with this attributes"""

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
    sock.connect((ipAddr, port))
    sock.settimeout(2)

    # Make sure it pongs.
    sock.send('d1:q4:pinge')
    data = sock.recv(BUFFER_SIZE)
    if (not data.endswith('1:q4:ponge')):
        raise Exception(
            "Looks like " + ipAddr + ":" + str(port) +
            " is to a non-cjdns socket.")

    # Get the functions and make the object
    page = 0
    availableFunctions = {}
    while True:
        sock.send(
            'd1:q24:Admin_availableFunctions4:argsd4:pagei' +
            str(page) + 'eee')
        data = sock.recv(BUFFER_SIZE)
        benc = bdecode(data)
        for func in benc['availableFunctions']:
            availableFunctions[func] = benc['availableFunctions'][func]
        if (not 'more' in benc):
            break
        page = page+1

    funcArgs = {}
    funcOargs = {}

    for (i, func) in availableFunctions.items():
        items = func.items()

        # grab all the required args first
        # append all the optional args
        rargList = [arg for arg,atts in items if atts['required']]
        argList = rargList + [arg for arg,atts in items if not atts['required']]

        # for each optional arg setup a default value with
        # a type which will be ignored by the core.
        oargList = {}
        for (arg,atts) in items:
            if not atts['required']:
                oargList[arg] = (
                    "''" if (func[arg]['type'] == 'Int')
                    else "0")

        setattr(Session, i, _functionFabric(
            i, argList, oargList, password))

        funcArgs[i] = rargList
        funcOargs[i] = oargList

    session = Session(sock)

    kat = threading.Thread(target=_receiverThread, args=[session])
    kat.setDaemon(True)
    kat.start()

    # Check our password.
    ret = _callFunc(session, "ping", password, {})
    if ('error' in ret):
        raise Exception(
            "Connect failed, incorrect admin password?\n" + str(ret))

    session._functions = ""

    funcOargs_c = {}
    for func in funcOargs:
        funcOargs_c[func] = list(
            [key + "=" + str(value)
                for (key, value) in funcOargs[func].items()])

    for func in availableFunctions:
        session._functions += (
            func + "(" + ', '.join(funcArgs[func] + funcOargs_c[func]) + ")\n")

    # print session.functions
    return session


def connectWithAdminInfo(path = None):
    """Connect to cjdns admin with data from user file"""

    if path is None:
        path = os.path.expanduser('~/.cjdnsadmin')
    try:
        with open(path, 'r') as adminInfo:
            data = json.load(adminInfo)
    except IOError:
        sys.stderr.write("""Please create a file named .cjdnsadmin in your
home directory with
ip, port, and password of your cjdns engine in json.
for example:
{
    "addr": "127.0.0.1",
    "port": 11234,
    "password": "You tell me! (Search in ~/cjdroute.conf)"
}
""")
        raise

    return connect(data['addr'], data['port'], data['password'])
