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
#define string_strcmp
#define string_strrchr
#define string_strlen
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "admin/angel/InterfaceWaiter.h"
#include "admin/angel/AngelInit.h"
#include "admin/angel/Core.h"
#include "admin/AuthorizedPasswords.h"
#include "admin/Configurator.h"
#include "benc/Int.h"
#include "crypto/AddressCalc.h"
#include "crypto/CryptoAuth.h"
#include "crypto/CryptoAuth_benchmark.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/UDPInterface_admin.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "util/log/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "net/Ducttape.h"
#include "net/DefaultInterfaceController.h"
#include "net/SwitchPinger.h"
#include "net/SwitchPinger_admin.h"
#include "switch/SwitchCore.h"
#include "util/platform/libc/string.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Process.h"
#include "util/Assert.h"
#include "util/Base32.h"
#include "util/Hex.h"
#include "util/Security.h"
#include "util/log/WriterLog.h"
#include "util/version/Version.h"

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
            Address_printIp(addressOut, &address);
            return 0;
        }
    }
}

static int genconf(struct Random* rand)
{
    uint8_t password[32];
    uint8_t password2[32];
    uint8_t password3[32];
    uint8_t password4[32];
    Random_base32(rand, password, 32);
    Random_base32(rand, password2, 32);
    Random_base32(rand, password3, 32);
    Random_base32(rand, password4, 32);

    uint8_t adminPassword[32];
    Random_base32(rand, adminPassword, 32);

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
           "    // WARNING: Currently there is no key derivation done on the password field,\n"
           "    //          DO NOT USE A PASSWORD HERE use something which is truly random and\n"
           "    //          cannot be guessed.\n"
           "    // Including a username in the beginning of the password string is encouraged\n"
           "    // to aid in remembering which users are who.\n"
           "    //\n"
           "    \"authorizedPasswords\":\n"
           "    [\n"
           "        // A unique string which is known to the client and server.\n"
           "        {\"password\": \"%s\"}\n", password);
    printf("\n"
           "        // More passwords should look like this.\n"
           "        // {\"password\": \"%s\"},\n", password2);
    printf("        // {\"password\": \"%s\"},\n", password3);
    printf("        // {\"password\": \"%s\"},\n", password4);
    printf("\n"
           "        // Below is an example of your connection credentials\n"
           "        // that you can give to other people so they can connect\n"
           "        // to you using your default password (from above) \n"
           "        // Adding a unique password for each user is advisable\n"
           "        // so that leaks can be isolated. \n"
           "        //\n"
           "        // \"your.external.ip.goes.here:%u\":{", port);
    printf("\"password\":\"%s\",", password);
    printf("\"publicKey\":\"%s.k\"}\n", publicKeyBase32);
    printf("    ],\n"
           "\n"
           "    // Settings for administering and extracting information from your router.\n"
           "    // This interface provides functions which can be called through a UDP socket.\n"
           "    // See admin/Readme.md for more information about the API and try:\n"
           "    // ./contrib/python/cexec 'functions'\n"
           "    // For a list of functions which can be called.\n"
           "    // For example:  ./contrib/python/cexec 'memory()'\n"
           "    // will call a function which gets the core's current memory consumption.\n"
           "    // ./contrib/python/cjdnslog\n"
           "    // is a tool which uses this admin interface to get logs from cjdns.\n"
           "    \"admin\":\n"
           "    {\n"
           "        // Port to bind the admin RPC server to.\n"
           "        \"bind\": \"127.0.0.1:11234\",\n"
           "\n"
           "        // Password for admin RPC server.\n"
           "        \"password\": \"%s\"\n", adminPassword);
    printf("    },\n"
           "\n"
           "\n\n" // TODO: Why is this needed and where are these newlines going?!!
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
           "                // Nodes to connect to.\n"
           "                \"connectTo\":\n"
           "                {\n"
           "                    // Add connection credentials here to join the network\n"
           "                    // Ask somebody who is already connected.\n"
           "                }\n"
           "            }\n"
           "        ]\n");
