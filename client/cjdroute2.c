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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "client/AdminClient.h"
#include "admin/angel/Core.h"
#include "admin/angel/InterfaceWaiter.h"
#include "client/Configurator.h"
#include "benc/Dict.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "benc/serialization/standard/BencMessageReader.h"
#include "benc/serialization/standard/BencMessageWriter.h"
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth.h"
#include "dht/Address.h"
#include "exception/Except.h"
#include "interface/Iface.h"
#include "io/FileReader.h"
#include "io/FileWriter.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "util/ArchInfo.h"
#include "util/Assert.h"
#include "util/Base32.h"
#include "util/CString.h"
#include "util/Defined.h"
#include "util/events/UDPAddrIface.h"
#include "util/events/Time.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Process.h"
#include "util/events/FakeNetwork.h"
#include "util/Hex.h"
#include "util/log/Log.h"
#include "util/log/FileWriterLog.h"
#include "util/SysInfo.h"
#include "util/version/Version.h"
#include "net/Benchmark.h"

#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define DEFAULT_TUN_DEV "tun0"

static int genAddress(uint8_t addressOut[40],
                      uint8_t privateKeyHexOut[65],
                      uint8_t publicKeyBase32Out[53],
                      struct Random* rand)
{
    struct Address address;
    uint8_t privateKey[32];

    for (;;) {
        Random_bytes(rand, privateKey, 32);
        crypto_scalarmult_curve25519_base(address.key, privateKey);
        // Brute force for keys until one matches FC00:/8
        if (AddressCalc_addressForPublicKey(address.ip6.bytes, address.key)) {
            Hex_encode(privateKeyHexOut, 65, privateKey, 32);
            Base32_encode(publicKeyBase32Out, 53, address.key, 32);
            Address_printShortIp(addressOut, &address);
            return 0;
        }
    }
}

