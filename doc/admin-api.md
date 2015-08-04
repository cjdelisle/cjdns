#Cjdns Admin API

Cjdns is inspected and configured through a UDP socket.
When cjdroute starts up, it reads the configuration file and spawns cjdns core. The core
knows nothing but the port which it should bind to and the private key which it should use.
All other information such as peers, interfaces and passwords is given to the core through the
admin UDP interface. When cjdroute is finished setting up the core, it exits leaving the core
running in the background.

You can call all of the functions which are called by cjdroute to collect information and alter
the core's configuration.

## How a function works
To call a function you send a udp packet containing a bencoded request to the core and it sends
back a bencoded response.

    echo -n 'd1:q4:pinge' | nc6 -u -t 1 -n -w3 127.0.0.1 11234

If you are more comfortable writing json then benc, you can use benc2json in reverse mode to
preprocess your message. **Note**: benc2json has been removed in 2821c81d49 to speed up the build.

    echo '{ "q": "ping" }' | ./build/benc2json -r

Stream the request from json into benc and then make the request to the core:

    echo '{ "q": "ping" }' \
        | ./build/benc2json -r \
        | tr -d '\n' \
        | nc6 -u -t 1 -n -w3 127.0.0.1 11234

Get the result back into json:

    echo '{ "q": "ping" }' \
        | ./build/benc2json -r \
        | tr -d '\n' \
        | nc6 -u -t 1 -n -w3 127.0.0.1 11234 \
        | ./build/benc2json


## Transaction ID
Because you can send multiple messages at once, you may add a transaction ID to a message and it
will be reflected back to you in the response.

    echo '{ "q": "ping", "txid": "my request" }' \
        | ./build/benc2json -r \
        | tr -d '\n' \
        | nc6 -u -t 1 -n -w3 127.0.0.1 11234 \
        | ./build/benc2json

Result:

    {
      "txid" : "my request",
      "q" : "pong"
    }


## Arguments
Some functions require arguments and others allow arguments but assume defaults if they are
not provided. Arguments are sent to a function through a benc *dictionary* called `args`.

The `Admin_availableFunctions()` function has an optional argument called `page`, this is
because there are too many functions to be described in a single UDP packet. The following
command will get the first page of functions from `Admin_availableFunctions` which will
describe other functions and their required and allowed arguments.

    echo -n '
    {
        "q": "Admin_availableFunctions",
        "args": {
            "page": 0
        }
    }' | ./build/benc2json -r \
        | tr -d '\n' \
        | nc6 -u -t 1 -n -w3 127.0.0.1 11234 \
        | ./build/benc2json


## Authentication
Any function which changes the state of cjdns core requires authentication to carry out.
Authentication is done on a per-request basis. Functions which don't require authentication
can still be called with authentication and will still fail if the authentication is incorrect.

* Step 1: Request a cookie from the server.

* Step 2: Calculate the SHA-256 of the cookie and your admin password, place this hash and cookie
in the request.

* Step 3: Calculate the SHA-256 of the entire request with the hash and cookie added,
replace the hash in the request with this result.

Steps 1 and 2 securely bind the cookie to the password so that the password hash cannot
be taken and used again in another request later on, step 3 binds the cookie and password to
the request so that a man-in-the-middle cannot change the content of the request in flight.


### Anatomy of an authenticated request

A plain request such as `{"q": "ping"}` becomes `{"q":"auth", "aq":"ping", "hash":<calculated hash>}`.
The `q` field is moved to `aq` (authenticated query) and the `q` field says `auth`.

**NOTE:** A cookie is only valid for 10 seconds so requesting and using a cookie must be done
in the same script.

**NOTE2:** Cookies are reusable *for now* this is not part of the API and is considered a
bug, you should always request a new cookie for each authenticated request otherwise you may
be broke by changes in the future.

### By example

**Step 1:** Get the cookie

    RESP=`echo -n 'd1:q6:cookiee' | nc6 -u -t 1 -n -w3 127.0.0.1 11234` \
    echo response=${RESP}; \
    COOKIE=`echo ${RESP} | sed 's/d6:cookie10:\([0-9]*\)e/\1/'` \
    echo cookie=${COOKIE};

