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
#include "admin/Admin.h"
#include "admin/AuthorizedPasswords.h"
#include "admin/Configurator.h"
#include "benc/Int.h"
#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "crypto/CryptoAuth_benchmark.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "exception/ExceptionHandler.h"
#include "exception/AbortHandler.h"
#include "exception/Jmp.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/TUNConfigurator.h"
#include "interface/UDPInterface_admin.h"
#include "interface/ETHInterface_admin.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "util/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "net/Ducttape.h"
#include "net/DefaultInterfaceController.h"
#include "net/SwitchPinger.h"
#include "net/SwitchPinger_admin.h"
#include "switch/SwitchCore.h"
#include "util/Base32.h"
#include "util/Hex.h"
#include "util/Security.h"

#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include "util/Assert.h"
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#define DEFAULT_TUN_DEV "cjdroute0"

/**
 * The worst possible packet overhead.
 * assuming the packet needs to be handed off to another node
 * because we have no route to the destination.
 * and the CryptoAuths to both the destination and the handoff node are both timed out.
 */
#define WORST_CASE_OVERHEAD ( \
    /* TODO: Headers_IPv4_SIZE */ 20 \
    + Headers_UDPHeader_SIZE \
    + 4 /* Nonce */ \
    + 16 /* Poly1305 authenticator */ \
    + Headers_SwitchHeader_SIZE \
    + Headers_CryptoAuth_SIZE \
    + Headers_IP6Header_SIZE \
    + Headers_CryptoAuth_SIZE \
)

/** The default MTU, assuming the external MTU is 1492 (common for PPPoE DSL) */
#define DEFAULT_MTU ( \
    1492 \
  - WORST_CASE_OVERHEAD \
  + Headers_IP6Header_SIZE /* The OS subtracts the IP6 header. */ \
  + Headers_CryptoAuth_SIZE /* Linux won't let set the MTU below 1280.
  TODO: make sure we never hand off to a node for which the CA session is expired. */ \
)

struct User
{
    uint64_t trust;
};

static int genAddress(uint8_t addressOut[40],
                      uint8_t privateKeyHexOut[65],
                      uint8_t publicKeyBase32Out[53])
{
    struct Address address;
    uint8_t privateKey[32];

    for (;;) {
        randombytes(privateKey, 32);
        crypto_scalarmult_curve25519_base(address.key, privateKey);
        AddressCalc_addressForPublicKey(address.ip6.bytes, address.key);
        // Brute force for keys until one matches FC00:/8
        if (address.ip6.bytes[0] == 0xFC) {
            Hex_encode(privateKeyHexOut, 65, privateKey, 32);
            Base32_encode(publicKeyBase32Out, 53, address.key, 32);
            Address_printIp(addressOut, &address);
            return 0;
        }
    }
}

static void randomBase32(uint8_t output[32])
{
    uint8_t bin[16];
    randombytes(bin, 16);
    Base32_encode(output, 32, bin, 16);
}