#ifdef HAS_ETH_INTERFACE
    printf("\n"
           "        /*\n"
           "        \"ETHInterface\":\n"
           "        [\n"
           "            {\n"
           "                // Bind to this device (interface name, not MAC etc.)\n"
           "                \"bind\": \"eth0\",\n"
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
           "                // Node(s) to connect to manually.\n"
           "                \"connectTo\":\n"
           "                {\n"
           "                    // Credentials for connecting look similar to UDP credientials\n"
           "                    // except they begin with the mac address, for example:\n"
           "                    // \"01:02:03:04:05:06\":{\"password\":\"a\",\"publicKey\":\"b\"}\n"
           "                }\n"
           "            }\n"
           "        ]\n"
           "        */\n"
           "\n");
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
           "            \"allowedConnections\":\n"
           "            [\n"
           "                // {\n"
           "                //     \"publicKey\": "
           "\"f64hfl7c4uxt6krmhPutTheRealAddressOfANodeHere7kfm5m0.k\",\n"
           "                //     \"ip4Address\": \"192.168.1.24\",\n"
           "                //     \"ip6Address\": \"2001:123:ab::10\"\n"
           "                // },\n"
           "\n"
           "                // It's ok to only specify one address.\n"
           "                // {\n"
           "                //     \"publicKey\": "
           "\"ydq8csdk8p8ThisIsJustAnExampleAddresstxuyqdf27hvn2z0.k\",\n"
           "                //     \"ip4Address\": \"192.168.1.24\",\n"
           "                //     \"ip6Address\": \"2001:123:ab::10\"\n"
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
           "\n"
           "    // Tear down inactive CryptoAuth sessions after this number of seconds\n"
           "    // to make them more forgiving in the event that they become desynchronized.\n"
           "    \"resetAfterInactivitySeconds\": 100,\n"
           "\n"
           "    // Dropping permissions.\n"
           "    \"security\":\n"
           "    [\n"
           "        // Set number of open files to zero, in Linux, this will succeed even if\n"
           "        // files are already open and will not allow any files to be opened for the\n"
           "        // duration of the program's operation.\n"
           "        // Most security exploits require the use of files.\n"
           "        \"nofiles\",\n"
           "\n"
           "        // Change the user id to this user after starting up and getting resources.\n"
           "        {\n"
           "            \"setuser\": \"nobody\",\n"
           "\n"
           "            // Exempt the Angel process from setting userId, the Angel is a small\n"
           "            // isolated piece of code which exists outside of the core's strict\n"
           "            // sandbox but does not handle network traffic.\n"
           "            // This must be enabled for IpTunnel to automatically set IP addresses\n"
           "            // for the TUN device.\n"
           "            \"exemptAngel\": 1\n"
           "        }\n"
           "     ],\n"
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
           "    // Recommended for use in conjunction with \"logTo\":\"stdout\".\n"
           "    \"noBackground\":0\n"
           "}\n");

    return 0;
}

static int usage(char* appName)
{
    printf("Usage: %s [--help] [--genconf] [--bench] [--version] [--cleanconf]\n"
           "\n"
           "To get the router up and running.\n"
           "Step 1:\n"
           "  Generate a new configuration file.\n"
           "    %s --genconf > cjdroute.conf\n"
           "\n"
           "Step 2:\n"
           "  Find somebody to connect to.\n"
           "  Check out the IRC channel or http://hyperboria.net/\n"
           "  for information about how to meet new people and make connect to them.\n"
           "\n"
           "Step 3:\n"
           "  Fire it up!\n"
           "    sudo %s < cjdroute.conf\n"
           "\n"
           "For more information about other functions and non-standard setups, see README.md\n",
           appName, appName, appName);

    return 0;
}

static int benchmark()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);
    CryptoAuth_benchmark(base, logger, alloc);
    return 0;
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
        Except_raise(eh, -1, "Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     addr->bytes);
    }
    struct AdminClient* adminClient =
        AdminClient_new(&pingAddrStorage.addr, password, base, logger, alloc);

    // 100 milliseconds is plenty to wait for a process to respond on the same machine.
    adminClient->millisecondsToWait = 100;

    Dict* pingArgs = Dict_new(alloc);

    struct AdminClient_Result* pingResult =
        AdminClient_rpcCall(String_new("ping", alloc), pingArgs, adminClient, alloc);

    if (pingResult->err == AdminClient_Error_NONE) {
        Except_raise(eh, -1, "Startup failed: cjdroute is already running.");
    }
    Allocator_free(alloc);
}

