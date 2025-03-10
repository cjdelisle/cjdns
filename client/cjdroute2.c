/* vim: set expandtab ts=4 sw=4: */
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "exception/Err.h"
#include "rust/cjdns_sys/RTypes.h"
#include "util/events/Socket.h"
#include "wire/Message.h"
#include "client/AdminClient.h"
#include "admin/angel/Core.h"
#include "admin/angel/InterfaceWaiter.h"
#include "client/Configurator.h"
#include "crypto/Key.h"
#include "benc/Dict.h"
#include "benc/serialization/json/JsonBencMessageReader.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "exception/Err.h"
#include "interface/Iface.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "util/AddrTools.h"
#include "util/ArchInfo.h"
#include "util/Assert.h"
#include "util/Base32.h"
#include "util/CString.h"
#include "util/Defined.h"
#include "util/events/UDPAddrIface.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Process.h"
#include "util/events/libuv/Glock.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/SysInfo.h"
#include "util/version/Version.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define DEFAULT_TUN_DEV "tun0"

#ifndef CJD_PACKAGE_VERSION
    #define CJD_PACKAGE_VERSION "unknown"
#endif

static int genconf(struct Allocator* alloc, struct Random* rand, bool eth, bool seed)
{
    if (seed) {
        uint8_t seedbuf[64];
        Bits_memset(seedbuf, 0, 64);
        Assert_true(64 == read(STDIN_FILENO, seedbuf, 64));
        RandomSeed_t* rs = DeterminentRandomSeed_new(alloc, seedbuf);
        Err_assert(Random_newWithSeed(&rand, alloc, NULL, rs));
    }

    uint8_t password[32];
    uint8_t password2[32];
    uint8_t password3[32];
    uint8_t password4[32];
    Random_base32(rand, password, 32);
    Random_base32(rand, password2, 32);
    Random_base32(rand, password3, 32);
    Random_base32(rand, password4, 32);

    uint16_t port = 0;
    while (port <= 1024) {
        port = Random_uint16(rand);
    }

    uint8_t publicKey[32];
    uint8_t publicKeyBase32[53];
    uint8_t ip[16];
    uint8_t address[40];
    uint8_t privateKey[32];
    uint8_t privateKeyHex[65];
    Key_gen(ip, publicKey, privateKey, rand);
    Base32_encode(publicKeyBase32, 53, publicKey, 32);
    Hex_encode(privateKeyHex, 65, privateKey, 32);
    AddrTools_printIp(address, ip);

    printf("{\n");
    printf("    // Private key:\n"
           "    // Your confidentiality and data integrity depend on this key, keep it secret!\n"
           "    \"privateKey\": \"%s\",\n\n", privateKeyHex);
    printf("    // This key corresponds to the public key and ipv6 address:\n"
           "    \"publicKey\": \"%s.k\",\n", publicKeyBase32);
    printf("    \"ipv6\": \"%s\",\n", address);
    printf("\n"
           "    // Anyone connecting and offering these passwords on connection will be allowed.\n"
           "    //\n"
           "    // WARNING: If a \"login\" parameter is passed, someone sniffing on the wire can\n"
           "    //          sniff the packet and crack to find it. If the \"login\" is not passed\n"
           "    //          then the hash of the 'password' is effectively the login, therefore\n"
           "    //          that can be cracked.\n"
           "    //\n"
           "    \"authorizedPasswords\": [\n"
           "        // Password is a unique string which is known to the client and server.\n"
           "        // User is an optional login name and will also be used to display the peer.\n"
           "        { \"password\": \"%s\", \"user\": \"default-login\" }\n", password);
    printf("\n"
           "        // More passwords should look like this.\n"
           "        // { \"password\": \"%s\", \"user\": \"my-second-peer\" },\n", password2);
    printf("        // { \"password\": \"%s\", \"user\": \"my-third-peer\" },\n", password3);
    printf("        // { \"password\": \"%s\", \"user\": \"my-fourth-peer\" },\n", password4);
    printf("\n"
           "        // Below is an example of your connection credentials\n"
           "        // that you can give to other people so they can connect\n"
           "        // to you using your default password (from above).\n"
           "        // The login field here yourself to your peer and the peerName field\n"
           "        // is the name the peer which will be displayed in peerStats\n"
           "        // Adding a unique password for each peer is advisable\n"
           "        // so that leaks can be isolated.\n"
           "        /*\n"
           "         \"your.external.ip.goes.here:%u\": {\n", port);
    printf("             \"login\": \"default-login\",\n"
           "             \"password\": \"%s\",\n", password);
    printf("             \"publicKey\": \"%s.k\",\n", publicKeyBase32);
    printf("             \"peerName\": \"your-name-goes-here\"\n"
           "         },\n"
           "         */\n");
    printf("    ],\n"
           "\n"
           "    // Settings for administering and extracting information from your router.\n"
           "    // This interface provides functions which can be called through a UDP socket.\n"
           "    // See admin/Readme.md for more information about the API and try:\n"
           "    // ./tools/cexec\n"
           "    // For a list of functions which can be called.\n"
           "    // For example: ./tools/cexec 'memory()'\n"
           "    // will call a function which gets the core's current memory consumption.\n"
           "    // ./tools/cjdnslog\n"
           "    // is a tool which uses this admin interface to get logs from cjdns.\n"
           "    \"admin\": {\n"
           "        // Port to bind the admin RPC server to.\n"
           "        \"bind\": \"127.0.0.1:11234\",\n"
           "\n"
           "        // Password for admin RPC server.\n"
           "        // This is a static password by default, so that tools like\n"
           "        // ./tools/cexec can use the API without you creating a\n"
           "        // config file at ~/.cjdnsadmin first. If you decide to\n"
           "        // expose the admin API to the network, change the password!\n"
           "        \"password\": \"NONE\"\n");
    printf("    },\n"
           "\n"
           "    // Interfaces to connect to the switch core.\n"
           "    \"interfaces\": {\n"
           "        // The interface which connects over UDP/IP based VPN tunnel.\n"
           "        \"UDPInterface\": [\n"
           "            {\n"
           "                // Bind to this port.\n"
           "                \"bind\": \"0.0.0.0:%u\",\n", port);
    printf("                // Set the DSCP value for Qos. Default is 0.\n"
           "                // \"dscp\": 46,\n"
           "\n"
           "                // Automatically connect to other nodes on the same LAN\n"
           "                // This works by binding a second port and sending beacons\n"
           "                // containing the main data port.\n"
           "                // beacon is a number between 0 and 2:\n"
           "                //   0 -> do not beacon nor connect to other nodes who beacon\n"
           "                //   1 -> quiet mode, accept beacons from other nodes only\n"
           "                //   2 -> send and accept beacons\n"
           "                // beaconDevices is a list which can contain names of devices such\n"
           "                // as eth0, as well as broadcast addresses to send to, such as\n"
           "                // 192.168.101.255, or the pseudo-name \"all\".\n"
           "                // in order to auto-peer, all cjdns nodes must use the same\n"
           "                // beaconPort.\n"
           "                \"beacon\": 2,\n"
           "                \"beaconDevices\": [ \"all\" ],\n"
           "                \"beaconPort\": 64512,\n");
    printf("\n"
           "                // Nodes to connect to (IPv4 only).\n"
           "                \"connectTo\": {\n"
           "                    // Add connection credentials here to join the network\n"
           "                    // If you have several, don't forget the separating commas\n"
           "                    // They should look like:\n"
           "                    // \"ipv4 address:port\": {\n"
           "                    //     \"login\": \"(optional) name your peer has for you\"\n"
           "                    //     \"password\": \"password to connect with\",\n"
           "                    //     \"publicKey\": \"remote node key.k\",\n"
           "                    //     \"peerName\": \"(optional) human-readable name for peer\"\n"
           "                    // },\n"
           "                    // Ask somebody who is already connected.\n"
           "                }\n"
           "            },\n"
           "            {\n"
           "                // Bind to this port.\n"
           "                \"bind\": \"[::]:%u\",\n", port);
    printf("                // Set the DSCP value for Qos. Default is 0.\n"
           "                // \"dscp\": 46,\n");
    printf("\n"
           "                // Nodes to connect to (IPv6 only).\n"
           "                \"connectTo\": {\n"
           "                    // Add connection credentials here to join the network\n"
           "                    // Ask somebody who is already connected.\n"
           "                }\n"
           "            }\n");
#ifdef HAS_ETH_INTERFACE

    printf("        ],\n\n"
           "        // The interface which allows peering using layer-2 ethernet frames\n"
           "        \"%sETHInterface\": [\n"
           "            // Alternatively bind to just one device and either beacon and/or\n"
           "            // connect to a specified MAC address\n"
           "            {\n"
           "                // Bind to this device (interface name, not MAC)\n"
           "                // \"all\" is a pseudo-name which will try to connect to all devices.\n"
           "                \"bind\": \"all\",\n"
           "\n"
           "                // Auto-connect to other cjdns nodes on the same network.\n"
           "                // Options:\n"
           "                //\n"
           "                // 0 -- Disabled.\n"
           "                //\n"
           "                // 1 -- Accept beacons, this will cause cjdns to accept incoming\n"
           "                //      beacon messages and try connecting to the sender.\n"
           "                //\n"
           "                // 2 -- Accept and send beacons, this will cause cjdns to broadcast\n"
           "                //      messages on the local network which contain a randomly\n"
           "                //      generated per-session password, other nodes which have this\n"
           "                //      set to 1 or 2 will hear the beacon messages and connect\n"
           "                //      automatically.\n"
           "                //\n"
           "                \"beacon\": 2,\n"
           "\n"
           "                // Node(s) to connect to manually\n"
           "                // Note: does not work with \"all\" pseudo-device-name\n"
           "                \"connectTo\": {\n"
           "                    // Credentials for connecting look similar to UDP credentials\n"
           "                    // except they begin with the mac address, for example:\n"
           "                    // \"01:02:03:04:05:06\":{\"password\":\"a\",\"publicKey\":\"b\"}\n"
           "                }\n"
           "            }\n"
           "        ]\n\n", (eth) ? "" : "_disabled_");
#else
    printf("        ]\n");
#endif
    printf("    },\n"
           "\n"
           "    // Configuration for the router.\n"
           "    \"router\": {\n"
           "        // DNS Seeds, these will be used to add peers automatically.\n"
           "        // The first seed in the list is trusted to provide the snode.\n"
           "        \"dnsSeeds\": [\n"
           "            \"seed.pns.cjdns.fr\"\n"
           "        ],\n"
           "\n"
           "        // When publicPeer id is set, this node will post its public peering credentials\n"
           "        // to its supernode. The specified peerID will be used to identify itself.\n"
           "        // For PKT yielding this must be set to the registered peerID, otherwise\n"
           "        // you can set it to anything. By *convention*, peerIDs that begin with \"PUB_\"\n"
           "        // are shared publicly and those which do not are tested by the snode but not\n"
           "        // shared, allowing you to use the snode's peer tester on an otherwise private\n"
           "        // node. If you leave \"id\" commented, your peering credentials will remain\n"
           "        // entirely private.\n"
           "        //\n"
           "        \"publicPeer\": {\n"
           "            // \"id\": \"PUB_XXX\",\n"
           "\n"
           "            // If you set the public peer, you may also hardcode the IPv4 address.\n"
           "            // By default, cjdns will request its public IP address from its peers, but\n"
           "            // in cases with non-standard routing, you may have a different IP address\n"
           "            // for traffic initiated from outside. In this case, you must manually enter\n"
           "            // the IP address. If the address is entered in the form of \"x.x.x.x\", then\n"
           "            // the IP address will be used, but the port will be detected. If it is entered\n"
           "            // as \"0.0.0.0:xxx\" then the port will be used, but the address will be detected\n"
           "            // finally, if it is in the form of \"x.x.x.x:xxx\" then the address AND port will\n"
           "            // be used.\n"
           "            //\n"
           "            // \"ipv4\": \"1.2.3.4:56789\",\n"
           "\n"
           "            // If you have a public IPv6 address which cannot be detected, you may hard-code\n"
           "            // it here. The same rules apply as IPv4 addresses: \"xxxx:xxxx::\" means use ip\n"
           "            // but detect port. \"[::]:xxx\" means use port but detect ip, and\n"
           "            // \"[xxxx:xxxx::]:xxx\" means use ip and port from configuration.\n"
           "            //\n"
           "            // \"ipv6\": \"[1234:5678::]:9012\",\n"
           "        },\n"
           "\n"
           "        // supernodes, if none are specified they'll be taken from your peers\n"
           "        \"supernodes\": [\n"
           "            //\"6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k\",\n"
           "        ],\n"
           "\n"
           "        // The interface which is used for connecting to the cjdns network.\n"
           "        \"interface\": {\n"
           "            // The type of interface (only TUNInterface is supported for now)\n"
           "            \"type\": \"TUNInterface\"\n");
#ifndef __APPLE__
    printf("\n"
           "            // The name of a persistent TUN device to use.\n"
           "            // This for starting cjdroute as its own user.\n"
           "            // *MOST USERS DON'T NEED THIS*\n"
           "            //\"tunDevice\": \"" DEFAULT_TUN_DEV "\"\n");
#endif
    printf("        },\n"
           "\n"
           "        // As an alternative to the TUN interface, you can create a socket interface\n"
           "        // which will create a UNIX socket which emits packets that would otherwise\n"
           "        // be sent through the TUN device.\n"
           "        // To enable this interface, change the name of the above TUN interface to\n"
           "        // \"_disabled_interface\" and change the name of this interface to\n"
           "        // simply \"interface\"\n"
           "        \"_disabled_interface\": {\n"
           "            \"type\": \"SocketInterface\",\n"
           "\n"
           "            // The filesystem path to the socket to create or connect to.\n"
           "            \"socketFullPath\": \"/var/run/cjdns.sock\"\n"
           "        },\n"
           "\n");
    printf("        // System for tunneling IPv4 and ICANN IPv6 through cjdns.\n"
           "        // This is using the cjdns switch layer as a VPN carrier.\n"
           "        \"ipTunnel\": {\n"
           "            // Nodes allowed to connect to us.\n"
           "            // When a node with the given public key connects, give them the\n"
           "            // ip4 and/or ip6 addresses listed.\n"
           "            \"allowedConnections\": [\n");
    printf("                // Give the client an address on 192.168.1.0/24, and an address\n"
           "                // it thinks has all of IPv6 behind it.\n"
           "                // ip4Prefix is the set of addresses which are routable from the tun\n"
           "                // for example, if you're advertizing a VPN into a company network\n"
           "                // which exists in 10.123.45.0/24 space, ip4Prefix should be 24\n"
           "                // default is 32 for ipv4 and 128 for ipv6\n"
           "                // so by default it will not install a route\n"
           "                // ip4Alloc is the block of addresses which are allocated to the\n"
           "                // for example if you want to issue 4 addresses to the client, those\n"
           "                // being 192.168.123.0 to 192.168.123.3, you would set this to 30\n"
           "                // default is 32 for ipv4 and 128 for ipv6 (1 address)\n"
           "                // {\n"
           "                //     \"publicKey\": "
           "\"f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k\",\n"
           "                //     \"ip4Address\": \"192.168.1.24\",\n"
           "                //     \"ip4Prefix\": 0,\n"
           "                //     \"ip4Alloc\": 32,\n"
           "                //     \"ip6Address\": \"2001:123:ab::10\",\n"
           "                //     \"ip6Prefix\": 0\n"
           "                //     \"ip6Alloc\": 64,\n"
           "                // },\n"
           "\n"
           "                // It's ok to only specify one address and prefix/alloc are optional.\n"
           "                // {\n"
           "                //     \"publicKey\": "
           "\"ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k\",\n"
           "                //     \"ip4Address\": \"192.168.1.25\",\n"
           "                //     \"ip4Prefix\": 0,\n"
           "                // }\n"
           "            ],\n"
           "\n"
           "            \"outgoingConnections\": [\n"
           "                // Connect to one or more machines and ask them for IP addresses.\n"
           "                // \"6743gf5tw80ExampleExampleExampleExamplevlyb23zfnuzv0.k\",\n"
           "                // \"pw9tfmr8pcrExampleExampleExampleExample8rhg1pgwpwf80.k\",\n"
           "                // \"g91lxyxhq0kExampleExampleExampleExample6t0mknuhw75l0.k\"\n"
           "            ]\n"
           "        }\n"
           "    },\n"
           "\n");
    printf("    // Dropping permissions.\n"
           "    // In the event of a serious security exploit in cjdns, leak of confidential\n"
           "    // network traffic and/or keys is highly likely but the following rules are\n"
           "    // designed to prevent the attack from spreading to the system on which cjdns\n"
           "    // is running.\n"
           "    // Counter-intuitively, cjdns is *more* secure if it is started as root because\n"
           "    // non-root users do not have permission to use chroot or change usernames,\n"
           "    // limiting the effectiveness of the mitigations herein.\n"
           "    \"security\": [\n"
           "        // Change the user id to sandbox the cjdns process after it starts.\n"
           "        // If keepNetAdmin is set to 0, IPTunnel will be unable to set IP addresses\n"
           "        // and ETHInterface will be unable to hot-add new interfaces\n"
           "        // Use { \"setuser\": 0 } to disable.\n"
           "        // Default: enabled with keepNetAdmin\n");
           if (Defined(Cjdns_android) || Defined(darwin)) {
    printf("        { \"setuser\": 0 },\n");
           } else {
    printf("        { \"setuser\": \"nobody\", \"keepNetAdmin\": 1 },\n");
           }
    printf("\n"
           "        // Chroot changes the filesystem root directory which cjdns sees, blocking it\n"
           "        // from accessing files outside of the chroot sandbox, if the user does not\n"
           "        // have permission to use chroot(), this will fail quietly.\n"
           "        // Use { \"chroot\": 0 } to disable.\n");
          if (Defined(Cjdns_android)) {
    printf("        // Default: disabled\n"
           "        { \"chroot\": 0 },\n");
          }
          else {
    printf("        // Default: enabled (using \"/var/run\")\n"
           "        { \"chroot\": \"/var/run/\" },\n");
          }
    printf("\n"
           "        // Nofiles is a deprecated security feature which prevents cjdns from opening\n"
           "        // any files at all, using this will block setting of IP addresses and\n"
           "        // hot-adding ETHInterface devices but for users who do not need this, it\n"
           "        // provides a formidable sandbox.\n"
           "        // Default: disabled\n"
           "        { \"nofiles\": 0 },\n"
           "\n"
           "        // Noforks will prevent cjdns from spawning any new processes or threads,\n"
           "        // this prevents many types of exploits from attacking the wider system.\n"
           "        // Default: enabled\n"
           "        { \"noforks\": 1 },\n");
    printf("\n"
           "        // The client sets up the core using a sequence of RPC calls, the responses\n"
           "        // to these calls are verified but in the event that the client crashes\n"
           "        // setup of the core completes, it could leave the core in an insecure state\n"
           "        // This call constitutes the client telling the core that the security rules\n"
           "        // have been fully applied and the core may run. Without it, the core will\n"
           "        // exit within a few seconds with return code 232.\n"
           "        // Default: enabled\n"
           "        { \"setupComplete\": 1 }\n"
           "    ],\n"
           "\n"
           "    // Logging\n"
           "    \"logging\": {\n"
           "        // Uncomment to have cjdns log to stdout rather than making logs available\n"
           "        // via the admin socket.\n"
           "        // \"logTo\": \"stdout\"\n"
           "    },\n"
           "\n"
           "    // If set to non-zero, cjdns will not fork to the background.\n"
           "    // Recommended for use in conjunction with \"logTo\":\"stdout\".\n");
    printf("    \"noBackground\": %d,\n", Defined(win32) ? 1 : 0);
    printf("\n"
           "    // Path for admin control pipe:\n"
           "    // If you pass only a filename then cjdns will guess the full path\n"
           "    // On unix the default path is /tmp/\n"
           "    // On windows: \\\\.\\pipe\\\n"
           "    \"pipe\": \"cjdroute.sock\",\n");
    printf("\n"
           "    // This is to make the configuration be parsed in strict mode, which allows\n"
           "    // it to be edited externally using cjdnsconf.\n"
           "    \"version\": 2\n");
    printf("}\n");

    return 0;
}