static int genconf()
{
    uint8_t password[32];
    randomBase32(password);

    uint8_t adminPassword[32];
    randomBase32(adminPassword);

    uint16_t port = 0;
    while (port <= 1024) {
        randombytes((uint8_t*) &port, 2);
    }

    uint8_t publicKeyBase32[53];
    uint8_t address[40];
    uint8_t privateKeyHex[65];
    genAddress(address, privateKeyHex, publicKeyBase32);

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
           "        {\n"
           "            // A unique string which is known to the client and server.\n"
           "            \"password\": \"%s\"\n", password);
    printf("        }\n"
           "\n"
           "        /* These are your connection credentials\n"
           "           for people connecting to you with your default password.\n"
           "           adding more passwords for different users is advisable\n"
           "           so that leaks can be isolated.\n"
           "\n"
           "            \"your.external.ip.goes.here:%u\":\n", port);
    printf("            {\n"
           "                \"password\": \"%s\",\n", password);
    printf("                \"publicKey\": \"%s.k\"\n", publicKeyBase32);
    printf("            }\n"
           "        */\n"
           "    ],\n"
           "\n"
           "    // Settings for administering and extracting information from your router.\n"
           "    // This interface provides functions which can be called through a TCP socket.\n"
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
           "        {\n"
           "            // Bind to this port.\n"
           "            \"bind\": \"0.0.0.0:%u\",\n", port);
    printf("\n"
           "            // Nodes to connect to.\n"
           "            \"connectTo\":\n"
           "            {\n"
           "                // Add connection credentials here to join the network\n"
           "                // Ask somebody who is already connected.\n"
           "            }\n"
           "        }\n"
           "    },\n"
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
           "            // This for starting cjdroute as it's own user.\n"
           "            // *MOST USERS DON'T NEED THIS*\n"
           "            //\"tunDevice\": \"" DEFAULT_TUN_DEV "\"\n"
#endif
           "        }\n"
           "    },\n"
           "\n"
           "    // Tear down inactive CryptoAuth sessions after this number of seconds\n"
           "    // to make them more forgiving in the event that they become desynchronized.\n"
           "    \"resetAfterInactivitySeconds\": 30,\n"
           "\n"
           "    // Save the pid of the running process to this file.\n"
           "    // If this file cannot be opened for writing, the router will not start.\n"
           "    //\"pidFile\": \"cjdroute.pid\",\n"
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
           "        {\"setuser\": \"nobody\"}\n"
           "     ],\n"
           "\n"
           "    // Version of the config file, used internally for migration.\n"
           "    \"version\": 1\n"
           "}\n");

    return 0;
}

static void parsePrivateKey(Dict* config, struct Address* addr, uint8_t privateKey[32])
{
    String* privateKeyStr = Dict_getString(config, String_CONST("privateKey"));
    if (privateKeyStr == NULL) {
        fprintf(stderr, "Could not extract private key from configuration.\n");
    } else if (privateKeyStr->len != 64) {
        fprintf(stderr, "Private key is not 32 bytes long.\n");
    } else if (Hex_decode(privateKey, 32, (uint8_t*)privateKeyStr->bytes, 64) != 32) {
        fprintf(stderr, "Failed to parse private key.\n");
    } else {
        crypto_scalarmult_curve25519_base(addr->key, privateKey);
        AddressCalc_addressForPublicKey(addr->ip6.bytes, addr->key);
        if (addr->ip6.bytes[0] != 0xFC) {
            fprintf(stderr, "Ip address is outside of the FC00/8 range, "
                            "invalid private key.\n");
        } else {
            return;
        }
    }
    exit(-1);
}

static int usage(char* appName)
{
    printf("Usage: %s [--help] [--genconf] [--bench] [--version]\n"
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

static void reconf(struct event_base* eventbase,
                   Dict* mainConf,
                   struct Log* logger,
                   struct Allocator* alloc)
{
    Dict* adminConf = Dict_getDict(mainConf, String_CONST("admin"));
    String* address = Dict_getString(adminConf, String_CONST("bind"));
    String* password = Dict_getString(adminConf, String_CONST("password"));

    if (!(address && password)) {
        Log_critical(logger, "Can't get the admin address and password from conf file.");
        exit(-1);
    }

    struct sockaddr_storage addr;
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    int addrLen = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(address->bytes, (struct sockaddr*) &addr, &addrLen)) {
        Log_critical(logger, "Unable to parse [%s] as an ip address port, "
                             "eg: 127.0.0.1:11234", address->bytes);
        exit(-1);
    }

    Configurator_config(mainConf, &addr, addrLen, password, eventbase, logger, alloc);
}

static char* setUser(List* config)
{
    for (int i = 0; i < List_size(config); i++) {
        Dict* d = List_getDict(config, i);
        if (d) {
            String* uname = Dict_getString(d, String_CONST("setuser"));
            if (uname) {
                return uname->bytes;
            }
        }
    }
    return NULL;
}

static void security(List* config, struct Log* logger, struct ExceptionHandler* eh)
{
    if (!config) {
        return;
    }
    bool nofiles = false;
    for (int i = 0; i < List_size(config); i++) {
        String* s = List_getString(config, i);
        if (s && String_equals(s, String_CONST("nofiles"))) {
            nofiles = true;
        }
    }
    char* user = setUser(config);
    if (user) {
        Log_info(logger, "Changing user to [%s]\n", user);
        Security_setUser(user, logger, eh);
    }
    if (nofiles) {
        Log_info(logger, "Setting max open files to zero.\n");
        Security_noFiles(eh);
    }
}

static void adminPing(Dict* input, void* vadmin, String* txid)
{
    Dict d = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("pong")), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

struct MemoryContext
{
    struct Allocator* allocator;
    struct Admin* admin;
};

static void adminMemory(Dict* input, void* vcontext, String* txid)
{
    struct MemoryContext* context = vcontext;
    Dict d = Dict_CONST(
        String_CONST("bytes"), Int_OBJ(MallocAllocator_bytesAllocated(context->allocator)), NULL
    );
    Admin_sendMessage(&d, txid, context->admin);
}

static int benchmark()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct event_base* base = event_base_new();
    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log logger = { .writer = logwriter };
    CryptoAuth_benchmark(base, &logger, alloc);
    return 0;
}

