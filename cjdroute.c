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
#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/Ducttape.h"
#include "exception/ExceptionHandler.h"
#include "exception/AbortHandler.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/UDPInterface.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "io/Writer.h"
#include "io/FileWriter.h"
#include "libbenc/serialization/BencSerializer.h"
#include "libbenc/serialization/json/JsonBencSerializer.h"
#include "log/Log.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
#include "memory/MemAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Base32.h"
#include "util/Hex.h"
#include "Version.h"

#include "crypto_scalarmult_curve25519.h"

#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <stdio.h>

struct Context
{
    struct event_base* base;

    struct MemAllocator* allocator;

    struct ExceptionHandler* eHandler;

    struct CryptoAuth* ca;

    struct Interface* routerIf;

    struct SwitchCore* switchCore;

    struct DHTModuleRegistry* registry;

    struct RouterModule* routerModule;

    struct Log* logger;
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

static int genconf()
{
    uint8_t publicKeyBase32[53];
    uint8_t address[40];
    uint8_t privateKeyHex[65];
    genAddress(address, privateKeyHex, publicKeyBase32);

    printf("{\n"
           "     // Private key:\n"
           "     // This key corrisponds to the public key: %s.k\n", publicKeyBase32);
    printf("     // And the ipv6 address: %s\n", address);
    printf("     // Your confidentiality and data integrity depend on this key, keep it secret!\n"
           "     //\n"
           "    \"privateKey\": \"%s\",\n", privateKeyHex);
    printf("\n"
           "     // Anyone connecting and offering these passwords on connection will be allowed.\n"
           "     //\n"
           "     // WARNING: Currently there is no key derivation done on the password field,\n"
           "     //          DO NOT USE A PASSWORD HERE use something which is truely random and\n"
           "     //          cannot be guessed.\n"
           "     // Including a username in the beginning of the password string is encouraged\n"
           "     // to aid in remembering which users are who.\n"
           "     //\n"
           "    \"authorizedPasswords\": [\n"
           "        {\n"
           "            // A unique string which is known to the client and server.\n"
           "            \"password\": \"Bob - 2Q4qAPGemxgrydSSetSmOWlE2YO8wYMSG2H1aBPolS3n\",\n"
           "\n"
           "            // the authentication type, currently only 1 is supported.\n"
           "            \"authType\": 1,\n"
           "\n"
           "            // How much anti-flood trust to give a client\n"
           "            // who connects with this password.\n"
           "            \"trust\": 5000\n"
           "        },\n"
           "\n"
           "        /* You can add as many authorized passwords as you want.\n"
           "        {\n"
           "            \"password\": \"Alice - wTNeK7nlFRn1tRfgnOkWEATkd/RFlZOQVuOsUy8ATWjD\",\n"
           "            \"authType\": 1,\n"
           "            \"trust\": 2500\n"
           "        },*/\n"
           "    ],\n"
           "\n"
           "    // Interfaces to connect to the switch core.\n"
           "    \"interfaces\":\n"
           "    {\n"
           "        // The interface which connects over UDP/IP based VPN tunnel.\n"
           "        \"UDPInterface\":\n"
           "        {\n"
           "            // Bind to this port.\n"
           "            \"bind\": \"0.0.0.0:10000\",\n"
           "\n"
           "            // Nodes to connect to.\n"
           "            \"connectTo\":\n"
           "            {\n"
           "                \"127.0.0.1:10000\":\n"
           "                {\n"
           "                    // Password to present when connecting.\n"
           "                    \"password\": \"secret\",\n"
           "\n"
           "                    // The method of authenticating, only 1 is supported for now.\n"
           "                    \"authType\": 1,\n"
           "\n"
           "                    // The public key of the node to connect to.\n"
           "                    \"publicKey\": "
           "\"y39gwfy5259s8fj4khntfy95bx6wxu5lbm2m132yx0ucrk0ruyx0.k\",\n"
           "\n"
           "                    // Anti-flood trust level.\n"
           "                    \"trust\": 9000\n"
           "                },\n"
           "                /* You may connect to as many other nodes as you want.\n"
           "                \"1.2.3.4:56789\": {\n"
           "                    \"password\": \"secret2\",\n"
           "                    \"authType\": 1,\n"
           "                    \"publicKey\": "
           "\"y39gwfy5259s8fj4khntfy95bx6wxu5lbm2m132yx0ucrk0ruyx0.k\",\n"
           "                    \"trust\": 1234\n"
           "                }\n"
           "                */\n"
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
           "            \"type\": \"TUNInterface\",\n"
           "\n"
           "            // The name of the TUN device to use.\n"
           "            // This allows you to create a persistent TUN device with the cjdns user\n"
           "            // authorized to use it so that cjdns does not need to run as root.\n"
           "            // If this is commented out, cjdns will try to allocate a TUN on startup.\n"
           "            // If it can't do that (because it's not root?) then it will run as a\n"
           "            // pure router, unable to send or receive traffic.\n"
           "            \"tunDevice\": \"tun0\"\n"
           "        }\n"
           "    }\n"
           "\n"
           "    \"resetAfterInactivitySeconds\": 20,\n"
           "\n"
           "    // Version of the config file, used internally for migration.\n"
           "    \"version\": 0\n"
           "}\n");

    return 0;
}

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })
static void parsePrivateKey(Dict* config, struct Address* addr, uint8_t privateKey[32])
{
    String* privateKeyStr = benc_lookupString(config, BSTR("privateKey"));
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
    printf("Step 1:\n"
           "  Generate a new configuration file.\n"
           "    %s --genconf > cjdroute.conf\n"
           "\n"
           "Step 2:\n"
           "  From a root shell or using sudo, run use these commands:\n"
           "\n"
           "  Create a cjdns user so it can run unprivileged.\n"
           "    useradd cjdns\n"
           "\n"
           "  Create a new TUN device and give the cjdns user authority to access it:\n"
           "    /sbin/ip tuntap add mode tun user cjdns\n"
           "    /sbin/ip tuntap list | grep `id -u cjdns`\n"
           "  The output of the last command will tell you the name of the new device.\n"
           "  This is needed to edit the configuration file.\n"
           "\n"
           "Step 3:\n"
           "  Edit the configuration file, fill in the key from the node to connect to and your\n"
           "  password as well as the bind address to listen for UDP packets on and the\n"
           "  passwords of other nodes who are allowed to connect to this node.\n"
           "  Also replace \"tunDevice\": \"tun0\" with the name of the TUN device gotten\n"
           "  in step 2\n"
           "\n"
           "Step 4:\n"
           "  Get the commands to run in order to prepare your TUN device by running:\n"
           "    %s --getcmds < cjdroute.conf\n"
           "  These commands should be executed as root now every time the system restarts.\n"
           "\n"
           "Step 5:\n"
           "  Fire it up!\n"
           "    sudo -u cjdns %s < cjdroute.conf\n"
           "\n"
           "Notes:\n"
           "  To delete a tunnel, use this command:\n"
           "    /sbin/ip tuntap del mode tun <name of tunnel>\n"
           "\n",
           appName, appName, appName);

    return 0;
}