**Step 2:** Calculate the hash of the cookie and password:
For this step, you will need the admin password from your cjdroute.conf file, it's to be found
inside of the block which says `"admin": {`.

    ADMIN_PASS=you_will_find_this_in_your_cjdroute_dot_conf \
    REQUEST='{"q": "auth", "aq": "ping", "hash": "__HASH__", "cookie": "__COOKIE__"}' \
    COOKIE_RESP=`echo -n 'd1:q6:cookiee' | nc6 -u -t 1 -n -w3 127.0.0.1 11234` \
    COOKIE=`echo ${COOKIE_RESP} | sed 's/d6:cookie10:\([0-9]*\)e/\1/'` \
    HASH_ONE=`echo -n "${ADMIN_PASS}${COOKIE}" | sha256sum -b | cut -d\  -f1` ; \
    REQ_ONE=`echo $REQUEST | sed -e "s/__HASH__/${HASH_ONE}/" -e "s/__COOKIE__/${COOKIE}/" \
            | ./build/benc2json -r | tr -d '\n'` ; \
    echo "hash of password and cookie is ${HASH_ONE}" ; \
    echo "Request with cookie and hash added:" ; \
    echo "${REQ_ONE}" ; \
    echo "JSON version of request:" ; \
    echo "${REQ_ONE}" | ./build/benc2json

**Step 3:** Calculate the SHA-256 of the entire request and replace the one in the request:
This will calculate the final request and send it to cjdns.

    ADMIN_PASS=you_will_find_this_in_your_cjdroute_dot_conf \
    REQUEST='{"q": "auth", "aq": "ping", "hash": "__HASH__", "cookie": "__COOKIE__"}' \
    COOKIE_RESP=`echo -n 'd1:q6:cookiee' | nc6 -u -t 1 -n -w3 127.0.0.1 11234` \
    COOKIE=`echo ${COOKIE_RESP} | sed 's/d6:cookie10:\([0-9]*\)e/\1/'` \
    HASH_ONE=`echo -n "${ADMIN_PASS}${COOKIE}" | sha256sum -b | cut -d\  -f1` \
    REQ_ONE=`echo $REQUEST | sed -e "s/__HASH__/${HASH_ONE}/" -e "s/__COOKIE__/${COOKIE}/" \
        | ./build/benc2json -r | tr -d '\n'` \
    FINAL_HASH=`echo -n "$REQ_ONE" | sha256sum -b | cut -d\  -f1` \
    FINAL_REQ=`echo $REQ_ONE | sed -e "s/${HASH_ONE}/${FINAL_HASH}/"` ; \
    echo -n "$FINAL_REQ" \
        | nc6 -u -t 1 -n -w3 127.0.0.1 11234 \
        | ./build/benc2json


If you see this:

    {
      "q" : "pong"
    }

then it has succeeded, if the password is incorrect, you will see this:

    {
      "error" : "Auth failed."
    }

### Tools:

Obviously using bash to craft cjdns admin RPC calls is probably the most awkward way possible,
there are tools in cjdns/contrib which will help you craft requests, specifically there are
libraries written in python and perl which will allow users to call cjdns internal functions
as python/perl native functions. A tool called `cexec` is provided with the python library which
allows you to call cjdns functions from shell scripts or the command line as follows:

    ./contrib/python/cexec 'ping()'


## Cjdns Functions:

    user@ubnta8:~/wrk/cjdns$ ./contrib/python/cexec 'functions()' | sort
    Admin_asyncEnabled()
    Admin_availableFunctions(page='')
    Allocator_bytesAllocated()
    Allocator_snapshot(includeAllocations='')
    AuthorizedPasswords_add(password, user, authType='', ipv6=0)
    AuthorizedPasswords_list()
    AuthorizedPasswords_remove(user)
    Core_exit()
    Core_initTunnel(desiredTunName=0)
    Core_pid()
    ETHInterface_beacon(interfaceNumber='', state='')
    ETHInterface_beginConnection(publicKey, macAddress, interfaceNumber='', password=0)
    ETHInterface_new(bindDevice)
    InterfaceController_disconnectPeer(pubkey)
    InterfaceController_peerStats(page='')
    IpTunnel_allowConnection(publicKeyOfAuthorizedNode, ip6Address=0, ip4Address=0)
    IpTunnel_connectTo(publicKeyOfNodeToConnectTo)
    IpTunnel_listConnections()
    IpTunnel_removeConnection(connection)
    IpTunnel_showConnection(connection)
    memory()
    NodeStore_dumpTable(page)
    NodeStore_getLink(parent, linkNum)
    NodeStore_getRouteLabel(pathParentToChild, pathToParent)
    NodeStore_nodeForAddr(ip=0)
    ping()
    RainflyClient_addKey(ident)
    RainflyClient_addServer(addr)
    RainflyClient_minSignatures(count)
    RouterModule_findNode(nodeToQuery, target, timeout='')
    RouterModule_getPeers(path, nearbyPath=0, timeout='')
    RouterModule_lookup(address)
    RouterModule_pingNode(path, timeout='')
    SearchRunner_showActiveSearch(number)
    Security_checkPermissions()
    Security_dropPermissions()
    Security_setUser(user)
    SessionManager_getHandles(page='')
    SessionManager_sessionStats(handle)
    SwitchPinger_ping(path, data=0, keyPing='', timeout='')
    UDPInterface_beginConnection(publicKey, address, interfaceNumber='', password=0)
    UDPInterface_new(bindAddress=0)


