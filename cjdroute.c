#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/SwitchConnectorModule.h"
#include "exception/ExceptionHandler.h"
#include "exception/AbortHandler.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/UDPInterface.h"
#include "io/Reader.h"
#include "io/FileReader.h"
#include "libbenc/serialization/BencSerializer.h"
#include "libbenc/serialization/json/JsonBencSerializer.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
#include "memory/MemAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Base32.h"
#include "util/Hex.h"

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

static int usage(char* appName)
{
    printf("Usage:\n"
           "%s --genconf > /path/to/cjdroute.conf\n"
           "Generate a new configuration file.\n"
           "%s < /path/to/cjdroute.conf\n"
           "Startup with configuration.\n",
           appName, appName);
    return 0;
}

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

    printf
    (
        "{\n"
        "     // Private key:\n"
        "     // This key corrisponds to the public key: %s.k\n" /* publicKeyBase32 */
        "     // And the ipv6 address: %s\n" /* address */
        "     // Your confidentiality and data integrity depend on this key, keep it secret!\n"
        "     //\n"
        "    \"privateKey\": \"%s\",\n" /* privateKeyHex */
        "\n"
        "     // Anyone connecting and offering these passwords on connection will be allowed in.\n"
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
        "            \"bind\": \"127.0.0.1:10001\",\n"
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
        "            // The path to the TUN device of a specific device should be used,\n"
        "            // this allows you to create a persistent TUN device with permissions set\n"
        "            // so that cjdns does not need to run as root.\n"
        "            //\"tunDevicePath\": \"/dev/net/tun0\"\n"
        "        }\n"
        "    }\n"
        "\n"
        "    // Version of the config file, used internally for migration.\n"
        "    \"version\": 0\n"
        "}\n",
        publicKeyBase32,
        address,
        privateKeyHex
    );

    return 0;
}

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })
static void parsePrivateKey(Dict* config, uint8_t myPubKey[32], uint8_t privateKey[32])
{
    String* privateKeyStr = benc_lookupString(config, BSTR("privateKey"));
    if (privateKeyStr == NULL) {
        fprintf(stderr, "Could not extract private key from configuration.\n");
    } else if (privateKeyStr->len != 64) {
        fprintf(stderr, "Private key is not 32 bytes long.\n");
    } else if (Hex_decode(privateKey, 32, (uint8_t*)privateKeyStr->bytes, 64) != 32) {
        fprintf(stderr, "Failed to parse private key.\n");
    } else {
        uint8_t ip6Addr[16];
        crypto_scalarmult_curve25519_base(myPubKey, privateKey);
        AddressCalc_addressForPublicKey(ip6Addr, myPubKey);
        if (ip6Addr[0] != 0xFC) {
            fprintf(stderr, "Ip address is out of the legal range, "
                            "please generate new private key.\n");
        } else {
            return;
        }
    }
    exit(-1);
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

static void serverFirstIncoming(struct Message* msg, struct Interface* iface)
{
    struct UDPInterfaceContext* uictx = (struct UDPInterfaceContext*) iface->receiverContext;

    struct Interface* udpDefault = UDPInterface_getDefaultInterface(uictx->udpContext);
    assert(udpDefault);
    UDPInterface_bindToCurrentEndpoint(udpDefault);

    struct User* u = CryptoAuth_getUser(iface);
    assert(u);
    // Add it to the switch.
    uint64_t discard;
    SwitchCore_addInterface(iface, u->trust, &discard, uictx->context->switchCore);

    // Prepare for the next connection.
    struct Interface* newUdpDefault = UDPInterface_getDefaultInterface(uictx->udpContext);
    struct Interface* newAuthedUdpDefault =
        CryptoAuth_wrapInterface(newUdpDefault, NULL, true, true, uictx->context->ca);
    newAuthedUdpDefault->receiveMessage = serverFirstIncoming;
    newAuthedUdpDefault->receiverContext = uictx;

    // Send the message on to the switch so the first message isn't lost.
    iface->receiveMessage(msg, iface);
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
    RouterModule_addNode(pkBytes, (uint8_t*)&switchAddr_be, ctx->routerModule);
}

static void configureUDP(Dict* config, struct Context* ctx)
{
    String* bindStr = benc_lookupString(config, BSTR("bind"));
    char* bindAddress = bindStr ? bindStr->bytes : NULL;

    struct UDPInterface* udpContext =
        UDPInterface_new(ctx->base, bindAddress, ctx->allocator, ctx->eHandler);

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
        String* tunPath = benc_lookupString(iface, BSTR("tunDevicePath"));
        context->routerIf = TunInterface_new(tunPath, context->base, context->allocator);
    }
    context->routerModule =
        RouterModule_register(context->registry, context->allocator, myPubKey, context->base);
}

int main(int argc, char** argv)
{
    Crypto_init();
    assert(argc > 0);
    if (isatty(STDIN_FILENO)) {
        if (argc == 2 && strcmp(argv[1], "--genconf") == 0) {
            return genconf();
        }
        return usage(argv[0]);
    }

    struct Context context;
    memset(&context, 0, sizeof(struct Context));

    // Allow it to allocate 2MB
    context.allocator = MallocAllocator_new(1<<21);
    struct Reader* reader = FileReader_new(stdin, context.allocator);
    Dict config;
    if (benc_getJsonBencSerializer()->parseDictionary(reader, context.allocator, &config)) {
        fprintf(stderr, "Failed to parse configuration.\n");
        return -1;
    }

    uint8_t myPubKey[32];
    uint8_t privateKey[32];
    parsePrivateKey(&config, myPubKey, privateKey);

    context.eHandler = AbortHandler_INSTANCE;
    context.base = event_base_new();
    context.switchCore = SwitchCore_new(context.allocator);
    context.ca = CryptoAuth_new(context.allocator, privateKey);
    context.registry = DHTModules_new(context.allocator);
    ReplyModule_register(context.registry, context.allocator);

    // Router
    Dict* routerConf = benc_lookupDictionary(&config, BSTR("router"));
    registerRouter(routerConf, myPubKey, &context);

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

    SwitchConnectorModule_register(privateKey,
                                   context.registry,
                                   context.routerModule,
                                   context.routerIf,
                                   context.switchCore,
                                   context.base,
                                   context.allocator);

    uint8_t address[53];
    Base32_encode(address, 53, myPubKey, 32);
    printf("Your address is: %s.k\n", address);

    event_base_loop(context.base, 0);
}
