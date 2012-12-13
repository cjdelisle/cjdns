Configuring cjdns
=================

In this document we are going to go over how to configure cjdns and what exactly each settings means. Note that this is a living document and this software is still in the alpha stages so things are subject to change.

cjdroute.conf
-------------

Let's start from the top of the file. First off you may notice it's JSON, ecxcept this JSON has comments. Technically that's not valid but it's also not un-common. Cjdns strips out the comments before parsing it so no worries. 

Your Keys and Address
---------------------
````javascript
{
    // The path to the cjdns core executable.
    "corePath": "/opt/cjdns/cjdns",

    // Private key:
    // Your confidentiality and data integrity depend on this key, keep it secret!
    "privateKey": "823e45d5ca85b6f89d2a7feb56d5214d0e2e3afd5456eb0af2c4ac10350ebc70",

    // This key corresponds to the public key and ipv6 address:
    "publicKey": "u2jf87mgqlxfzdnywp60z3tx6tkulvgh2nyc2jk1zc69zzt2s8u0.k",
    "ipv6": "fcff:a215:1e7b:a4e9:c00d:0813:93b3:7c87",
````

- `corePath`: This specifies where the core cjdns executable file is. If you downloaded the source to /opt/cjdns, then the default is fine. If you downloaded it somewhere else, like your home directory for example, then this needs to be udpdated accordingly.
- `privateKey`: Your private key is part of the system that ensures all the data coming and going out of your computer is encrypted. You must protect your private key. Do not give it out to anyone.
- `publicKey`: The public key is what your computer gives to other computers to encrypt data with. This data can then only be decrypted with your private key, that way no one can access your information as it moves across the network.
- `ipv6`: This is your IP address on the cjdns network. It is unique to you and is created by securely hashing your public key.

Incoming Connections
--------------------
````javascript
    // Anyone connecting and offering these passwords on connection will be allowed.
    //
    // WARNING: Currently there is no key derivation done on the password field,
    //          DO NOT USE A PASSWORD HERE use something which is truly random and
    //          cannot be guessed.
    // Including a username in the beginning of the password string is encouraged
    // to aid in remembering which users are who.
    //
    "authorizedPasswords":
    [
        // A unique string which is known to the client and server.
        {"password": "zxl6zgxpl4stnuybdt0xlg4tn2cdl5h"}

        // More passwords should look like this.
        // {"password": "10ru8br0mhk25ccpvubv0sqnl7kuc6s"},
        // {"password": "y68jm490dztxn3d2gvuv09bz55wqmjj"},
        // {"password": "bnpphnq205v8nf2ksrs1fknfr572xzc"},

        // These are your connection credentials
        // for people connecting to you with your default password.
        // adding more passwords for different users is advisable
        // so that leaks can be isolated.
        //
        // "your.external.ip.goes.here:33808":{"password":"zxl6zgxpl4stnuybdt0xlg4tn2cdl5h","publicKey":"u2jf87mgqlxfzdnywp60z3tx6tkulvgh2nyc2jk1zc69zzt2s8u0.k"}
    ],
````

The `authorizedPasswords` section is the area where you can specify passwords to allow people to connect to you. Any system that presents a valud password will be allowed access. 
**NOTE:** These passwords should be long and random. There is no reason to make them short, easily remembered words becuase they are only going to be used by cjdns software.

Admin Interface
---------------
````javascript
    // Settings for administering and extracting information from your router.
    // This interface provides functions which can be called through a TCP socket.
    "admin":
    {
        // Port to bind the admin RPC server to.
        "bind": "127.0.0.1:11234",

        // Password for admin RPC server.
        "password": "j6mukf2khplcgpbzz0kulb8hu0xq2v9"
    },
````
The `admin ` section defines the settings for the administrative interface of cjdns. Many of the scripts in `/contrib/` use this to interact with cjdns. You probably wont need to use anything in there unless you are helping to test something out. 

Connection Interface(s)
-----------------------
````javascript
    // Interfaces to connect to the switch core.
    "interfaces":
    {
        // The interface which connects over UDP/IP based VPN tunnel.
        "UDPInterface":
        [
            {
                // Bind to this port.
                "bind": "0.0.0.0:33808",

                // Nodes to connect to.
                "connectTo":
                {
                    // Add connection credentials here to join the network
                    // Ask somebody who is already connected.
                }
            }
        ]

        /*
        "ETHInterface":
        [
            {
                // Bind to this device (interface name, not MAC etc.)
                "bind": "eth0", 
                // Node(s) to connect to.
                "connectTo":
                {
                    // Add connection credential here to join the network
                    // Ask your peer on the other side of the link.
                }
            }
        ]
        */
    },
````

Miscellaneous
-------------
````javascript
    // Configuration for the router.
    "router":
    {
        // The interface which is used for connecting to the cjdns network.
        "interface":
        {
            // The type of interface (only TUNInterface is supported for now)
            "type": "TUNInterface"

            // The name of a persistent TUN device to use.
            // This for starting cjdroute as its own user.
            // *MOST USERS DON'T NEED THIS*
            //"tunDevice": "tun0"
        },
````
````javascript
        // System for tunneling IPv4 and ICANN IPv6 through cjdns.
        // This is using the cjdns switch layer as a VPN carrier.
        "ipTunnel":
        {
            // Nodes allowed to connect to us.
            // When a node with the given public key connects, give them the
            // ip4 and/or ip6 addresses listed.
            "allowedConnections":
            [
                // {
                //     "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                //     "ip4Address": "192.168.1.24",
                //     "ip6Address": "2001:123:ab::10"
                // },

                // It's ok to only specify one address.
                // {
                //     "publicKey": "ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k",
                //     "ip4Address": "192.168.1.24",
                //     "ip6Address": "2001:123:ab::10"
                // }
            ],

            "outgoingConnections":
            [
                // Connect to one or more machines and ask them for IP addresses.
                // "6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k",
                // "pw9tfmr8pcrExampleExampleExampleExample8rhg1pgwpwf80.k",
                // "g91lxyxhq0kExampleExampleExampleExample6t0mknuhw75l0.k"
            ]
        }
    },
````
 
````javascript
    // Tear down inactive CryptoAuth sessions after this number of seconds
    // to make them more forgiving in the event that they become desynchronized.
    "resetAfterInactivitySeconds": 100,

    // Save the pid of the running process to this file.
    // If this file cannot be opened for writing, the router will not start.
    //"pidFile": "cjdroute.pid",

    // Dropping permissions.
    "security":
    [
        // Set number of open files to zero, in Linux, this will succeed even if
        // files are already open and will not allow any files to be opened for the
        // duration of the program's operation.
        // Most security exploits require the use of files.
        "nofiles",

        // Change the user id to this user after starting up and getting resources.
        {"setuser": "nobody"}
     ],

    // Version of the config file, used internally for migration.
    "version": 1
}
````


