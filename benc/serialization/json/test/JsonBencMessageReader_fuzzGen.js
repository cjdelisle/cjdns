'use strict';
// stringify it to reduce size
const DEFAULT_CONF = JSON.stringify({
    // Private key:
    // Your confidentiality and data integrity depend on this key, keep it secret!
    "privateKey": "ba13c8023eda49bd626ebfda4ecf9abd0eda6e07a8e77f19aa0b20c0d69eb915",

    // This key corresponds to the public key and ipv6 address:
    "publicKey": "hukfzjh5n30m10303bkhjg1wvuft90s0cqqg5jrjgg781dbly2z0.k",
    "ipv6": "fccf:4352:ac9:7d1f:b32b:ba08:f086:8432",

    // Anyone connecting and offering these passwords on connection will be allowed.
    //
    // WARNING: If a "login" parameter is passed, someone sniffing on the wire can
    //          sniff the packet and crack to find it. If the "login" is not passed
    //          then the hash of the 'password' is effectively the login, therefore
    //          that can be cracked.
    //
    "authorizedPasswords":
    [
        // A unique string which is known to the client and server.
        // Specify an optional user to identify the peer locally.
        // It is not used for authentication.
        {"password": "jq89d23lbvm26tk4t36yg5ugj717wkr", "user": "default-login"}

        // More passwords should look like this.
        // {"password": "l5t3tpn7z710p8cxly59zxbmy5n8buu", "user": "my-second-peer"},
        // {"password": "97ylvcn49q4y19f7w7f248hts5l7pbp", "user": "my-third-peer"},
        // {"password": "yxdzufqxwkbgp4ubusglxm5su4hwp41", "user": "my-fourth-peer"},

        // Below is an example of your connection credentials
        // that you can give to other people so they can connect
        // to you using your default password (from above).
        // The login field here yourself to your peer and the peerName field
        // is the name the peer which will be displayed in peerStats
        // Adding a unique password for each peer is advisable
        // so that leaks can be isolated.
        /*
        "your.external.ip.goes.here:61151": {
            "login": "default-login",
            "password":"jq89d23lbvm26tk4t36yg5ugj717wkr",
            "publicKey":"hukfzjh5n30m10303bkhjg1wvuft90s0cqqg5jrjgg781dbly2z0.k",
            "peerName":"your-name-goes-here"
        },
        */
    ],

    // Settings for administering and extracting information from your router.
    // This interface provides functions which can be called through a UDP socket.
    // See admin/Readme.md for more information about the API and try:
    // ./tools/cexec
    // For a list of functions which can be called.
    // For example: ./tools/cexec 'memory()'
    // will call a function which gets the core's current memory consumption.
    // ./tools/cjdnslog
    // is a tool which uses this admin interface to get logs from cjdns.
    "admin":
    {
        // Port to bind the admin RPC server to.
        "bind": "127.0.0.1:11234",

        // Password for admin RPC server.
        // This is a static password by default, so that tools like
        // ./tools/cexec can use the API without you creating a
        // config file at ~/.cjdnsadmin first. If you decide to
        // expose the admin API to the network, change the password!
        "password": "NONE"
    },

    // Interfaces to connect to the switch core.
    "interfaces":
    {
        // The interface which connects over UDP/IP based VPN tunnel.
        "UDPInterface":
        [
            {
                // Bind to this port.
                "bind": "0.0.0.0:61151",

                // Nodes to connect to (IPv4 only).
                "connectTo":
                {
                    // Add connection credentials here to join the network
                    // If you have several, don't forget the separating commas
                    // They should look like:
                    // "ipv4 address:port": {
                    //     "login": "(optional) name your peer has for you"
                    //     "password": "password to connect with",
                    //     "publicKey": "remote node key.k",
                    //     "peerName": "(optional) human-readable name for peer"
                    // },
                    // Ask somebody who is already connected.
                }
            },
            {
                // Bind to this port.
                "bind": "[::]:61151",

                // Nodes to connect to (IPv6 only).
                "connectTo":
                {
                    // Add connection credentials here to join the network
                    // Ask somebody who is already connected.
                }
            }
        ]
,
        "ETHInterface":
        [
            // Alternatively bind to just one device and either beacon and/or
            // connect to a specified MAC address
            {
                // Bind to this device (interface name, not MAC)
                // "all" is a pseudo-name which will try to connect to all devices.
                "bind": "all",

                // Auto-connect to other cjdns nodes on the same network.
                // Options:
                //
                // 0 -- Disabled.
                //
                // 1 -- Accept beacons, this will cause cjdns to accept incoming
                //      beacon messages and try connecting to the sender.
                //
                // 2 -- Accept and send beacons, this will cause cjdns to broadcast
                //      messages on the local network which contain a randomly
                //      generated per-session password, other nodes which have this
                //      set to 1 or 2 will hear the beacon messages and connect
                //      automatically.
                //
                "beacon": 2,

                // Node(s) to connect to manually
                // Note: does not work with "all" pseudo-device-name
                "connectTo":
                {
                    // Credentials for connecting look similar to UDP credentials
                    // except they begin with the mac address, for example:
                    // "01:02:03:04:05:06":{"password":"a","publicKey":"b"}
                }
            }
        ]

    },

    // Configuration for the router.
    "router":
    {
        // The interface which is used for connecting to the cjdns network.
        "interface":
        {
            // The type of interface (only TUNInterface is supported for now)
            "type": "TUNInterface"
            // The type of tunfd (only "android" for now)
            // If "android" here, the tunDevice should be used as the pipe path
            // to transfer the tun file description.
            // "tunfd" : "android"
        },

        // System for tunneling IPv4 and ICANN IPv6 through cjdns.
        // This is using the cjdns switch layer as a VPN carrier.
        "ipTunnel":
        {
            // Nodes allowed to connect to us.
            // When a node with the given public key connects, give them the
            // ip4 and/or ip6 addresses listed.
            "allowedConnections":
            [
                // Give the client an address on 192.168.1.0/24, and an address
                // it thinks has all of IPv6 behind it.
                // ip4Prefix is the set of addresses which are routable from the tun
                // for example, if you're advertizing a VPN into a company network
                // which exists in 10.123.45.0/24 space, ip4Prefix should be 24
                // default is 32 for ipv4 and 128 for ipv6
                // so by default it will not install a route
                // ip4Alloc is the block of addresses which are allocated to the
                // for example if you want to issue 4 addresses to the client, those
                // being 192.168.123.0 to 192.168.123.3, you would set this to 30
                // default is 32 for ipv4 and 128 for ipv6 (1 address)
                // {
                //     "publicKey": "f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k",
                //     "ip4Address": "192.168.1.24",
                //     "ip4Prefix": 0,
                //     "ip4Alloc": 32,
                //     "ip6Address": "2001:123:ab::10",
                //     "ip6Prefix": 0
                //     "ip6Alloc": 64,
                // },

                // It's ok to only specify one address and prefix/alloc are optional.
                // {
                //     "publicKey": "ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k",
                //     "ip4Address": "192.168.1.25",
                //     "ip4Prefix": 0,
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

    // Dropping permissions.
    // In the event of a serious security exploit in cjdns, leak of confidential
    // network traffic and/or keys is highly likely but the following rules are
    // designed to prevent the attack from spreading to the system on which cjdns
    // is running.
    // Counter-intuitively, cjdns is *more* secure if it is started as root because
    // non-root users do not have permission to use chroot or change usernames,
    // limiting the effectiveness of the mitigations herein.
    "security":
    [
        // Change the user id to sandbox the cjdns process after it starts.
        // If keepNetAdmin is set to 0, IPTunnel will be unable to set IP addresses
        // and ETHInterface will be unable to hot-add new interfaces
        // Use { "setuser": 0 } to disable.
        // Default: enabled with keepNetAdmin
        { "setuser": "nobody", "keepNetAdmin": 1 },

        // Chroot changes the filesystem root directory which cjdns sees, blocking it
        // from accessing files outside of the chroot sandbox, if the user does not
        // have permission to use chroot(), this will fail quietly.
        // Use { "chroot": 0 } to disable.
        // Default: enabled (using "/var/run")
        { "chroot": "/var/run/" },

        // Nofiles is a deprecated security feature which prevents cjdns from opening
        // any files at all, using this will block setting of IP addresses and
        // hot-adding ETHInterface devices but for users who do not need this, it
        // provides a formidable sandbox.
        // Default: disabled
        { "nofiles": 0 },

        // Noforks will prevent cjdns from spawning any new processes or threads,
        // this prevents many types of exploits from attacking the wider system.
        // Default: enabled
        { "noforks": 1 },

        // Seccomp is the most advanced sandboxing feature in cjdns, it uses
        // SECCOMP_BPF to filter the system calls which cjdns is able to make on a
        // linux system, strictly limiting it's access to the outside world
        // This will fail quietly on any non-linux system
        // Default: enabled
        { "seccomp": 1 },

        // The client sets up the core using a sequence of RPC calls, the responses
        // to these calls are verified but in the event that the client crashes
        // setup of the core completes, it could leave the core in an insecure state
        // This call constitutes the client telling the core that the security rules
        // have been fully applied and the core may run. Without it, the core will
        // exit within a few seconds with return code 232.
        // Default: enabled
        { "setupComplete": 1 }
    ],

    // Logging
    "logging":
    {
        // Uncomment to have cjdns log to stdout rather than making logs available
        // via the admin socket.
        // "logTo":"stdout"
    },

    // If set to non-zero, cjdns will not fork to the background.
    // Recommended for use in conjunction with "logTo":"stdout".
    "noBackground":0,

    // Pipe file will store in this path, recommended value: /tmp (for unix),
    // \\\\.\\pipe (for windows)
    // /data/local/tmp (for rooted android)
    // /data/data/AppName (for non-root android)
    "pipe":"/tmp",
});

module.exports.defaultConfLen = () => { return DEFAULT_CONF.length; };
module.exports.defaultConf = (x) => {
    const out = [];
    for (let i = 0; i < DEFAULT_CONF.length; i++) {
        out.push(x + '[' + i + ']=' + DEFAULT_CONF.charCodeAt(i) + ';');
    }
    return out.join('\n');
}