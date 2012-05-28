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
#include "dht/CJDHTConstants.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "exception/ExceptionHandler.h"
#include "exception/AbortHandler.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/TUNConfigurator.h"
#include "interface/UDPInterface.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/json/JsonBencSerializer.h"
#include "util/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
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

struct Context
{
    struct event_base* base;

    struct Allocator* allocator;

    struct ExceptionHandler* eHandler;

    struct CryptoAuth* ca;

    /** The TUN/TAP which the user will use to connect to the network. */
    struct Interface* tunInterface;

    struct SwitchCore* switchCore;

    struct DHTModuleRegistry* registry;

    struct RouterModule* routerModule;

    struct Log* logger;

    struct Admin* admin;
};

struct UDPInterfaceContext
{
    struct Context* context;
    struct UDPInterface* udpContext;
};

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

    uint16_t port;
    randombytes((uint8_t*) &port, 2);
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

static void reconf(struct Context* ctx, Dict* mainConf)
{
    Dict* adminConf = Dict_getDict(mainConf, String_CONST("admin"));
    String* address = Dict_getString(adminConf, String_CONST("bind"));
    String* password = Dict_getString(adminConf, String_CONST("password"));

    if (!(address && password)) {
        Log_critical(ctx->logger, "Can't get the admin address and password from conf file.");
        exit(-1);
    }

    struct sockaddr_storage addr;
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    int addrLen = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(address->bytes, (struct sockaddr*) &addr, &addrLen)) {
        Log_critical(ctx->logger, "Unable to parse [%s] as an ip address port, "
                                   "eg: 127.0.0.1:11234", address->bytes);
        exit(-1);
    }

    Configurator_config(mainConf, &addr, addrLen, password, ctx->base, ctx->logger, ctx->allocator);
}

static void registerRouter(Dict* config, struct Address *addr, struct Context* context)
{
    Dict* iface = Dict_getDict(config, String_CONST("interface"));
    if (String_equals(Dict_getString(iface, String_CONST("type")), String_CONST("TUNInterface"))) {
        String* tunPath = Dict_getString(iface, String_CONST("tunDevice"));
        struct TUNInterface* tun = TUNInterface_new(tunPath, context->base, context->allocator);
        if (TUNConfigurator_configure(tun, addr->ip6.bytes, 8) != 0) {
            fprintf(stderr, "Couldn't configure TUN interface\n");
        }
        context->tunInterface = TUNInterface_asGeneric(tun);
    }
    context->routerModule = RouterModule_register(context->registry,
                                                  context->allocator,
                                                  addr->key,
                                                  context->base,
                                                  context->logger,
                                                  context->admin);

    RouterModule_admin_register(context->routerModule, context->admin, context->allocator);
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
    Dict d = Dict_CONST(CJDHTConstants_QUERY, String_OBJ(String_CONST("pong")), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

static void adminMemory(Dict* input, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;
    Dict d = Dict_CONST(
        String_CONST("bytes"), Int_OBJ(MallocAllocator_bytesAllocated(context->allocator)), NULL
    );
    Admin_sendMessage(&d, txid, context->admin);
}

static void admin(Dict* mainConf, char* user, struct Log* logger, struct Context* context)
{
    Dict* adminConf = Dict_getDict(mainConf, String_CONST("admin"));
    String* address = Dict_getString(adminConf, String_CONST("bind"));
    String* password = Dict_getString(adminConf, String_CONST("password"));

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));
    if (address) {
        if (evutil_parse_sockaddr_port(address->bytes, (struct sockaddr*) &addr, &addrLen)) {
            Log_critical(logger, "Unable to parse [%s] as an ip address port, "
                                  "eg: 127.0.0.1:11234", address->bytes);
            exit(-1);
        }
    }

    if (!password) {
        uint8_t buff[32];
        randomBase32(buff);
        password = String_new((char*)buff, context->allocator);
    }

    context->admin = Admin_new(&addr,
                               addrLen,
                               password,
                               user,
                               context->base,
                               context->eHandler,
                               context->logger,
                               context->allocator);

    // AuthorizedPasswords_get()
    AuthorizedPasswords_init(context->admin, context->ca, context->allocator);
    Admin_registerFunction("ping", adminPing, context->admin, false, NULL, context->admin);
    Admin_registerFunction("memory", adminMemory, context, false, NULL, context->admin);
}

