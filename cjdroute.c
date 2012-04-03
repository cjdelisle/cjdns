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
#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "dht/CJDHTConstants.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/Ducttape.h"
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
#include "switch/SwitchCore.h"
#include "util/Base32.h"
#include "util/Hex.h"
#include "util/Security.h"

#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

#define DEFAULT_TUN_DEV "tun0"

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

    printf("{\n"
           "    // Private key:\n"
           "    // This key corresponds to the public key: %s.k\n", publicKeyBase32);
    printf("    // And the ipv6 address: %s\n", address);
    printf("    // Your confidentiality and data integrity depend on this key, keep it secret!\n"
           "    //\n"
           "    \"privateKey\": \"%s\",\n", privateKeyHex);
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
           "    // This interface provides API functions which can be called through a TCP socket.\n"
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

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })
static void parsePrivateKey(Dict* config, struct Address* addr, uint8_t privateKey[32])
{
    String* privateKeyStr = Dict_getString(config, BSTR("privateKey"));
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
    printf("Usage: %s [--help] [--genconf] [--getcmds]\n"
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

static int getcmds(Dict* config)
{
#ifdef __APPLE__
    printf("# The Apple implementation does not support persistent tunnels.\n"
           "# Therefor this is not necessary.\n"
           "# Just start the router and it will configure automatically.\n");

    return 0;

#else

    uint8_t privateKey[32];
    struct Address addr;
    parsePrivateKey(config, &addr, privateKey);

    uint8_t myIp[40];
    Address_printIp(myIp, &addr);

    Dict* router = Dict_getDict(config, BSTR("router"));
    Dict* iface = Dict_getDict(router, BSTR("interface"));
    String* type = Dict_getString(iface, BSTR("type"));
    String* tunDevice = Dict_getString(iface, BSTR("tunDevice"));
    if (!String_equals(type, BSTR("TUNInterface"))) {
        fprintf(stderr, "router.interface.type is not recognized.\n");
        return -1;
    }
    char *tunDev = (tunDevice) ? tunDevice->bytes : DEFAULT_TUN_DEV;
    if (strrchr(tunDev, '/') != NULL) {
        tunDev = strrchr(tunDev, '/') + 1;
    }

    printf("#!/bin/bash\n"
           "# This is for starting cjdroute as it's own user. see README.md\n"
           "# *MOST USERS DON'T NEED THIS*\n"
           "# To setup a persistent tunnel, run these commands every reboot.\n"
           "#\n"
           "# NOTE: tun devices are destroyed when a system is rebooted.\n"
           "# To create the tun device use this command.\n"
           "# replace \"cjdns\" with the name of the user which the router will run as.\n"
           "# /sbin/ip tuntap add dev %s mode tun user cjdns\n\n", tunDev);

    printf("/sbin/ip addr add %s/8 dev %s\n", myIp, tunDev);
    printf("/sbin/ip link set %s up\n", tunDev);

    return 0;

#endif /* __APPLE__ */
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
    int addrLen = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(address->bytes, (struct sockaddr*) &addr, &addrLen)) {
        Log_critical1(ctx->logger, "Unable to parse [%s] as an ip address port, "
                                   "eg: 127.0.0.1:11234", address->bytes);
        exit(-1);
    }

    Configurator_config(mainConf, &addr, addrLen, password, ctx->base, ctx->logger, ctx->allocator);
}

static uint8_t serverFirstIncoming(struct Message* msg, struct Interface* iface)
{
    struct UDPInterfaceContext* uictx = (struct UDPInterfaceContext*) iface->receiverContext;

    struct Interface* udpDefault = UDPInterface_getDefaultInterface(uictx->udpContext);
    assert(udpDefault);
    UDPInterface_bindToCurrentEndpoint(udpDefault);

    struct User* u = CryptoAuth_getUser(iface);
    assert(u);
    // Add it to the switch, this will change the receiveMessage for this interface.
    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    SwitchCore_addInterface(iface, u->trust, &addr.networkAddress_be, uictx->context->switchCore);

    uint8_t* herKey = CryptoAuth_getHerPublicKey(iface);
    memcpy(addr.key, herKey, 32);
    uint8_t printedAddr[60];
    Address_print(printedAddr, &addr);
    Log_info1(uictx->context->logger,
              "Node %s has connected to us.\n",
              printedAddr);

    // Prepare for the next connection.
    struct Interface* newUdpDefault = UDPInterface_getDefaultInterface(uictx->udpContext);
    struct Interface* newAuthedUdpDefault =
        CryptoAuth_wrapInterface(newUdpDefault, NULL, true, true, uictx->context->ca);
    newAuthedUdpDefault->receiveMessage = serverFirstIncoming;
    newAuthedUdpDefault->receiverContext = uictx;

    // Send the message on to the switch so the first message isn't lost.
    return iface->receiveMessage(msg, iface);
}

static void udpConnectTo(String* connectToAddress,
                         Dict* config,
                         struct UDPInterface* udpContext,
                         struct Context* ctx)
{
    String* password = Dict_getString(config, BSTR("password"));
    String* publicKey = Dict_getString(config, BSTR("publicKey"));

    #define FAIL_IF_NULL(cannotBeNull, fieldName) \
        if (!cannotBeNull) {                                                     \
            fprintf(stderr,                                                      \
                    "interfaces.UDPInterface['%s']." fieldName " is not set, "   \
                    "this field is mandatory.\n",                                \
                    connectToAddress->bytes);                                    \
            exit(-1);                                                            \
        }

    FAIL_IF_NULL(password, "password")
    FAIL_IF_NULL(publicKey, "publicKey")

    #undef FAIL_IF_NULL


    uint8_t pkBytes[32];
    if (publicKey->len < 52 || Base32_decode(pkBytes, 32, (uint8_t*)publicKey->bytes, 52) != 32) {
        fprintf(stderr,
                "interfaces.UDPInterface['%s'].publicKey could not be parsed.\n",
                connectToAddress->bytes);
        exit(-1);
    }
    uint8_t addressBytes[16];
    AddressCalc_addressForPublicKey(addressBytes, pkBytes);
    if (addressBytes[0] != 0xFC) {
        fprintf(stderr,
                "interfaces.UDPInterface['%s'].publicKey\n( %s )\nis not in FC00/8 range, "
                "it was probably mistranscribed.\n",
                connectToAddress->bytes,
                publicKey->bytes);
        exit(-1);
    }

    struct Interface* udp =
        UDPInterface_addEndpoint(udpContext, connectToAddress->bytes, ctx->eHandler);
    struct Interface* authedUdp = CryptoAuth_wrapInterface(udp, pkBytes, false, true, ctx->ca);
    CryptoAuth_setAuth(password, 1, authedUdp);

    uint64_t switchAddr_be;
    SwitchCore_addInterface(authedUdp, 0, &switchAddr_be, ctx->switchCore);
    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    memcpy(addr.key, pkBytes, 32);
    addr.networkAddress_be = switchAddr_be;
    RouterModule_addNode(&addr, ctx->routerModule);
}

static void configureUDP(Dict* config, struct Context* ctx)
{
    String* bindStr = Dict_getString(config, BSTR("bind"));
    char* bindAddress = bindStr ? bindStr->bytes : NULL;

    struct UDPInterface* udpContext =
        UDPInterface_new(ctx->base, bindAddress, ctx->allocator, ctx->eHandler, ctx->logger);

    if (bindStr) {
        struct Interface* udpDefault = UDPInterface_getDefaultInterface(udpContext);
        struct Interface* authedDef =
            CryptoAuth_wrapInterface(udpDefault, NULL, true, true, ctx->ca);

        struct UDPInterfaceContext* uictx =
            ctx->allocator->malloc(sizeof(struct UDPInterfaceContext), ctx->allocator);
        uictx->context = ctx;
        uictx->udpContext = udpContext;
        authedDef->receiveMessage = serverFirstIncoming;
        authedDef->receiverContext = uictx;
    }

    Dict* connectTo = Dict_getDict(config, BSTR("connectTo"));
    if (connectTo) {
        struct Dict_Entry* entry = *connectTo;
        while (entry != NULL) {
            String* key = (String*) entry->key;
            if (entry->val->type != Object_DICT) {
                fprintf(stderr,
                        "interfaces.UDPInterface.connectTo: entry %s is not a dictionary type.\n",
                        key->bytes);
                abort();
            }
            Dict* value = entry->val->as.dictionary;

            udpConnectTo(key, value, udpContext, ctx);

            entry = entry->next;
        }
    }
}

static void registerRouter(Dict* config, struct Address *addr, struct Context* context)
{
    Dict* iface = Dict_getDict(config, BSTR("interface"));
    if (String_equals(Dict_getString(iface, BSTR("type")), BSTR("TUNInterface"))) {
        String* tunPath = Dict_getString(iface, BSTR("tunDevice"));
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
}

static char* setUser(List* config)
{
    for (int i = 0; i < List_size(config); i++) {
        Dict* d = List_getDict(config, i);
        if (d) {
            String* uname = Dict_getString(d, BSTR("setuser"));
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
        if (s && String_equals(s, BSTR("nofiles"))) {
            nofiles = true;
        }
    }
    char* user = setUser(config);
    if (user) {
        Log_info1(logger, "Changing user to [%s]\n", user);
        Security_setUser(user, logger, eh);
    }
    if (nofiles) {
        Log_info(logger, "Setting max open files to zero.\n");
        Security_noFiles(eh);
    }
}

static void adminPing(Dict* input, void* vadmin, String* txid)
{
    uint8_t buffer[256];
    struct Allocator* alloc = BufferAllocator_new(buffer, 256);

    String* pong = BSTR("pong");
    Dict* d = Dict_new(alloc);
    Dict_putString(d, CJDHTConstants_QUERY, pong, alloc);

    Admin_sendMessage(d, txid, (struct Admin*) vadmin);
}

static void adminMemory(Dict* input, void* vcontext, String* txid)
{
    struct Context* context = (struct Context*) vcontext;
    uint8_t buffer[256];
    struct Allocator* alloc = BufferAllocator_new(buffer, 256);

    String* bytes = BSTR("bytes");
    Dict* d = Dict_new(alloc);
    Dict_putInt(d, bytes, MallocAllocator_bytesAllocated(context->allocator), alloc);

    Admin_sendMessage(d, txid, context->admin);
}

static void admin(Dict* mainConf, char* user, struct Log* logger, struct Context* context)
{
    Dict* adminConf = Dict_getDict(mainConf, String_CONST("admin"));
    String* address = Dict_getString(adminConf, String_CONST("bind"));
    String* password = Dict_getString(adminConf, String_CONST("password"));

    if (!password || !address) {
        uint8_t randomPass[32];
        randomBase32(randomPass);
        Log_critical1(logger, "cjdns now requires you to specify an admin port and password.\n"
                              "if you don't have an \"admin\" section in your configuration, "
                              "add this.\n"
                              "Otherwise add field so that it looks like this:\n"
                              "\n"
                              "    \"admin\": {\n"
                              "        \"bind\": \"127.0.0.1:11234\",\n"
                              "        \"password\": \"%s\"\n"
                              "    }\n", randomPass);
        exit(-1);
    }

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(address->bytes, (struct sockaddr*) &addr, &addrLen)) {
        Log_critical1(logger, "Unable to parse [%s] as an ip address port, "
                              "eg: 127.0.0.1:11234", address->bytes);
        exit(-1);
    }

    context->admin = Admin_new(&addr,
                               addrLen,
                               password,
                               user,
                               context->base,
                               context->eHandler,
                               context->allocator);

    // AuthorizedPasswords_get()
    AuthorizedPasswords_init(context->admin, context->ca, context->allocator);
    Admin_registerFunction("ping", adminPing, context->admin, false, context->admin);
    Admin_registerFunction("memory", adminMemory, context, false, context->admin);

    Configurator_config(mainConf,
                        &addr,
                        addrLen,
                        password,
                        context->base,
                        logger,
                        context->allocator);
}

static void pidfile(Dict* config)
{
    String* pidFile = Dict_getString(config, BSTR("pidFile"));
    if (pidFile) {
        printf("%s", pidFile->bytes);
    }
}

int main(int argc, char** argv)
{
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif
    Crypto_init();
    assert(argc > 0);

    if (argc == 2) { // one argument
        if (strcmp(argv[1], "--help") == 0) {
            return usage(argv[0]);
        } else if (strcmp(argv[1], "--genconf") == 0) {
            return genconf();
        } else if (strcmp(argv[1], "--getcmds") == 0) {
            // Performed after reading the configuration
        } else if (strcmp(argv[1], "--pidfile") == 0) {
            // Performed after reading the configuration
        } else if (strcmp(argv[1], "--reconf") == 0) {
            // Performed after reading the configuration
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

    if (argc == 2 && strcmp(argv[1], "--getcmds") == 0) {
        return getcmds(&config);
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
    char* user = setUser(Dict_getList(&config, BSTR("security")));
    admin(&config, user, &logger, &context);

    context.eHandler = AbortHandler_INSTANCE;
    context.switchCore = SwitchCore_new(context.logger, context.allocator);
    context.registry = DHTModules_new(context.allocator);
    ReplyModule_register(context.registry, context.allocator);

    // Router
    Dict* routerConf = Dict_getDict(&config, BSTR("router"));
    registerRouter(routerConf, &myAddr, &context);

    SerializationModule_register(context.registry, context.allocator);

    // Interfaces.
    Dict* interfaces = Dict_getDict(&config, BSTR("interfaces"));
    Dict* udpConf = Dict_getDict(interfaces, BSTR("UDPInterface"));

    if (udpConf) {
        configureUDP(udpConf, &context);
    }

    if (udpConf == NULL) {
        fprintf(stderr, "No interfaces configured to connect to.\n");
        return -1;
    }

    // pid file
    String* pidFile = Dict_getString(&config, BSTR("pidFile"));
    if (pidFile) {
        Log_info1(context.logger, "Writing pid of process to [%s].\n", pidFile->bytes);
        FILE* pf = fopen(pidFile->bytes, "w");
        if (!pf) {
            Log_critical2(context.logger,
                          "Failed to open pid file [%s] for writing, errno=%d\n",
                          pidFile->bytes,
                          errno);
            return -1;
        }
        fprintf(pf, "%d", getpid());
        fclose(pf);
    }

    Ducttape_register(&config,
                      privateKey,
                      context.registry,
                      context.routerModule,
                      context.tunInterface,
                      context.switchCore,
                      context.base,
                      context.allocator,
                      context.logger);

    uint8_t address[53];
    Base32_encode(address, 53, myAddr.key, 32);
    Log_info1(context.logger, "Your address is: %s.k\n", address);
    uint8_t myIp[40];
    Address_printIp(myIp, &myAddr);
    Log_info1(context.logger, "Your IPv6 address is: %s\n", myIp);

    // Security.
    security(Dict_getList(&config, BSTR("security")), context.logger, context.eHandler);

    event_base_loop(context.base, 0);
abort();
    // Never reached.
    return 0;
}