static int getcmds(Dict* config)
{
    uint8_t privateKey[32];
    struct Address addr;
    parsePrivateKey(config, &addr, privateKey);

    uint8_t myIp[40];
    Address_printIp(myIp, &addr);

    Dict* router = benc_lookupDictionary(config, BSTR("router"));
    Dict* iface = benc_lookupDictionary(router, BSTR("interface"));
    String* type = benc_lookupString(iface, BSTR("type"));
    String* tunDevice = benc_lookupString(iface, BSTR("tunDevice"));
    if (!benc_stringEquals(type, BSTR("TUNInterface"))) {
        fprintf(stderr, "router.interface.type is not recognized.\n");
        return -1;
    }
    char* tunDev = tunDevice ? tunDevice->bytes : "tun0";
    if (strrchr(tunDev, '/') != NULL) {
        tunDev = strrchr(tunDev, '/') + 1;
    }

    printf("#!/bin/bash\n"
           "# Run these commands as root to get the interfaces setup properly.\n\n");
    printf("/sbin/ip addr add %s dev %s\n", myIp, tunDev);
    printf("/sbin/ip -6 route add fc00::/8 dev %s\n", tunDev);

    return 0;
}

static void authorizedPassword(String* passwd,
                               Integer* authType,
                               Integer* trust,
                               uint32_t index,
                               struct Context* ctx)
{
    if (!(passwd && authType && trust)) {
        fprintf(stderr,
                "authorizedPasswords[%u] is must specify authType, password, and trust.\n",
                index);
        exit(-1);
    }
    if (*authType < 1 || *authType > 255) {
        fprintf(stderr,
                "authorizedPasswords[%u] auth must be between 1 and 255 inclusive.\n",
                index);
        exit(-1);
    }
    if (*trust < 0) {
        fprintf(stderr, "authorizedPasswords[%u] trust cannot be negative.\n", index);
        exit(-1);
    }
    printf("adding authorized password.\n");
    struct User* u = ctx->allocator->malloc(sizeof(struct User), ctx->allocator);
    u->trust = (uint64_t) *trust;
    CryptoAuth_addUser(passwd, *authType, u, ctx->ca);
}