int main(int argc, char** argv)
{
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif
    Crypto_init();
    Assert_true(argc > 0);

    if (argc == 2) {
        // one argument
        if (strcmp(argv[1], "--help") == 0) {
            return usage(argv[0]);
        } else if (strcmp(argv[1], "--genconf") == 0) {
            return genconf();
        } else if (strcmp(argv[1], "--pidfile") == 0) {
            // Performed after reading the configuration
        } else if (strcmp(argv[1], "--reconf") == 0) {
            // Performed after reading the configuration
        } else if (strcmp(argv[1], "--bench") == 0) {
            return benchmark();
        } else if (strcmp(argv[1], "--version") == 0) {
            printf("Version ID: %s\n", RouterModule_gitVersion());
            return 0;
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

    struct event_base* eventBase = event_base_new();

    // Allow it to allocate 4MB
    struct Allocator* allocator = MallocAllocator_new(1<<22);
    struct Reader* reader = FileReader_new(stdin, allocator);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(reader, allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    // Logging. TODO: admin based logging.
    struct Writer* logwriter = FileWriter_new(stdout, allocator);
    struct Log* logger = &(struct Log) { .writer = logwriter };


    // pid file
    String* pidFile = Dict_getString(&config, String_CONST("pidFile"));
    if (pidFile) {
        if (argc == 2 && strcmp(argv[1], "--pidfile") == 0) {
            printf("%s", pidFile->bytes);
            return 0;
        }
        Log_info(logger, "Writing pid of process to [%s].\n", pidFile->bytes);
        FILE* pf = fopen(pidFile->bytes, "w");
        if (!pf) {
            Log_critical(logger,
                          "Failed to open pid file [%s] for writing, errno=%d\n",
                          pidFile->bytes,
                          errno);
            return -1;
        }
        fprintf(pf, "%d", (int) getpid());
        fclose(pf);
    }

    // re-configure
    if (argc == 2 && strcmp(argv[1], "--reconf") == 0) {
        reconf(eventBase, &config, logger, allocator);
        return 0;
    }

    // Admin -------------------------------------------------------------------------------------
    char* user = setUser(Dict_getList(&config, String_CONST("security")));

    Dict* adminConf = Dict_getDict(&config, String_CONST("admin"));
    String* adminAddress = Dict_getString(adminConf, String_CONST("bind"));
    String* password = Dict_getString(adminConf, String_CONST("password"));

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    if (adminAddress) {
        if (evutil_parse_sockaddr_port(adminAddress->bytes, (struct sockaddr*) &addr, &addrLen)) {
            Log_critical(logger, "Unable to parse [%s] as an ip address port, "
                                  "eg: 127.0.0.1:11234", adminAddress->bytes);
            exit(-1);
        }
    }
    struct Admin* admin = Admin_new(&addr,
                                    addrLen,
                                    password,
                                    user,
                                    eventBase,
                                    AbortHandler_INSTANCE,
                                    logger,
                                    allocator);

    // CryptoAuth ---------------------------------------------------------------------
    struct Address myAddr;
    uint8_t privateKey[32];
    parsePrivateKey(&config, &myAddr, privateKey);
    struct CryptoAuth* cryptoAuth =
        CryptoAuth_new(allocator, privateKey, eventBase, logger);


    struct SwitchCore* switchCore = SwitchCore_new(logger, allocator);
    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(allocator);
    ReplyModule_register(registry, allocator);

    // Router
    struct RouterModule* router = RouterModule_register(registry,
                                                        allocator,
                                                        myAddr.key,
                                                        eventBase,
                                                        logger,
                                                        admin);

    SerializationModule_register(registry, allocator);

    struct Ducttape* dt = Ducttape_register(privateKey,
                                            registry,
                                            router,
                                            switchCore,
                                            eventBase,
                                            allocator,
                                            logger,
                                            admin);

    struct SwitchPinger* sp =
        SwitchPinger_new(&dt->switchPingerIf, eventBase, logger, allocator);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(cryptoAuth,
                                       switchCore,
                                       router,
                                       logger,
                                       eventBase,
                                       sp,
                                       allocator);


    // ------------------- Register RPC functions ----------------------- //
    SwitchPinger_admin_register(sp, admin, allocator);
    UDPInterface_admin_register(eventBase, allocator, logger, admin, ifController);
    RouterModule_admin_register(router, admin, allocator);
    AuthorizedPasswords_init(admin, cryptoAuth, allocator);
    Admin_registerFunction("ping", adminPing, admin, false, NULL, admin);

    struct MemoryContext* mc =
        allocator->clone(sizeof(struct MemoryContext), allocator,
            &(struct MemoryContext) {
                .allocator = allocator,
                .admin = admin
            });
    Admin_registerFunction("memory", adminMemory, mc, false, NULL, admin);


    // --------------------- Configuration ------------------------- //
    struct sockaddr_storage* adminAddr;
    int adminAddrLen;
    String* adminPassword;
    Admin_getConnectInfo(&adminAddr, &adminAddrLen, &adminPassword, admin);
    Configurator_config(&config,
                        adminAddr,
                        adminAddrLen,
                        adminPassword,
                        eventBase,
                        logger,
                        allocator);

    // ---------------------- Legacy Config ----------------------- //

    Dict* routerConf = Dict_getDict(&config, String_CONST("router"));
    Dict* iface = Dict_getDict(routerConf, String_CONST("interface"));
    if (String_equals(Dict_getString(iface, String_CONST("type")), String_CONST("TUNInterface"))) {
        String* ifName = Dict_getString(iface, String_CONST("tunDevice"));

        char assignedTunName[TUNConfigurator_IFNAMSIZ];
        void* tunPtr = TUNConfigurator_initTun(((ifName) ? ifName->bytes : NULL),
                                               assignedTunName,
                                               logger,
                                               AbortHandler_INSTANCE);
        struct Jmp jmp;

        Jmp_try(jmp) {
            TUNConfigurator_setIpAddress(
                assignedTunName, myAddr.ip6.bytes, 8, logger, &jmp.handler);
        } Jmp_catch {
            Log_warn(logger, "Unable to configure ip address [%s]", jmp.message);
        }

        Jmp_try(jmp) {
            TUNConfigurator_setMTU(
                assignedTunName, DEFAULT_MTU, logger, &jmp.handler);
        } Jmp_catch {
            Log_warn(logger, "Unable to set MTU [%s], skipping.", jmp.message);
        }

        struct TUNInterface* tun = TUNInterface_new(tunPtr, eventBase, allocator);
        Ducttape_setUserInterface(dt, &tun->iface);
    }


    uint8_t address[53];
    Base32_encode(address, 53, myAddr.key, 32);
    Log_info(logger, "Your address is: %s.k\n", address);
    uint8_t myIp[40];
    Address_printIp(myIp, &myAddr);
    Log_info(logger, "Your IPv6 address is: %s\n", myIp);

    // Security.
    security(Dict_getList(&config, String_CONST("security")), logger, AbortHandler_INSTANCE);

    event_base_loop(eventBase, 0);
abort();
    // Never reached.
    return 0;
}