static int usage(struct Allocator* alloc, char* appName)
{
    char* sysInfo = SysInfo_describe(SysInfo_detect(), alloc);
    printf("Cjdns %s %s\n"
           "Usage:\n"
           "    cjdroute --help                This information\n"
           "    cjdroute --genconf [--eth]     Generate a configuration file, write it to stdout\n"
           "                                   if --eth is specified then eth beaconing will\n"
           "                                   be enabled. Caution it can interfere with UDP\n"
           "                                   beaconing\n"
           "    cjdroute --genconf-seed [--eth] Generate a configuration file from a 64 byte seed\n"
           "                                   which is read in from stdin."
           "    cjdroute --version             Print the protocol version which this node speaks.\n"
           "    cjdroute --cleanconf < conf    Print a clean (valid json) version of the config.\n"
           "    cjdroute --nobg                Never fork to the background no matter the config.\n"
           "\n"
           "To get the router up and running.\n"
           "Step 1:\n"
           "  Generate a new configuration file.\n"
           "    cjdroute --genconf > cjdroute.conf\n"
           "\n"
           "Step 2:\n"
           "  Find somebody to connect to.\n"
           "  Check out the IRC channel #cjdns on Efnet and Freenode\n"
           "  for information about how to meet new people and make connect to them.\n"
           "  Read more here: https://github.com/cjdelisle/cjdns/#2-find-a-friend\n"
           "\n"
           "Step 3:\n"
           "  Add that somebody's node to your cjdroute.conf file.\n"
           "  https://github.com/cjdelisle/cjdns/#3-connect-your-node-to-your-friends-node\n"
           "\n"
           "Step 4:\n"
           "  Fire it up!\n"
           "    sudo cjdroute < cjdroute.conf\n"
           "\n"
           "For more information about other functions and non-standard setups, see README.md\n",
           ArchInfo_getArchStr(), sysInfo);

    return 0;
}