static void authorizedPasswords(List* list, struct Context* ctx)
{
    uint32_t count = benc_itemCount(list);
    for (uint32_t i = 0; i < count; i++) {
        Dict* d = benc_getDictionary(list, i);
        if (!d) {
            fprintf(stderr, "authorizedPasswords[%u] is not a dictionary type.\n", i);
            exit(-1);
        }
        String* passwd = benc_lookupString(d, BSTR("password"));
        Integer* authType = benc_lookupInteger(d, BSTR("authType"));
        Integer* trust = benc_lookupInteger(d, BSTR("trust"));
        authorizedPassword(passwd, authType, trust, i, ctx);
    }
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
    String* password = benc_lookupString(config, BSTR("password"));
    Integer* authType = benc_lookupInteger(config, BSTR("authType"));
    String* publicKey = benc_lookupString(config, BSTR("publicKey"));
    Integer* trust = benc_lookupInteger(config, BSTR("trust"));

    #define FAIL_IF_NULL(cannotBeNull, fieldName) \
        if (!cannotBeNull) {                                                     \
            fprintf(stderr,                                                      \
                    "interfaces.UDPInterface['%s']." fieldName " is not set, "   \
                    "this field is mandatory.\n",                                \
                    connectToAddress->bytes);                                    \
            exit(-1);                                                            \
        }

    FAIL_IF_NULL(password, "password")
    FAIL_IF_NULL(authType, "authType")
    FAIL_IF_NULL(publicKey, "publicKey")
    FAIL_IF_NULL(trust, "trust")

    #undef FAIL_IF_NULL

    #define CHECK_RANGE(number, min, max) \
        if (number < min || number > max) {                                           \
        fprintf(stderr,                                                               \
                "interfaces.UDPInterface['%s'].number must be between min and max\n", \
                connectToAddress->bytes);                                             \
        exit(-1);                                                                     \
    }

    CHECK_RANGE(*authType, 1, 255)
    CHECK_RANGE(*trust, 0, INT64_MAX)

    #undef CHECK_RANGE

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
    CryptoAuth_setAuth(password, *authType, authedUdp);

    uint64_t switchAddr_be;
    SwitchCore_addInterface(authedUdp, *trust, &switchAddr_be, ctx->switchCore);
    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    memcpy(addr.key, pkBytes, 32);
    addr.networkAddress_be = switchAddr_be;
    RouterModule_addNode(&addr, ctx->routerModule);
}