int main(int argc, char** argv)
{
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif

    if (isatty(STDIN_FILENO) || argc < 2) {
        // Fall through.
    } else if (!strcmp("angel", argv[1])) {
        return AngelInit_main(argc, argv);
    } else if (!strcmp("core", argv[1])) {
        return Core_main(argc, argv);
    }

    Assert_true(argc > 0);
    struct Except* eh = NULL;

    // Allow it to allocate 4MB
    struct Allocator* allocator = MallocAllocator_new(1<<22);
    struct Random* rand = Random_new(allocator, NULL, eh);
    struct EventBase* eventBase = EventBase_new(allocator);

    if (argc == 2) {
        // one argument
        if (strcmp(argv[1], "--help") == 0) {
            return usage(argv[0]);
        } else if (strcmp(argv[1], "--genconf") == 0) {
            return genconf(rand);
        } else if (strcmp(argv[1], "--pidfile") == 0) {
            // deprecated
            return 0;
        } else if (strcmp(argv[1], "--reconf") == 0) {
            // Performed after reading the configuration
        } else if (strcmp(argv[1], "--bench") == 0) {
            return benchmark();
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("Cjdns Git Version ID: %s\n", Version_gitVersion());
            return 0;
        } else if (strcmp(argv[1], "--cleanconf") == 0) {
            // Performed after reading configuration
        } else {
            fprintf(stderr, "%s: unrecognized option '%s'\n", argv[0], argv[1]);
            fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
            return -1;
        }
    } else if (argc >  2) {
        // more than one argument?
        fprintf(stderr, "%s: too many arguments\n", argv[0]);
        fprintf(stderr, "Try `%s --help' for more information.\n", argv[0]);
        return -1;
    }

    if (isatty(STDIN_FILENO)) {
        // We were started from a terminal
        // The chances an user wants to type in a configuration
        // bij hand are pretty slim so we show him the usage
        return usage(argv[0]);
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

    if (argc == 2 && strcmp(argv[1], "--cleanconf") == 0) {
        struct Writer* stdoutWriter = FileWriter_new(stdout, allocator);
        JsonBencSerializer_get()->serializeDictionary(stdoutWriter, &config);
        printf("\n");
        return 0;
    }

    struct Writer* logWriter = FileWriter_new(stdout, allocator);
    struct Log* logger = WriterLog_new(logWriter, allocator);

    // --------------------- Get Admin  --------------------- //
    Dict* configAdmin = Dict_getDict(&config, String_CONST("admin"));
    String* adminPass = Dict_getString(configAdmin, String_CONST("password"));
    String* adminBind = Dict_getString(configAdmin, String_CONST("bind"));
    if (!adminPass) {
        adminPass = String_newBinary(NULL, 32, allocator);
        Random_base32(rand, (uint8_t*) adminPass->bytes, 32);
        adminPass->len = strlen(adminPass->bytes);
    }
    if (!adminBind) {
        Except_raise(eh, -1, "You must specify admin.bind in the cjdroute.conf file.");
    }

    // --------------------- Check for running instance  --------------------- //

    Log_info(logger, "Checking for running instance...");
    checkRunningInstance(allocator, eventBase, adminBind, adminPass, logger, eh);

    // --------------------- Setup Pipes to Angel --------------------- //
    char angelPipeName[64] = "client-angel-";
    Random_base32(rand, (uint8_t*)angelPipeName+13, 31);
    Assert_true(EventBase_eventCount(eventBase) == 0);
    struct Pipe* angelPipe = Pipe_named(angelPipeName, eventBase, eh, allocator);
    Assert_true(EventBase_eventCount(eventBase) == 2);
    angelPipe->logger = logger;

    char* args[] = { "angel", angelPipeName, NULL };

    // --------------------- Spawn Angel --------------------- //
    String* privateKey = Dict_getString(&config, String_CONST("privateKey"));

    char* corePath = Process_getPath(allocator);

    if (!corePath) {
        Except_raise(eh, -1, "Can't find a usable cjdns core executable, "
                             "make sure it is in the same directory as cjdroute");
    }

    if (!privateKey) {
        Except_raise(eh, -1, "Need to specify privateKey.");
    }
    Log_info(logger, "Forking angel to background.");
    Process_spawn(corePath, args, eventBase, allocator);

    // --------------------- Get user for angel to setuid() ---------------------- //
    String* securityUser = NULL;
    List* securityConf = Dict_getList(&config, String_CONST("security"));
    for (int i = 0; i < List_size(securityConf); i++) {
        securityUser = Dict_getString(List_getDict(securityConf, i), String_CONST("setuser"));
        if (securityUser) {
            int64_t* ea = Dict_getInt(List_getDict(securityConf, i), String_CONST("exemptAngel"));
            if (ea && *ea) {
                securityUser = NULL;
            }
            break;
        }
    }

    // --------------------- Pre-Configure Angel ------------------------- //
    Dict* preConf = Dict_new(allocator);
    Dict* adminPreConf = Dict_new(allocator);
    Dict_putDict(preConf, String_CONST("admin"), adminPreConf, allocator);
    Dict_putString(adminPreConf, String_CONST("core"), String_new(corePath, allocator), allocator);
    Dict_putString(preConf, String_CONST("privateKey"), privateKey, allocator);
    Dict_putString(adminPreConf, String_CONST("bind"), adminBind, allocator);
    Dict_putString(adminPreConf, String_CONST("pass"), adminPass, allocator);
    if (securityUser) {
        Dict_putString(adminPreConf, String_CONST("user"), securityUser, allocator);
    }
    Dict* logging = Dict_getDict(&config, String_CONST("logging"));
    if (logging) {
        Dict_putDict(preConf, String_CONST("logging"), logging, allocator);
    }

    #define CONFIG_BUFF_SIZE 1024
    uint8_t buff[CONFIG_BUFF_SIZE] = {0};
    struct Writer* toAngelWriter = ArrayWriter_new(buff, CONFIG_BUFF_SIZE - 1, allocator);
    if (StandardBencSerializer_get()->serializeDictionary(toAngelWriter, preConf)) {
        Except_raise(eh, -1, "Failed to serialize pre-configuration");
    }
    struct Message* toAngelMsg = &(struct Message) {
        .bytes = buff,
        .length = toAngelWriter->bytesWritten
    };
    toAngelMsg = Message_clone(toAngelMsg, allocator);
    Interface_sendMessage(&angelPipe->iface, toAngelMsg);

    Log_keys(logger, "Sent [%s] to angel process.", buff);

    // --------------------- Get Response from Angel --------------------- //

    struct Message* fromAngelMsg =
        InterfaceWaiter_waitForData(&angelPipe->iface, eventBase, allocator, eh);
    Dict responseFromAngel;
    struct Reader* responseFromAngelReader =
        ArrayReader_new(fromAngelMsg->bytes, fromAngelMsg->length, allocator);
    if (StandardBencSerializer_get()->parseDictionary(responseFromAngelReader,
                                                      allocator,
                                                      &responseFromAngel))
    {
        Except_raise(eh, -1, "Failed to parse pre-configuration response [%s]", buff);
    }

    // --------------------- Get Admin Addr/Port/Passwd --------------------- //
    Dict* responseFromAngelAdmin = Dict_getDict(&responseFromAngel, String_CONST("admin"));
    adminBind = Dict_getString(responseFromAngelAdmin, String_CONST("bind"));

    if (!adminBind) {
        Except_raise(eh, -1, "didn't get address and port back from angel");
    }
    struct Sockaddr_storage adminAddr;
    if (Sockaddr_parse(adminBind->bytes, &adminAddr)) {
        Except_raise(eh, -1, "Unable to parse [%s] as an ip address port, eg: 127.0.0.1:11234",
                     adminBind->bytes);
    }

    // sanity check, Pipe_named() creates 2 events, see above.
    Assert_true(EventBase_eventCount(eventBase) == 2);

    // --------------------- Configuration ------------------------- //
    Configurator_config(&config,
                        &adminAddr.addr,
                        adminPass,
                        eventBase,
                        logger,
                        allocator);

    // --------------------- noBackground ------------------------ //

    int64_t* noBackground = Dict_getInt(&config, String_CONST("noBackground"));
    if (noBackground && *noBackground) {
        EventBase_beginLoop(eventBase);
    }

    //Allocator_free(allocator);
    return 0;
}