static int genconf(struct Random* rand, bool eth)
{
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

    uint8_t publicKeyBase32[53];
    uint8_t address[40];
    uint8_t privateKeyHex[65];
    genAddress(address, privateKeyHex, publicKeyBase32, rand);

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
           "    \"authorizedPasswords\":\n"
           "    [\n"
           "        // A unique string which is known to the client and server.\n"
           "        // Specify an optional user to identify the peer locally.\n"
           "        // It is not used for authentication.\n"
           "        {\"password\": \"%s\", \"user\": \"default-login\"}\n", password);
    printf("\n"
           "        // More passwords should look like this.\n"
           "        // {\"password\": \"%s\", \"user\": \"my-second-peer\"},\n", password2);
    printf("        // {\"password\": \"%s\", \"user\": \"my-third-peer\"},\n", password3);
    printf("        // {\"password\": \"%s\", \"user\": \"my-fourth-peer\"},\n", password4);
    printf("\n"
           "        // Below is an example of your connection credentials\n"
           "        // that you can give to other people so they can connect\n"
           "        // to you using your default password (from above).\n"
           "        // The login field here yourself to your peer and the peerName field\n"
           "        // is the name the peer which will be displayed in peerStats\n"
           "        // Adding a unique password for each peer is advisable\n"
           "        // so that leaks can be isolated.\n"
           "        /*\n"
           "        \"your.external.ip.goes.here:%u\": {\n", port);
    printf("            \"login\": \"default-login\",\n"
           "            \"password\":\"%s\",\n", password);
    printf("            \"publicKey\":\"%s.k\",\n", publicKeyBase32);
    printf("            \"peerName\":\"your-name-goes-here\"\n"
           "        },\n"
           "        */\n");
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
           "    \"admin\":\n"
           "    {\n"
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
           "    \"interfaces\":\n"
           "    {\n"
           "        // The interface which connects over UDP/IP based VPN tunnel.\n"
           "        \"UDPInterface\":\n"
           "        [\n"
           "            {\n"
           "                // Bind to this port.\n"
           "                \"bind\": \"0.0.0.0:%u\",\n", port);
    printf("\n"
           "                // Nodes to connect to (IPv4 only).\n"
           "                \"connectTo\":\n"
           "                {\n"
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
    printf("\n"
           "                // Nodes to connect to (IPv6 only).\n"
           "                \"connectTo\":\n"
           "                {\n"
           "                    // Add connection credentials here to join the network\n"
           "                    // Ask somebody who is already connected.\n"
           "                }\n"
           "            }\n"
           "        ]\n");
#ifdef HAS_ETH_INTERFACE
    printf(",\n");
    if (!eth) {
        printf("        /*\n");
    }
    printf("        \"ETHInterface\":\n"
           "        [\n"
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
           "                \"connectTo\":\n"
           "                {\n"
           "                    // Credentials for connecting look similar to UDP credentials\n"
           "                    // except they begin with the mac address, for example:\n"
           "                    // \"01:02:03:04:05:06\":{\"password\":\"a\",\"publicKey\":\"b\"}\n"
           "                }\n"
           "            }\n"
           "        ]\n");
    if (!eth) {
        printf("        */\n");
    }
    printf("\n");
#endif
    printf("    },\n"
           "\n"
           "    // Configuration for the router.\n"
           "    \"router\":\n"
           "    {\n"
           "        // The interface which is used for connecting to the cjdns network.\n"
           "        \"interface\":\n"
           "        {\n"
           "            // The type of interface (only TUNInterface is supported for now)\n"
           "            \"type\": \"TUNInterface\"\n"
           "            // The type of tunfd (only \"android\" for now)\n"
           "            // If \"android\" here, the tunDevice should be used as the pipe path\n"
           "            // to transfer the tun file description.\n"
           "            // \"tunfd\" : \"android\"\n"
#ifndef __APPLE__
           "\n"
           "            // The name of a persistent TUN device to use.\n"
           "            // This for starting cjdroute as its own user.\n"
           "            // *MOST USERS DON'T NEED THIS*\n"
           "            //\"tunDevice\": \"" DEFAULT_TUN_DEV "\"\n"
#endif
           "        },\n"
           "\n"
           "        // System for tunneling IPv4 and ICANN IPv6 through cjdns.\n"
           "        // This is using the cjdns switch layer as a VPN carrier.\n"
           "        \"ipTunnel\":\n"
           "        {\n"
           "            // Nodes allowed to connect to us.\n"
           "            // When a node with the given public key connects, give them the\n"
           "            // ip4 and/or ip6 addresses listed.\n"
           "            \"allowedConnections\":\n");
    printf("            [\n"
           "                // Give the client an address on 192.168.1.0/24, and an address\n"
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
           "            \"outgoingConnections\":\n"
           "            [\n"
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
           "    \"security\":\n"
           "    [\n"
           "        // Change the user id to sandbox the cjdns process after it starts.\n"
           "        // If keepNetAdmin is set to 0, IPTunnel will be unable to set IP addresses\n"
           "        // and ETHInterface will be unable to hot-add new interfaces\n"
           "        // Use { \"setuser\": 0 } to disable.\n"
           "        // Default: enabled with keepNetAdmin\n"
           "        { \"setuser\": \"nobody\", \"keepNetAdmin\": 1 },\n"
           "\n"
           "        // Chroot changes the filesystem root directory which cjdns sees, blocking it\n"
           "        // from accessing files outside of the chroot sandbox, if the user does not\n"
           "        // have permission to use chroot(), this will fail quietly.\n"
           "        // Use { \"chroot\": 0 } to disable.\n");
          if (Defined(android)) {
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
           "        { \"noforks\": 1 },\n"
           "\n"
           "        // Seccomp is the most advanced sandboxing feature in cjdns, it uses\n"
           "        // SECCOMP_BPF to filter the system calls which cjdns is able to make on a\n"
           "        // linux system, strictly limiting it's access to the outside world\n"
           "        // This will fail quietly on any non-linux system\n");
          if (Defined(android)) {
    printf("        // Default: disabled\n"
           "        { \"seccomp\": 0 },\n");
          }
          else {
    printf("        // Default: enabled\n"
           "        { \"seccomp\": 1 },\n");
          }
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
           "    \"logging\":\n"
           "    {\n"
           "        // Uncomment to have cjdns log to stdout rather than making logs available\n"
           "        // via the admin socket.\n"
           "        // \"logTo\":\"stdout\"\n"
           "    },\n"
           "\n"
           "    // If set to non-zero, cjdns will not fork to the background.\n"
           "    // Recommended for use in conjunction with \"logTo\":\"stdout\".\n");
          if (Defined(win32)) {
    printf("    \"noBackground\":1,\n");
          }
          else {
    printf("    \"noBackground\":0,\n");
          }
    printf("\n"
           "    // Pipe file will store in this path, recommended value: /tmp (for unix),\n"
           "    // \\\\.\\pipe (for windows) \n"
           "    // /data/local/tmp (for rooted android) \n"
           "    // /data/data/AppName (for non-root android)\n");
          if (Defined(android)) {
    printf("    \"pipe\":\"/data/local/tmp\",\n");
          }
          else if (!Defined(win32)){
    printf("    \"pipe\":\"/tmp\",\n");
          }
    printf("}\n");

    return 0;
}