static void configureUDP(Dict* config, struct Context* ctx)
{
    String* bindStr = benc_lookupString(config, BSTR("bind"));
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

    Dict* connectTo = benc_lookupDictionary(config, BSTR("connectTo"));
    if (connectTo) {
        benc_dict_entry_t* entry = *connectTo;
        while (entry != NULL) {
            String* key = (String*) entry->key;
            if (entry->val->type != BENC_DICT) {
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

static void registerRouter(Dict* config, uint8_t myPubKey[32], struct Context* context)
{
    Dict* iface = benc_lookupDictionary(config, BSTR("interface"));
    if (benc_stringEquals(benc_lookupString(iface, BSTR("type")), BSTR("TUNInterface"))) {
        String* tunPath = benc_lookupString(iface, BSTR("tunDevice"));
        context->routerIf = TunInterface_new(tunPath, context->base, context->allocator);
    }
    context->routerModule = RouterModule_register(context->registry,
                                                  context->allocator,
                                                  myPubKey,
                                                  context->base,
                                                  context->logger);
}

int main(int argc, char** argv)
{
    #ifdef Log_KEYS
        fprintf(stderr, "Log_LEVEL = KEYS, EXPECT TO SEE PRIVATE KEYS IN YOUR LOGS!\n");
    #endif
    Crypto_init();
    assert(argc > 0);
    if (isatty(STDIN_FILENO)) {
        if (argc < 2) {
            return usage(argv[0]);
        }
        if (strcmp(argv[1], "--genconf") == 0) {
            return genconf();
        }
    }

    struct Context context;
    memset(&context, 0, sizeof(struct Context));

    // Allow it to allocate 4MB
    context.allocator = MallocAllocator_new(1<<22);
    struct Reader* reader = FileReader_new(stdin, context.allocator);
    Dict config;
    if (benc_getJsonBencSerializer()->parseDictionary(reader, context.allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    if (argc == 2 && strcmp(argv[1], "--getcmds") == 0) {
        return getcmds(&config);
    }

    printf("Version: " Version_STRING "\n");

    // Logging
    struct Writer* logwriter = FileWriter_new(stdout, context.allocator);
    struct Log logger = { .writer = logwriter };
    context.logger = &logger;

    struct Address myAddr;
    uint8_t privateKey[32];
    parsePrivateKey(&config, &myAddr, privateKey);

    context.eHandler = AbortHandler_INSTANCE;
    context.base = event_base_new();
    context.switchCore = SwitchCore_new(context.logger, context.allocator);
    context.ca =
        CryptoAuth_new(&config, context.allocator, privateKey, context.base, context.logger);
    context.registry = DHTModules_new(context.allocator);
    ReplyModule_register(context.registry, context.allocator);

    // Router
    Dict* routerConf = benc_lookupDictionary(&config, BSTR("router"));
    registerRouter(routerConf, myAddr.key, &context);

    SerializationModule_register(context.registry, context.allocator);

    // Authed passwords.
    List* authedPasswords = benc_lookupList(&config, BSTR("authorizedPasswords"));
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &context);
    }

    // Interfaces.
    Dict* interfaces = benc_lookupDictionary(&config, BSTR("interfaces"));
    Dict* udpConf = benc_lookupDictionary(interfaces, BSTR("UDPInterface"));

    if (udpConf) {
        configureUDP(udpConf, &context);
    }

    if (udpConf == NULL) {
        fprintf(stderr, "No interfaces configured to connect to.\n");
        return -1;
    }

    Ducttape_register(&config,
                      privateKey,
                      context.registry,
                      context.routerModule,
                      context.routerIf,
                      context.switchCore,
                      context.base,
                      context.allocator,
                      context.logger);

    uint8_t address[53];
    Base32_encode(address, 53, myAddr.key, 32);
    printf("Your address is: %s.k\n", address);

    event_base_loop(context.base, 0);
}