struct CheckRunningInstanceContext
{
    EventBase_t* base;
    struct Allocator* alloc;
    struct AdminClient_Result* res;
};

static void checkRunningInstanceCallback(struct AdminClient_Promise* p,
                                         struct AdminClient_Result* res)
{
    struct CheckRunningInstanceContext* ctx = p->userData;
    // Prevent this from freeing until after we drop out of the loop.
    Allocator_adopt(ctx->alloc, p->alloc);
    ctx->res = res;
    EventBase_endLoop(ctx->base);
}

static Err_DEFUN checkRunningInstance(struct Allocator* allocator,
                                 EventBase_t* base,
                                 String* addr,
                                 String* password,
                                 struct Log* logger)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Sockaddr_storage pingAddrStorage;
    if (Sockaddr_parse(addr->bytes, &pingAddrStorage)) {
        Allocator_free(alloc);
        Err_raise(allocator, "Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     addr->bytes);
    }

    struct UDPAddrIface* udp = NULL;
    RTypes_Error_t* err = UDPAddrIface_new(&udp, NULL, alloc);
    if (err) {
        Allocator_free(alloc);
        return err;
    }
    struct AdminClient* adminClient =
        AdminClient_new(&udp->generic, &pingAddrStorage.addr, password, base, logger, alloc);

    // 100 milliseconds is plenty to wait for a process to respond on the same machine.
    adminClient->millisecondsToWait = 100;

    Dict* pingArgs = Dict_new(alloc);

    // Indirection is needed otherwise adopt doesn't work
    struct Allocator* alloc1 = Allocator_child(alloc);
    struct AdminClient_Promise* pingPromise =
        AdminClient_rpcCall(String_new("ping", alloc), pingArgs, adminClient, alloc1);

    struct CheckRunningInstanceContext* ctx =
        Allocator_malloc(alloc, sizeof(struct CheckRunningInstanceContext));
    ctx->base = base;
    ctx->alloc = alloc;
    ctx->res = NULL;

    pingPromise->callback = checkRunningInstanceCallback;
    pingPromise->userData = ctx;

    EventBase_beginLoop(base);

    Assert_true(ctx->res);
    if (ctx->res->err != AdminClient_Error_TIMEOUT) {
        Err_raise(allocator, "Startup failed: cjdroute is already running. [%d]", ctx->res->err);
    }
    return NULL;
}