static int usage(struct Allocator* alloc, char* appName)
{
    char* sysInfo = SysInfo_describe(SysInfo_detect(), alloc);
    printf("Cjdns %s %s\n"
           "Usage:\n"
           "    cjdroute --help                This information\n"
           "    cjdroute --genconf [--no-eth]  Generate a configuration file, write it to stdout\n"
           "                                   if --no-eth is specified then eth beaconing will\n"
           "                                   be disabled.\n"
           "    cjdroute --bench               Run some cryptography performance benchmarks.\n"
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
           "  Check out the IRC channel or http://hyperboria.net/\n"
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
    struct EventBase* base;
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

static void checkRunningInstance(struct Allocator* allocator,
                                 struct EventBase* base,
                                 String* addr,
                                 String* password,
                                 struct Log* logger,
                                 struct Except* eh)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Sockaddr_storage pingAddrStorage;
    if (Sockaddr_parse(addr->bytes, &pingAddrStorage)) {
        Except_throw(eh, "Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     addr->bytes);
    }

    struct UDPAddrIface* udp = UDPAddrIface_new(base, NULL, alloc, NULL, logger);
    struct AdminClient* adminClient =
        AdminClient_new(&udp->generic, &pingAddrStorage.addr, password, base, logger, alloc);

    // 100 milliseconds is plenty to wait for a process to respond on the same machine.
    adminClient->millisecondsToWait = 100;

    Dict* pingArgs = Dict_new(alloc);

    struct AdminClient_Promise* pingPromise =
        AdminClient_rpcCall(String_new("ping", alloc), pingArgs, adminClient, alloc);

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
        Except_throw(eh, "Startup failed: cjdroute is already running. [%d]", ctx->res->err);
    }

    Allocator_free(alloc);
}

static void onCoreExit(int64_t exit_status, int term_signal)
{
    Assert_failure("Core exited with status [%d], signal [%d]\n", (int)exit_status, term_signal);
}

int main(int argc, char** argv)
{
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif

    if (argc > 1 && (!CString_strcmp("angel", argv[1]) || !CString_strcmp("core", argv[1]))) {
        return Core_main(argc, argv);
    }

    Assert_ifParanoid(argc > 0);
    struct Except* eh = NULL;

    // Allow it to allocate 8MB
    struct Allocator* allocator = MallocAllocator_new(1<<23);
    struct Random* rand = Random_new(allocator, NULL, eh);
    struct EventBase* eventBase = EventBase_new(allocator);

    if (argc >= 2) {
        // one argument
        if ((CString_strcmp(argv[1], "--help") == 0) || (CString_strcmp(argv[1], "-h") == 0)) {
            return usage(allocator, argv[0]);
        } else if (CString_strcmp(argv[1], "--genconf") == 0) {
            bool eth = 1;
            for (int i = 1; i < argc; i++) {
                if (!CString_strcmp(argv[i], "--no-eth")) {
                    eth = 0;
                }
            }
            return genconf(rand, eth);
        } else if (CString_strcmp(argv[1], "--pidfile") == 0) {
            // deprecated
            fprintf(stderr, "'--pidfile' option is deprecated.\n");
            return 0;
        } else if (CString_strcmp(argv[1], "--reconf") == 0) {
            // Performed after reading the configuration
        } else if (CString_strcmp(argv[1], "--bench") == 0) {
            Benchmark_runAll();
            return 0;
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
        fprintf(stderr, "%s: too many arguments [%s]\n", argv[0], argv[1]);
        fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
        // because of '--pidfile $filename'?
        if (CString_strcmp(argv[1], "--pidfile") == 0)
        {
            fprintf(stderr, "\n'--pidfile' option is deprecated.\n");
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

    struct Reader* stdinReader = FileReader_new(stdin, allocator);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(stdinReader, allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    if (argc == 2 && CString_strcmp(argv[1], "--cleanconf") == 0) {
        struct Writer* stdoutWriter = FileWriter_new(stdout, allocator);
        JsonBencSerializer_get()->serializeDictionary(stdoutWriter, &config);
        printf("\n");
        return 0;
    }

    int forceNoBackground = 0;
    if (argc == 2 && CString_strcmp(argv[1], "--nobg") == 0) {
        forceNoBackground = 1;
    }

    struct Log* logger = FileWriterLog_new(stdout, allocator);

    // --------------------- Get Admin  --------------------- //
    Dict* configAdmin = Dict_getDict(&config, String_CONST("admin"));
    String* adminPass = Dict_getString(configAdmin, String_CONST("password"));
    String* adminBind = Dict_getString(configAdmin, String_CONST("bind"));
    if (!adminPass) {
        adminPass = String_newBinary(NULL, 32, allocator);
        Random_base32(rand, (uint8_t*) adminPass->bytes, 32);
        adminPass->len = CString_strlen(adminPass->bytes);
    }
    if (!adminBind) {
        Except_throw(eh, "You must specify admin.bind in the cjdroute.conf file.");
    }

    // --------------------- Welcome to cjdns ---------------------- //
    char* sysInfo = SysInfo_describe(SysInfo_detect(), allocator);
    Log_info(logger, "Cjdns %s %s", ArchInfo_getArchStr(), sysInfo);

    // --------------------- Check for running instance  --------------------- //

    Log_info(logger, "Checking for running instance...");
    checkRunningInstance(allocator, eventBase, adminBind, adminPass, logger, eh);

    // --------------------- Setup Pipes to Angel --------------------- //
    struct Allocator* corePipeAlloc = Allocator_child(allocator);
    char corePipeName[64] = "client-core-";
    Random_base32(rand, (uint8_t*)corePipeName+CString_strlen(corePipeName), 31);
    String* pipePath = Dict_getString(&config, String_CONST("pipe"));
    if (!pipePath) {
        pipePath = String_CONST(Pipe_PATH);
    }
    if (!Defined(win32) && access(pipePath->bytes, W_OK)) {
        Except_throw(eh, "Can't have writable permission to pipe directory.");
    }
    Assert_ifParanoid(EventBase_eventCount(eventBase) == 0);
    struct Pipe* corePipe = Pipe_named(pipePath->bytes, corePipeName, eventBase,
                                       eh, corePipeAlloc);
    Assert_ifParanoid(EventBase_eventCount(eventBase) == 2);
    corePipe->logger = logger;

    char* args[] = { "core", pipePath->bytes, corePipeName, NULL };

    // --------------------- Spawn Angel --------------------- //
    String* privateKey = Dict_getString(&config, String_CONST("privateKey"));

    char* corePath = Process_getPath(allocator);

    if (!corePath) {
        Except_throw(eh, "Can't find a usable cjdns core executable, "
                         "make sure it is in the same directory as cjdroute");
    }

    if (!privateKey) {
        Except_throw(eh, "Need to specify privateKey.");
    }
    Process_spawn(corePath, args, eventBase, allocator, onCoreExit);

    // --------------------- Pre-Configure Core ------------------------- //
    Dict* preConf = Dict_new(allocator);
    Dict* adminPreConf = Dict_new(allocator);
    Dict_putDict(preConf, String_CONST("admin"), adminPreConf, allocator);
    Dict_putString(preConf, String_CONST("privateKey"), privateKey, allocator);
    Dict_putString(adminPreConf, String_CONST("bind"), adminBind, allocator);
    Dict_putString(adminPreConf, String_CONST("pass"), adminPass, allocator);
    Dict* logging = Dict_getDict(&config, String_CONST("logging"));
    if (logging) {
        Dict_putDict(preConf, String_CONST("logging"), logging, allocator);
    }

    struct Message* toCoreMsg = Message_new(0, 1024, allocator);
    BencMessageWriter_write(preConf, toCoreMsg, eh);
    Iface_CALL(corePipe->iface.send, toCoreMsg, &corePipe->iface);

    Log_debug(logger, "Sent [%d] bytes to core", toCoreMsg->length);

    // --------------------- Get Response from Core --------------------- //

    struct Message* fromCoreMsg =
        InterfaceWaiter_waitForData(&corePipe->iface, eventBase, allocator, eh);
    Dict* responseFromCore = BencMessageReader_read(fromCoreMsg, allocator, eh);

    // --------------------- Close the Core Pipe --------------------- //
    Allocator_free(corePipeAlloc);
    corePipe = NULL;

    // --------------------- Get Admin Addr/Port/Passwd --------------------- //
    Dict* responseFromCoreAdmin = Dict_getDict(responseFromCore, String_CONST("admin"));
    adminBind = Dict_getString(responseFromCoreAdmin, String_CONST("bind"));

    if (!adminBind) {
        Except_throw(eh, "didn't get address and port back from core");
    }
    struct Sockaddr_storage adminAddr;
    if (Sockaddr_parse(adminBind->bytes, &adminAddr)) {
        Except_throw(eh, "Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     adminBind->bytes);
    }

    Assert_ifParanoid(EventBase_eventCount(eventBase) == 1);

    // --------------------- Configuration ------------------------- //
    Configurator_config(&config,
                        &adminAddr.addr,
                        adminPass,
                        eventBase,
                        logger,
                        allocator);

    // --------------------- noBackground ------------------------ //

    int64_t* noBackground = Dict_getInt(&config, String_CONST("noBackground"));
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