###RouterModule_pingNode()

**Auth Required**

Send a node a cjdns ping request.

Parameters:

* required String **path** may be a route such as "0000.0000.0000.1d53" or an ip address such as
"fc5d:baa5:61fc:6ffd:9554:67f0:e290:7536", or an ip with explicit path
eg: "fc5d:baa5:61fc:6ffd:9554:67f0:e290:7536@0000.0000.0000.1d53"

* Int **timeout** (optional) the number of milliseconds after which to timeout the ping
if there is no response. Defaults to router's adaptive ping timeout if unspecified.

Responses:

* **error**: `could not find node to ping` if there was no node by the given address found in the
routing table

* **result**: `timeout` gives timeout and number of milliseconds since the ping.

* **result**: `pong` gives `version` representing the git hash of the source code which built the
pinged node, and `ms` which is the number of milliseconds since the original ping.

Examples:

    >>> cjdns.RouterModule_pingNode('fc38:4c2c:1a8f:3981:f2e7:c2b9:6870:6e84')
    {'version': '5c5e84ccdba3f31f7c88077729700b4368320bc2', 'result': 'pong', 'ms': 79}

    >>> cjdns.RouterModule_pingNode('fc5d:baa5:61fc:6ffd:9554:67f0:e290:7536')
    {'error': 'could not find node to ping'}

    >>> cjdns.RouterModule_pingNode('0000.0000.0000.0013')
    {'version': '2b62b9ae911f1044e45f3f28fdd63d0d5a7fc512', 'result': 'pong', 'ms': 0}

    >>> cjdns.RouterModule_pingNode('a')
    {'error': "Unexpected length, must be either 39 char ipv6 address (with leading zeros)
    eg: 'fc4f:000d:e499:8f5b:c49f:6e6b:01ae:3120' or 19 char path eg: '0123.4567.89ab.cdef'"}

    >>> cjdns.RouterModule_pingNode('aaaaaaaaaaaaaaaaaaa')
    {'error': 'parse path failed'}

    >>> cjdns.RouterModule_pingNode('aaaaaaaaaaaaaaaaaaazzzzzzzzzzzzzzzzzzzz')
    {'error': 'parsing address failed'}

    >>> cjdns.RouterModule_pingNode('fc38:4c2c:1a8f:3981:f2e7:c2b9:6870:6e84', 10)
    {'result': 'timeout', 'ms': 10}

### ETHInterface Functions:

ETHInterface is a connector which allows cjdns nodes on the same lan to automatically connect
without the need to IP addresses on the LAN or sharing of connection credentials. It works on
wireless LANs as well as wired ethernet LANs.

#### ETHInterface_new()

Create a new ETHInterface and bind it to a device.
**NOTE**: this call will always fail with `'error': 'call to socket() failed. [permission denied]'`
unless it is running as root and will fail with `process cannot open more files` if
`Security_setUser()` has already been called.

**Auth Required**

Parameters:

* required String **bindDevice** the name of the ethernet device to bind to, eg: `eth0` or `wlan0`.

Returns:

* Int **interfaceNumber** an number which can be used to carry out other operations on the interface later.


#### ETHInterface_beginConnection()

Connect an ETHInterface to another computer which has an ETHInterface running.

**Auth Required**

Parameters:

* required String **publicKey** The public key of the other node, similar to `UDPInterface_beginConnection()`
* required String **macAddress** The mac address of the other node.
* Int **interfaceNumber** The interface number to use, assumed 0 (first ETHinterface created) if not supplied.
* String **password** A password for connecting to the other node if required.

Returns:

* String **error**: `none` if everything went well.

Other errors are self-explanitory.


#### ETHInterface_beacon()

Enable or disable sending or receiving of ETHInterface beacon messages.
ETHInterface uses periodic beacon messages to automatically peer nodes which are on the same LAN.
Be mindful that if your lan has is open wifi, enabling beaconing will allow anyone to peer with you.

**Auth Required**

Beacon States:
0. Disabled, no beacons are sent and incoming beacon messages are discarded.
1. Accepting, no beacons are sent but if an incoming beacon is received, it is acted upon.
2. Sending and Accepting, beacons are sent and accepted.

Parameters:

* Int **interfaceNumber** The number of the ETHInterface to change the state of, assumed 0 if not provided.
* Int **state** What state to switch to, if not provided, the current state will be queried only.

Returns:

* String **error**: `none` if all went well.
* Int **state**: the state number after the call is complete.
* String **stateName**: a description of the state.


Example:

    $ ./contrib/python/cexec 'ETHInterface_beacon(2)'
    {'txid': 'FYRKHAPIM3', 'error': 'invalid interfaceNumber'}

    $ ./contrib/python/cexec 'ETHInterface_beacon(0)'
    {'txid': 'Z7KHE7SZ5R', 'state': 2, 'stateName': 'sending and accepting', 'error': 'none'}

    $ ./contrib/python/cexec 'ETHInterface_beacon(0, 0)'
    {'txid': 'TP1R8PYCNS', 'state': 0, 'stateName': 'disabled', 'error': 'none'}

    $ ./contrib/python/cexec 'ETHInterface_beacon(0, 1)'
    {'txid': 'UGKKGX4ZC9', 'state': 1, 'stateName': 'accepting', 'error': 'none'}

    $ ./contrib/python/cexec 'ETHInterface_beacon(0, 2)'
    {'txid': '1B7RXJEH3N', 'state': 2, 'stateName': 'sending and accepting', 'error': 'none'}


### IpTunnel Functions

IPTunnel is designed to allow tunneling of IPv4 and IPv6 packets through a cjdns network
to the external internet or to a virtual LAN. It provides familiar VPN type functionality.
There are 2 nodes, a client and a server, the server uses `IPTunnel_allowConnection()` and the
client uses `IPTunnel_connectTo()` the server assigns IPv4 and/or IPv6 addresses to the client
and the client is required to use only these addresses, subnet assignment is not supported.
When the client uses `IPTunnel_connectTo()`, it sends a request to the server for addresses and
continues polling the server periodically until the addresses are provided.

#### IpTunnel_listConnections()

List the connection numbers of all IPTunnel connections.

**Auth Required**

Returns:

* List **connections**: A list of integers representing the connection numbers for each connection.
* String **error**: `none`


Example:

    $ ./contrib/python/cexec 'IpTunnel_listConnections()'
    {'connections': [0], 'txid': '5ZFPFJ60AT', 'error': 'none'}


#### IpTunnel_showConnection()

Show information about a perticular IPTunnel connection.

**Auth Required**

Parameters:

* required Int **connection**: the connection number for the connection to show information about.

Returns:

* Int **outgoing**: 1 if the connection is outgoing, 0 if it's incoming.
* String **key**: the cjdns public key of the foreign node.
* String **ip6Address**: the IPv6 address which is assigned to this IPTunnel if applicable.
* Int **ip6Prefix**: the IPv6 netmask/prefix length which is assigned to this IPTunnel if applicable.
* String **ip4Address**: the IPv4 address which is assigned to this IPTunnel if applicable.
* Int **ip4Prefix**: the IPv4 netmask/prefix length which is assigned to this IPTunnel if applicable.
* String **error**: `none` unless the connection number is invalid.

Examples:

    # Prior to getting it's addresses from the server, they are not listed.
    $ ./contrib/python/cexec 'IpTunnel_showConnection(0)'
    {'outgoing': 1, 'txid': 'REIV40SXD9', 'key': 'd5d0wu0usrkufd8s98t19gt7m2ggvbz1xbnuxu82x63uqlnk2kb0.k', 'error': 'none'}

    # After a short wait, the addresses are provided and they are now listed.
    $ ./contrib/python/cexec 'IpTunnel_showConnection(0)'
    {'outgoing': 1, 'txid': 'CAQCTWECRD', 'ip4Address': '192.168.10.2', 'key': 'd5d0wu0usrkufd8s98t19gt7m2ggvbz1xbnuxu82x63uqlnk2kb0.k', 'error': 'none', 'ip6Address': '2a02:2498:e000:20::144:3'}


#### IpTunnel_removeConnection()

Remove an IPTunnel connection from the list, the other end will nolonger be able to send traffic
over this connection.

**Auth Required**

**NOT IMPLEMENTED**


#### IpTunnel_connectTo()

Initiate an *outgoing* connection to another node and request IP addresses from them.

**Auth Required**

Parameters:

* required String **publicKeyOfNodeToConnectTo** the pubkey of the node to connect to.

Returns:

* String **error**: `none` if all went well
* Int **connection**: the connection number of the new connection

Examples:

    $ ./contrib/python/cexec 'IpTunnel_connectTo("d5d0wu0usrkufd8s98t19gt7m2ggvbz1xbnuxu82x63uqlnk2kb0.k")'
    {'connection': 1, 'txid': '9QXRQO1FG8', 'error': 'none'}


#### IpTunnel_allowConnection()

Allow in *incoming* connection from another node, they must also use `IPTunnel_connectTo()` in order
to complete the connection.

**Auth Required**

Parameters:
* required String **publicKeyOfAuthorizedNode** The key of the node which is authorized to connect.
* String **ip6Address** The IPv6 address to give them if applicable.
* String **ip4Address** The IPv4 address to give them if applicable.

Returns:

* String **error** `none` if all went well.
* Int **connection** the connection number for the new connection.



### UDPInterface Functions

UDPInterface is the basic cjdns interface which is used to link distant nodes over the internet.
It will work on a LAN as long as the nodes have IP addresses but for linking on a LAN, ETHInterface
is easier.

#### UDPInterface_new()

Create a new UDPInterface which is either bound to an address/port or not.

**NOTE**: This call will fail with `'error': 'call to socket() failed [process cannot open more files]'`
is `Security_noFiles()` has already been called.

Parameters:

* String **bindAddress**: the address/port to bind to, if unspecified, it is assumed to be `0.0.0.0`.

Returns:

* String **error** `none` if all went well
* Int **interfaceNumber** the number of the interface, usable with `UDPInterface_beginConnection()`


#### UDPInterface_beginConnection()

Start a direct connection to another node.

**Auth Required**

Parameters:

* required String **publicKey** the base32 public key for the node to connect to, ending in .k.
* required String **address** the ip address and port for the node, at this time DNS resolution
and IPv6 is not supported.
* Int **interfaceNumber** the number for the UDPInterface to use for connecting, provided by
*UDPInterface_new()* if not sent, 0 is assumed.
* String **password** a password to use when connecting.

Note: just because it returns `'error': 'none'` does not mean that the connection was successful.
The neighbor may still reject our connection attempts.

Example:

    >>> cjdns.UDPInterface_beginConnection("v0zyvrjuc4xbzh4n9c4k3qpx7kg8xgndv2k45j9nfgb373m8sss0.k", "192.168.0.2:10000", "null")
    {'error': 'none'}

    >>> cjdns.UDPInterface_beginConnection("v0zyvrjuc4xbzh4n9c4k3qpx7kg8xgndv2k45j9nfgb373m8sss0.k", "x", "null")
    {'error': 'unable to parse ip address and port.'}

    >>> cjdns.UDPInterface_beginConnection("k", "x", "null")
    {'error': 'publicKey is too short, must be 52 characters long.'}

    >>> cjdns.UDPInterface_beginConnection("------------------------------------------------------", "x", "null")
    {'error': 'failed to parse publicKey.'}

    >>> cjdns.UDPInterface_beginConnection("zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz0.k", "192.168.0.2:10000", "null")
    {'error': 'invalid cjdns public key.'}

    >>> cjdns.UDPInterface_beginConnection("v0zyvrjuc4xbzh4n9c4k3qpx7kg8xgndv2k45j9nfgb373m8sss0.k", "[1234::5]:10000", "null")
    {'error': 'different address type than this socket is bound to.'}


### AdminLog Functions:

Since cjdns contains so many logging locations, logging to a file would not only be inefficient
but it would fill up your disk rather quickly. Because of this, cjdns logging is only enabled on
request, with these functions you can ask for logs to be enabled on a log level, per-file or even
per-line basis.

Log levels may be excluded at compile time in which case they will not be available.
Each log level implies inclusion of every higher level, if you subscribe to **INFO** logging, you
will also automatically get **WARN**, **ERROR**, and **CRITICAL**.

Cjdns log levels:

* **KEYS** Not compiled in by default, contains private keys and other secret information.
* **DEBUG** Default level, contains lots of information which is probably not useful unless you are
diagnosing an ongoing problem.
* **INFO** Shows starting and stopping of various components and general purpose information.
* **WARN** Generally this means some system has undergone a minor failure, this includes failures
due to network disturbance.
* **ERROR** This means there was a (possibly temporary) failure of a system within cjdns.
* **CRITICAL** This means something is broken such that the cjdns core will likely
have to exit immedietly.


To see an implementation of cjdns log consumer, look at `contrib/python/cjdnslog`.


#### AdminLog_subscribe()

Subscribe to logging of a level/file/line.

**Auth Required**

**NOTE**: Because this function responds asynchronously, using `netcat` or `cexec` to call it
will not work, additionally it will stop sending asynchronous messages unless an incoming message
comes in every 10 seconds so you must send periodic messages on the same UDP port.
See: `Admin_asyncEnabled()` for more information.

Parameters:

* Int **line**: If specified, the logging will be constrained to the log message which appers on
the given line number in the source file.
* String **file**: If specified, the logging will be constrained to the named file, names are not
fully qualified, use "CryptoAuth.c", not "/path/to/CryptoAuth.c".
* String **level**: If specified, the logging will be constrained to log lines which are of the
given level or higher.

Returns:

* String **error**: `none` if all goes well.
* String **streamId**: an opaque string which will be contained in each log message.

Log message structure:

* String **file** the name of the file where the log message came from, eg: "CryptoAuth.c".
* String **level** the log level, one of `["KEYS", "DEBUG", "INFO", "WARN", "ERROR", "CRITICAL"]`
* Int **line** the line number of the line where the log function was called.
* String **message** the log message
* String **streamId** the streamId for the logging subscription.
* Int **time** the time in seconds since the unix epoch when the log message was created.
* String **txid** the same transaction which was used in the call to `AdminLog_subscribe()`.


#### AdminLog_unsubscribe()

Unsubscribe from logging.

**Auth Required**

Parameters:

* required String **streamId**: The id returned in the call to `AdminLog_subscribe()`.

Returns:

* String **error**: `none` if the subscription existed and was removed.

**Note**: If the subscription has already timed out, removing it will yield `'error': 'No such subscription.'`.

Example:

    $ ./contrib/python/cexec 'AdminLog_subscribe()'
    {'txid': '0EKWEP7VXI', 'streamId': 'f1a0e225183397f4', 'error': 'none'}

    $ ./contrib/python/cexec 'AdminLog_unsubscribe("f1a0e225183397f4")'
    {'txid': 'CB4V7KLYCC', 'error': 'none'}


### Admin Functions

These functions are for dealing with the Admin interface, the infrastructure which allows all
of the other functions throughout cjdns to be accessed from the admin socket.

#### Admin_availableFunctions()

Get a list of functions which are available to the admin socket as well as their required and
optional parameters, unfortunately their return values are not provided and can only be determined
by experimentation or by reading the source.

**Note**: The list of functions is paged to make sure each message fits inside of a UDP packet, in
order to get the whole list of functions, you must increment the `page` parameter until the result
nolonger contains the `more` field.

Parameters:

* Int **page**: the page of functions to request, if unspecified it will be assumed to be 0.

Returns:

* Dict **availableFunctions**: a map of function descriptions by function name.
* Int **more**: only present if there are more pages.

##### Function Description:

Each function description is a Dict of function parameters with the parameter name as the key and
the specifications as the value. The specification `required` is an Int which is either 0 meaning
the parameter is optional or 1 meaning it is required. `type` is a String which is one of
`["Int", "String", "Dict", "List"]` and defines the type which the parameter must be.

    'AdminLog_subscribe': {
        'line': {
            'required': 0,
            'type': 'Int'
        },
        'file': {
            'required': 0,
            'type': 'String'
        },
        'level': {
            'required': 0,
            'type': 'String'
        }
    }


#### Admin_asyncEnabled()

This function is for determining whether asynchronous communication is allowed.
Asynchronous communication, EG: AdminLog responses, is only allowed with clients which satisfy
certain requirements.

1. They must send an authenticated request, in the case of AdminLog this is no worry because
`AdminLog_subscribe()` requires authentication.
2. They must have sent something in the past 10 seconds, because of the statelessness of UDP,
there is no way to tell a client which is listening quietly from one which has wandered off so
in order to remain enabled, it must periodically ping (or periodically call `Admin_asyncEnabled()`).
These calls do not need to be authenticated, there just needs to have been one in history.

Returns:

* Int **asyncEnabled**: 1 if asynchronous communication is allowed for this session, 0 otherwise.

Example:
This example illustrates how using `cexec` to call it returns true because `cexec` uses
authenticated calls whereas manually calling it without authentication returns false.

    $ ./contrib/python/cexec 'Admin_asyncEnabled()'
    {'asyncEnabled': 1, 'txid': '74GF0SS2N0'}

    echo '{ "q": "Admin_asyncEnabled" }' \
        | ./build/benc2json -r \
        | tr -d '\n' \
        | nc -u 127.0.0.1 11234 \
        | ./build/benc2json

    {
      "asyncEnabled" : 0
    }


### Security Functions

These functions are available for putting the cjdns core into a sandbox where
a security breach within the core would be less likely to cause a total system compromize.


#### Security_setUser()

Set the user ID which cjdns is running under to a different user. This function allows cjdns
to shed privileges after starting up.

**NOTE**: This function will always fail with an error about `process cannot open more files` if
`Security_noFiles()` has already been called.

Parameters:

* required String **user**: the name of the user to change to.

Return:

* String **error**: `none` if all went well, otherwise a description of the failure.


#### Security_noFiles()

Set the hard open file limit to zero, while this does not force closed file descriptors which are
already open, it makes any function requiring the opening of a file to fail providing a powerful
sandbox. By calling this function after cjdns is started, one can insure that cjdns core cannot
touch the filesystem or open network sockets which it does not already have open. This will however
prevent a number of other admin API functions fron working.

Returns:

* String **error**: `none`

Examples:

    $ ./contrib/python/cexec 'UDPInterface_new("[::]:2048")'
    {'interfaceNumber': 3, 'txid': 'NQGOZXJZIC', 'error': 'none'}

    $ ./contrib/python/cexec 'Security_noFiles()'
    {'txid': 'CQYQWA5SZY', 'error': 'none'}

    $ ./contrib/python/cexec 'UDPInterface_new("[::]:5000")'
    {'txid': 'UZH9LIUOG0', 'cause': 'process cannot open more files', 'error': 'call to socket() failed [process cannot open more files]'}



### Core_initTunnel()

This function is used during cjdns startup to initialize the TUN device, set it's IP address
and set the MTU, it is hastily designed and may be removed in the future.

Parameters:

* String **desiredTunName**: the name of the TUN device to use, if unspecified it will ask the
kernel for a new device.

Returns:

* String **error**: `none` if all went well, otherwise the error which occured.

**Note**: an error will be returned if anything goes wrong initializing the tunnel, setting it's
IP address or setting it's MTU, even if there is an error, the tunnel may work just fine and
even if the tunnel doesn't work, cjdns will function as a router only without the TUN device.


### Core_exit()

A function to stop cjdns.

Returns:

* String **error**: `none` before exiting.



### ping()

Returns:

    {'q':'pong'}

For checking if the admin connection is functioning.


### RouterModule_lookup()

**Auth Required**

Parameters:

* String **address** a 39 character (zero padded) ipv6 address.

Returns:

* A route if one is found in the routing table.
* An address and route of the node which should be handed the packet,
if a route is not found in the local table.
* An error if the address is not parsable.

Examples:

    >>> print cjdns.RouterModule_lookup('fc5d:baa5:61fc:6ffd:9554:67f0:e290:7535')
    {'result': '0000.0000.0000.1953', 'error': 'none'}

    >>> print cjdns.RouterModule_lookup('fc5d:baa5:61fc:6ffd:9554:67f0:e290:7536')
    {'result': 'fcf1:a7a8:8ec0:589b:c64c:cc95:1ced:3679@0000.0000.0000.0013', 'error': 'none'}

    >>> print cjdns.RouterModule_lookup('f')
    {'result': '', 'error': 'address wrong length'}

    >>> print cjdns.RouterModule_lookup('zzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzzz')
    {'result': '', 'error': 'failed to parse address'}


### AuthorizedPasswords_add()

**Auth Required**

Parameters:

* String **password** a password which will allow neighbors to make direct connections.
* String **user** a friendly string to identify this password.
* Int **authType** (optional) the method for authenticating,
defaults to `1` (only currently supported method).

Returns:

* **error**:`none` if everything went well.
* **error**:`Specified auth type is not supported.` if the auth type is specified and not `1`.
* **error**:`Password already added.` if you try to add the same user or password twice.
* **error**:`Out of memory to store password.` if the buffer for storing
authorized passwords is full.

Examples:

    $ ./contrib/python/cexec 'AuthorizedPasswords_add(user="test",password="yh14wl2ffgcqq6bvut12xrz7g3")'
    {'error': 'none'}

    $ ./contrib/python/cexec 'AuthorizedPasswords_add(user="test2",password="2yh14wl2ffgcqq6bvut12xrz7g3",authType=300)'
    {'error': 'Specified auth type is not supported.'}

    $ ./contrib/python/cexec 'AuthorizedPasswords_add(user="test",password="yh14wl2ffgcqq6bvut12xrz7g3")'
    {'error': 'Password already added.'}

### AuthorizedPasswords_list()

**Auth Required**

Get a list of all the authorized users.

Example:

    $ ./contrib/python/cexec 'AuthorizedPasswords_list()'
    {'total': 2, 'users': ['Test User1', 'Local Peers'], 'txid': 'W0DUG0D50K'}

### memory()

Get the number of bytes of memory allocated by all memory allocators in the router.

Example:

    >>> cjdns.memory()
    {'bytes': 779259}


### NodeStore_dumpTable()

Parameters:

* Int **page** the page of the routing table to dump,
allowing you to get the whole table in a series of reasonably small requests.

Response:

* `routingTable` a key which contains a list of dictionaries, each containing `ip`,
`link` and `path`. `ip` is the IPv6 address of the node, `link` is a unitless number
between 0 inclusive and 2^32 exclusive, representing the router's opinion of the quality of
that path, higher is better. `path` is the route to the node.

* `more` to signal that there is another page of results, the engine will add a `more` key
with the integer 1, if there isn't another page of results, the `more` key will not be added.

What the data looks like:

    {
        'routingTable': [
            {
                'ip': 'fce5:de17:cbde:c87b:5289:0556:8b83:c9c8',
                'link': 4294967295,
                'path': '0000.0000.0000.0001'
            }, {
                'ip': 'fcfc:2ebe:346c:7fe7:95af:a58b:2631:dead',
                'link': 235149061,
                'path': '0000.0000.631a.3b53'
            }, {
                'ip': 'fc70:772a:f803:7c4e:38bd:981b:f791:60a1',
                'link': 271119350,
                'path': '0000.0000.017b.b333'
            },
            ..............................
        ],
        'more': 1
    }

Example:

    >>> cjdns.NodeStore_dumpTable(0)
    {'routingTable': [{'ip': 'fce5:de17:cbde:c87b:5289:0556:8b83:c9c8', 'link': 4294967295,....

    >>> cjdns.NodeStore_dumpTable(4)
    {'routingTable': []}


### SwitchPinger_ping()

**Auth Required**

Send a switch level ping. There is no routing table lookup and the router is not involved.
Pinging IP addresses this way is not possible.

Parameters:
SwitchPinger_ping(required String path, String data, Int timeout)
* String **path** the route to the node to ping eg: `0000.0000.04f5.2555`
* String **data** (optional) for diagnosing data-dependent errors.
* Int **timeout** (optional) milliseconds to wait for a response.
If unspecified, will default to `DEFAULT_TIMEOUT` as defined in `SwitchPinger_admin.c` (2 seconds).

Examples:

    >>> cjdns.SwitchPinger_ping('0000.0000.04f5.2555')
    {'path': '0000.0000.04f5.2555', 'data': '', 'result': 'pong', 'ms': 281}

    >>> cjdns.SwitchPinger_ping('fca5:9fe0:3fa2:d576:71e6:8373:7aeb:ea11')
    {'error': 'path was not parsable.'}

    >>> cjdns.SwitchPinger_ping('0000.0000.04f5.2555', '12345abcdefg')
    {'path': '0000.0000.04f5.2555', 'data': '12345abcdefg', 'result': 'pong', 'ms': 326}

    >>> cjdns.SwitchPinger_ping('0000.0000.0405.2555')
    {'path': '0000.0000.0405.2555', 'data': '', 'result': 'ping message caused switch error', 'ms': 278}

    >>> cjdns.SwitchPinger_ping('0000.0000.04f5.2555', '', 30)
    {'result': 'timeout', 'ms': 77}

