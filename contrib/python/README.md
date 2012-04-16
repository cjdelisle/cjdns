# cjdns.py

Using cjdns.py is trivially simple, you can connect to a router and issue it
commands in 3 steps:

## from cjdns import cjdns_connect;
Make sure cjdns.py and bencode.py are in your path, the easiest thing to do is
put them in the same directory as your script.

## cjdns_connect()
This takes 3 parameters, the ip address of the listening router, it's port
number, and the password to connect to it. These can be found in your
cjdroute.conf here:

    "admin":
    {
        // Port to bind the admin RPC server to.
        "bind": "127.0.0.1:11234",

        // Password for admin RPC server.
        "password": "4s8mshm4hbb2lbdwz4bxfdn9w7"
    },

To connect to this node, you would use:

    cjdns = cjdns_connect('127.0.0.1', 11234, '4s8mshm4hbb2lbdwz4bxfdn9w7');

The password will be checked when you connect and if it's incorrect you will get
an exception.


## Use it!

The `cjdns` object returned from `cjdns_connect()` contains functions
corrisponding to each of the RPC calls in the router. There is a field called
`cjdns.functions` which contains a list of the functions and their parameters.


## Example usage of cjdns.py

    user@debo8:~/wrk/play$ python
    Python 2.7.2+ (default, Jan 20 2012, 17:51:10) 
    [GCC 4.6.2] on linux2
    Type "help", "copyright", "credits" or "license" for more information.
    >>> from cjdns import cjdns_connect;
    >>> cjdns = cjdns_connect('127.0.0.1', 11234, '4s8mshm4hbb2lbdwz4bxfdn9w7');
    >>> print(cjdns.functions);
    RouterModule_pingNode(required String path)
    UDPInterface_scrambleKeys(required String xorValue)
    ping()
    AuthorizedPasswords_add(Int authType, required String password)
    AuthorizedPasswords_flush()
    memory()
    NodeStore_dumpTable()
    UDPInterface_beginConnection(required String publicKey, String password, required String address)
    >>> print cjdns.AuthorizedPasswords_add(1, 1);
    {'error': 'Entry [password] is required and must be of type [String]'}
    >>> print cjdns.AuthorizedPasswords_add(1, 'abcd');
    {'error': 'none'}
    >>> print cjdns.memory();
    {'bytes': 750556}
    >>> routes = cjdns.NodeStore_dumpTable();
    >>> print routes;
    {'routingTable': [{'ip': 'fc45:a51e:89eb:6d57:ad43:5723:e1d3:5d51', 'link': 4294967295, 'path': '0000.0000.0000.0001'}, {'ip': 'fcf1:a7a8:8ec0:589b:c64c:cc95:1ced:3679', 'link': 266287520, 'path': '0000.0000.0000.0006'}, {'ip': 'fce5:de17:cbde:c87b:5289:0556:8b83:c9c8', 'link': 0, 'path': '0000.0000.0000.0004'}]}
    >>> for route in routes['routingTable']: print route['ip'] + "@" + route['path'] + "\n";
    ... 
    fc45:a51e:89eb:6d57:ad43:5723:e1d3:5d51@0000.0000.0000.0001

    fcf1:a7a8:8ec0:589b:c64c:cc95:1ced:3679@0000.0000.0000.0006

    fce5:de17:cbde:c87b:5289:0556:8b83:c9c8@0000.0000.0000.0004

    >>> cjdns.disconnect();
    >>> exit(0)