static void pidfile(Dict* config)
{
    String* pidFile = Dict_getString(config, String_CONST("pidFile"));
    if (pidFile) {
        printf("%s", pidFile->bytes);
    }
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

    if (argc == 2) { // one argument
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
    } else if (argc >  2) { // more than one argument?
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

    struct Context context;
    memset(&context, 0, sizeof(struct Context));
    context.base = event_base_new();

    // Allow it to allocate 4MB
    context.allocator = MallocAllocator_new(1<<22);
    struct Reader* reader = FileReader_new(stdin, context.allocator);
    Dict config;
    if (JsonBencSerializer_get()->parseDictionary(reader, context.allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    if (argc == 2 && strcmp(argv[1], "--pidfile") == 0) {
        pidfile(&config);
        return 0;
    }

    // Logging.
    struct Writer* logwriter = FileWriter_new(stdout, context.allocator);
    struct Log logger = { .writer = logwriter };
    context.logger = &logger;

    if (argc == 2 && strcmp(argv[1], "--reconf") == 0) {
        reconf(&context, &config);
        return 0;
    }

    // ca, needed for admin.
    struct Address myAddr;
    uint8_t privateKey[32];
    parsePrivateKey(&config, &myAddr, privateKey);
    context.ca =
        CryptoAuth_new(&config, context.allocator, privateKey, context.base, context.logger);

    // Admin
    char* user = setUser(Dict_getList(&config, String_CONST("security")));
    admin(&config, user, &logger, &context);

    context.eHandler = AbortHandler_INSTANCE;
    context.switchCore = SwitchCore_new(context.logger, context.allocator);
    context.registry = DHTModuleRegistry_new(context.allocator);
    ReplyModule_register(context.registry, context.allocator);

    // Router
    Dict* routerConf = Dict_getDict(&config, String_CONST("router"));
    registerRouter(routerConf, &myAddr, &context);

    SerializationModule_register(context.registry, context.allocator);

    struct Ducttape* dt = Ducttape_register(&config,
                                            privateKey,
                                            context.registry,
                                            context.routerModule,
                                            context.tunInterface,
                                            context.switchCore,
                                            context.base,
                                            context.allocator,
                                            context.logger,
                                            context.admin);

    struct SwitchPinger* sp =
        SwitchPinger_new(&dt->switchPingerIf, context.base, context.logger, context.allocator);
    SwitchPinger_admin_register(sp, context.admin, context.allocator);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(context.ca,
                                       context.switchCore,
                                       context.routerModule,
                                       context.logger,
                                       context.base,
                                       sp,
                                       context.allocator);

    Dict* interfaces = Dict_getDict(&config, String_CONST("interfaces"));

    Dict* udpConf = Dict_getDict(interfaces, String_CONST("UDPInterface"));
    if (udpConf) {
        String* bindStr = Dict_getString(udpConf, String_CONST("bind"));
        UDPInterface_new(context.base,
                         (bindStr) ? bindStr->bytes : NULL,
                         context.allocator,
                         context.eHandler,
                         context.logger,
                         ifController,
                         context.admin);
    }

    if (udpConf == NULL) {
        fprintf(stderr, "No interfaces configured to connect to.\n");
        return -1;
    }

    // pid file
    String* pidFile = Dict_getString(&config, String_CONST("pidFile"));
    if (pidFile) {
        Log_info(context.logger, "Writing pid of process to [%s].\n", pidFile->bytes);
        FILE* pf = fopen(pidFile->bytes, "w");
        if (!pf) {
            Log_critical(context.logger,
                          "Failed to open pid file [%s] for writing, errno=%d\n",
                          pidFile->bytes,
                          errno);
            return -1;
        }
        fprintf(pf, "%d", getpid());
        fclose(pf);
    }

    struct sockaddr_storage* adminAddr;
    int adminAddrLen;
    String* adminPassword;
    Admin_getConnectInfo(&adminAddr, &adminAddrLen, &adminPassword, context.admin);
    Configurator_config(&config,
                        adminAddr,
                        adminAddrLen,
                        adminPassword,
                        context.base,
                        &logger,
                        context.allocator);

    uint8_t address[53];
    Base32_encode(address, 53, myAddr.key, 32);
    Log_info(context.logger, "Your address is: %s.k\n", address);
    uint8_t myIp[40];
    Address_printIp(myIp, &myAddr);
    Log_info(context.logger, "Your IPv6 address is: %s\n", myIp);

    // Security.
    security(Dict_getList(&config, String_CONST("security")), context.logger, context.eHandler);

    event_base_loop(context.base, 0);
abort();
    // Never reached.
    return 0;
}