static void onCoreExit(int64_t exit_status, int term_signal)
{
    printf("Core exited with status [%d], signal [%d]\n", (int)exit_status, term_signal);
    exit(exit_status);
}

#define Chunk_MAX_LEN 4000
struct Chunk {
    uint32_t length;
    struct Chunk* next;
    uint8_t buf[Chunk_MAX_LEN];
};
static Message_t* readToMsg(FILE* f, struct Allocator* alloc)
{
    struct Allocator* child = Allocator_child(alloc);
    struct Chunk* c = NULL;
    uint32_t totalLength = 0;
    do {
        struct Chunk* cc = Allocator_calloc(child, sizeof(struct Chunk), 1);
        cc->length = fread(cc->buf, 1, Chunk_MAX_LEN, f);
        totalLength += cc->length;
        cc->next = c;
        c = cc;
    } while (c->length == Chunk_MAX_LEN);
    Message_t* out = Message_new(0, totalLength, alloc);
    while (c) {
        Err_assert(Message_epush(out, c->buf, c->length));
        c = c->next;
    }
    Allocator_free(child);
    return out;
}

static String* getPipePath(Dict* config, struct Allocator* alloc)
{
    String* pipePath = Dict_getStringC(config, "pipe");
    char* pp = (pipePath) ? pipePath->bytes : "cjdroute.sock";
    if (pp[0] == Pipe_PATH_SEP[0]) {
        return pipePath;
    }
    char* path = Pipe_PATH;
    if (Defined(Cjdns_android)) {
        char* t = getenv("TMPDIR");
        if (!t) {
            t = getenv("HOME");
        }
        if (t) {
            path = t;
        }
    }
    String* out = String_newBinary(NULL,
        strlen(pp) + strlen(Pipe_PATH_SEP) + strlen(path) + 2, alloc);
    snprintf(out->bytes, out->len, "%s%s%s", path, Pipe_PATH_SEP, pp);
    out->len = strlen(out->bytes);
    return out;
}

