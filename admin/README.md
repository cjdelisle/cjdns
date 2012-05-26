cjdns Admin API
===============

Functions:

RouterModule_pingNode(required String path, Int timeout)
UDPInterface_scrambleKeys(required String xorValue)
ping()
RouterModule_lookup(required String address)
AuthorizedPasswords_add(required String password, Int authType)
AuthorizedPasswords_flush()
memory()
NodeStore_dumpTable(required Int page)
UDPInterface_beginConnection(required String publicKey, required String address, String password)
SwitchPinger_ping(required String path, String data, Int timeout)


RouterModule_pingNode()
-----------------------

**Auth Required**

Send a node a cjdns ping request.

Parameters:

* String **path** may be a route such as "0000.0000.0000.1d53" or an ip address such as
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


UDPInterface_scrambleKeys()
---------------------------

**Auth Required**

Used for testing the behavior of the router when the IPv4 address
of a neighbor changes, likely to be removed in the future.


ping()
------

Returns:

    {'q':'pong'}

For checking if the admin connection is functioning.


RouterModule_lookup()
---------------------

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


AuthorizedPasswords_add()
-------------------------

**Auth Required**

Parameters:

* String **password** a password which will allow neighbors to make direct connections.
* Int **authType** (optional) the method for authenticating,
defaults to `1` (only currently supported method).

Returns:

* **error**:`none` if everything went well.
* **error**:`Specified auth type is not supported.` if the auth type is specified and not `1`.
* **error**:`Password already added.` if you try to add the same password twice.
* **error**:`Out of memory to store password.` if the buffer for storing
authorized passwords is full.

Examples:

    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3', 1)
    {'error': 'none'}

    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3', 300)
    {'error': 'Specified auth type is not supported.'}

    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3')
    {'error': 'Password already added.'}


AuthorizedPasswords_flush()
---------------------------

**Auth Required**

Remove all Authorized passwords from the system.

Example:

    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3')
    {'error': 'none'}
    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3')
    {'error': 'Password already added.'}
    >>> cjdns.AuthorizedPasswords_flush()
    {'error': 'none'}
    >>> cjdns.AuthorizedPasswords_add('yh14wl2ffgcqq6bvut12xrz7g3')
    {'error': 'none'}


memory()
--------

Get the number of bytes of memory allocated by all memory allocators in the router.

Example:

    >>> cjdns.memory()
    {'bytes': 779259}


NodeStore_dumpTable()
---------------------

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


UDPInterface_beginConnection()
------------------------------

**Auth Required**

Start a direct connection to another node.

Parameters:

* String **publicKey** the base32 public key for the node to connect to, ending in .k.
* String **address** the ip address and port for the node, at this time DNS resolution
and IPv6 is not supported.
* String **password** (optional) a password to use when connecting.

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


SwitchPinger_ping()
-------------------

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