// This is invoked from cjdroute.rs
int cjdroute2_main(int argc, char** argv);
int cjdroute2_main(int argc, char** argv)
{
    Glock_init();
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif

    if (argc > 1 && (!CString_strcmp("angel", argv[1]) || !CString_strcmp("core", argv[1]))) {
        return Core_main(argc, argv);
    }

    Assert_ifParanoid(argc > 0);

    // Allow it to allocate 8MB
    struct Allocator* allocator = Allocator_new(1<<23);
    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, allocator, NULL));
    EventBase_t* eventBase = EventBase_new(allocator);

    if (argc == 2) {
        // one argument
        if ((CString_strcmp(argv[1], "--help") == 0) || (CString_strcmp(argv[1], "-h") == 0)) {
            return usage(allocator, argv[0]);
        } else if (CString_strcmp(argv[1], "--genconf-seed") == 0) {
            return genconf(allocator, rand, 0, 1);
        } else if (CString_strcmp(argv[1], "--genconf") == 0) {
            return genconf(allocator, rand, 0, 0);
        } else if ((CString_strcmp(argv[1], "--version") == 0)
            || (CString_strcmp(argv[1], "-v") == 0))
        {
            printf("Cjdns version: %s\n", CJD_PACKAGE_VERSION);
            printf("Cjdns protocol version: %d\n", Version_CURRENT_PROTOCOL);
            return 0;
        } else if (CString_strcmp(argv[1], "--cleanconf") == 0) {
            // Performed after reading configuration
        } else if (CString_strcmp(argv[1], "--nobg") == 0) {
            // Performed while reading configuration
        } else {
            fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[1]);
            fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
            return -1;
        }
    } else if (argc > 2) {
        // more than one argument?
        // because of '--pidfile $filename'?
        if (CString_strcmp(argv[1], "--pidfile") == 0) {
            fprintf(stderr, "\n'--pidfile' option is deprecated.\n");
        } else if (CString_strcmp(argv[1], "--genconf") == 0 ||
            CString_strcmp(argv[1], "--genconf-seed") == 0)
        {
            bool eth = 0;
            for (int i = 2; i < argc; i++) {
                if (!CString_strcmp(argv[i], "--eth")) {
                    eth = 1;
                } else {
                    fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[i]);
                    fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
                    return -1;
                }
            }
            return genconf(allocator, rand, eth, CString_strcmp(argv[1], "--genconf-seed") == 0);
        } else {
            fprintf(stderr, "%s: too many arguments [%s]\n", argv[0], argv[1]);
            fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
        }
        return -1;
    }

    if (isatty(STDIN_FILENO)) {
        // We were started from a terminal
        // The chances an user wants to type in a configuration
        // bij hand are pretty slim so we show him the usage
        return usage(allocator, argv[0]);
    } else {
        // We assume stdin is a configuration file and that we should
        // start routing
    }

    // First try reading the conf with the new parser, then try the old parser
    // and if the old parser fails or the parsed content contains "version": 2,
    // fail to launch
    Message_t* confMsg = readToMsg(stdin, allocator);
    Dict* config = NULL;
    RTypes_Error_t* err = JsonBencMessageReader_read(
        &config,
        Message_clone(confMsg, allocator),
        allocator,
        false
    );
    if (err) {
        // Try again with lax parsing to check if version is < 2
        Err_assert(JsonBencMessageReader_read(
            &config,
            Message_clone(confMsg, allocator),
            allocator,
            true
        ));
        int64_t* ver = Dict_getIntC(config, "version");
        if (ver && *ver > 1) {
            Err_assert(err);
        }
    }

    if (argc == 2 && CString_strcmp(argv[1], "--cleanconf") == 0) {
        // Slip a v2 in there because at this point, the conf file is definitely v2 valid
        Dict_putIntC(config, "version", 2, allocator);
        struct Message* msg = Message_new(0, 10000, allocator);
        Err_assert(JsonBencMessageReader_write(config, msg, allocator));
        struct Writer* stdoutWriter = FileWriter_new(stdout, allocator);
        stdoutWriter->write(stdoutWriter, Message_bytes(msg), Message_getLength(msg));
        printf("\n");
        return 0;
    }

    int forceNoBackground = 0;
    if (argc == 2 && CString_strcmp(argv[1], "--nobg") == 0) {
        forceNoBackground = 1;
    }

    struct Log* logger = FileWriterLog_new(stdout, allocator);
    Rffi_setLogger(logger);

    // --------------------- Get Admin  --------------------- //
    Dict* configAdmin = Dict_getDictC(config, "admin");
    String* adminPass = Dict_getStringC(configAdmin, "password");
    String* adminBind = Dict_getStringC(configAdmin, "bind");
    if (!adminPass) {
        adminPass = String_newBinary(NULL, 32, allocator);
        Random_base32(rand, (uint8_t*) adminPass->bytes, 32);
        adminPass->len = CString_strlen(adminPass->bytes);
    }
    if (!adminBind) {
        Assert_failure("You must specify admin.bind in the cjdroute.conf file.");
    }

    // --------------------- Welcome to cjdns ---------------------- //
    char* sysInfo = SysInfo_describe(SysInfo_detect(), allocator);
    Log_info(logger, "%s %s %s", CJD_PACKAGE_VERSION, ArchInfo_getArchStr(), sysInfo);

    // --------------------- Check for running instance  --------------------- //

    Log_info(logger, "Checking for running instance...");
    Err_assert(checkRunningInstance(allocator, eventBase, adminBind, adminPass, logger));

    // --------------------- Setup Pipes to Angel --------------------- //
    struct Allocator* corePipeAlloc = Allocator_child(allocator);
    String* pipePath = getPipePath(config, corePipeAlloc);
    if (!Defined(win32)) {
        // win32 sockets are not files
        char* lastsep = strrchr(pipePath->bytes, '/');
        Assert_true(lastsep);
        *lastsep = '\0';
        int ret = access(pipePath->bytes, W_OK);
        *lastsep = '/';
        if (ret) {
            Assert_failure("Pipe directory not writable: [%s]", pipePath->bytes);
        }
        if (unlink(pipePath->bytes) && (errno != ENOENT)) {
            Assert_failure("Unable to delete existing pipe at path [%s] err [%s]",
                pipePath->bytes, strerror(errno));
        }
    }

    const char* args[] = { "core", pipePath->bytes, NULL };

    // --------------------- Spawn Core --------------------- //
    String* privateKey = Dict_getStringC(config, "privateKey");

    const char* corePath = Process_getPath(allocator);

    if (!corePath) {
        Assert_failure("Can't find a usable cjdns core executable, "
                         "make sure it is in the same directory as cjdroute");
    }

    if (!privateKey) {
        Assert_failure("Need to specify privateKey.");
    }
    Process_spawn(corePath, args, allocator, onCoreExit);

    // --------------------- Wait for socket ------------------------- //
    // cycle for up to 1 minute
    bool exists = false;
    for (int i = 0; i < 2 * 10 * 60; i++) {
        Err_assert(Socket_fileExists(&exists, pipePath->bytes, allocator));
        if (exists) {
            break;
        }
        // sleep 50ms
        Rffi_sleep_ms_sync(50);
    }
    if (!exists) {
        Assert_failure("Core did not setup pipe file [%s] within 60 seconds",
            pipePath->bytes);
    }

    // --------------------- Connect to socket ------------------------- //
    Iface_t* corePipe = NULL;
    Err_assert(Socket_connect(&corePipe, pipePath->bytes, allocator));

    // --------------------- Pre-Configure Core ------------------------- //
    Dict* preConf = Dict_new(allocator);
    Dict* adminPreConf = Dict_new(allocator);
    Dict_putDictC(preConf, "admin", adminPreConf, allocator);
    Dict_putStringC(preConf, "privateKey", privateKey, allocator);
    Dict_putStringC(adminPreConf, "bind", adminBind, allocator);
    Dict_putStringC(adminPreConf, "pass", adminPass, allocator);
    Dict* logging = Dict_getDictC(config, "logging");
    if (logging) {
        Dict_putDictC(preConf, "logging", logging, allocator);
    }

    Message_t* toCoreMsg = Message_new(0, 1024, allocator);
    Err_assert(BencMessageWriter_write(preConf, toCoreMsg));
    Iface_CALL(corePipe->send, toCoreMsg, corePipe);

    Log_debug(logger, "Sent [%d] bytes to core", Message_getLength(toCoreMsg));

    // --------------------- Get Response from Core --------------------- //

    Message_t* fromCoreMsg = NULL;
    Err_assert(InterfaceWaiter_waitForData(&fromCoreMsg, corePipe, eventBase, allocator));
    Dict* responseFromCore = NULL;
    Err_assert(BencMessageReader_read(&responseFromCore, fromCoreMsg, allocator));

    // --------------------- Close the Core Pipe --------------------- //
    Allocator_free(corePipeAlloc);
    corePipe = NULL;

    // --------------------- Get Admin Addr/Port/Passwd --------------------- //
    Dict* responseFromCoreAdmin = Dict_getDictC(responseFromCore, "admin");
    adminBind = Dict_getStringC(responseFromCoreAdmin, "bind");

    if (!adminBind) {
        Assert_failure("didn't get address and port back from core");
    }
    struct Sockaddr_storage adminAddr;
    if (Sockaddr_parse(adminBind->bytes, &adminAddr)) {
        Assert_failure("Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     adminBind->bytes);
    }

    //Assert_ifParanoid(EventBase_eventCount(eventBase) == 1);

    // --------------------- Configuration ------------------------- //
    Configurator_config(config,
                        &adminAddr.addr,
                        adminPass,
                        eventBase,
                        logger,
                        allocator);

    // --------------------- noBackground ------------------------ //

    int64_t* noBackground = Dict_getIntC(config, "noBackground");
    if (forceNoBackground || (noBackground && *noBackground)) {
        Log_debug(logger, "Keeping cjdns client alive because %s",
            (forceNoBackground) ? "--nobg was specified on the command line"
                                : "noBackground was set in the configuration");
        EventBase_beginLoop(eventBase);
    }

    // Freeing this allocator here causes the core to be terminated in the epoll syscall.
    //Allocator_free(allocator);

    return 0;
}
