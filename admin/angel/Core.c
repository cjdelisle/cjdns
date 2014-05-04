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
#include "admin/AdminLog.h"
#include "admin/angel/Angel.h"
#include "admin/angel/Core.h"
#include "admin/angel/InterfaceWaiter.h"
#include "admin/angel/Hermes.h"
#include "admin/AuthorizedPasswords.h"
#include "benc/Int.h"
#include "benc/serialization/BencSerializer.h"
#include "benc/serialization/standard/StandardBencSerializer.h"
#include "crypto/AddressCalc.h"
#include "crypto/random/Random.h"
#include "crypto/random/libuv/LibuvEntropyProvider.h"
#include "dht/ReplyModule.h"
#include "dht/EncodingSchemeModule.h"
#include "dht/SerializationModule.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_admin.h"
#include "dht/dhtcore/RumorMill.h"
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/SearchRunner_admin.h"
#include "dht/dhtcore/NodeStore_admin.h"
#include "dht/dhtcore/Janitor.h"
#include "exception/Jmp.h"
#include "interface/addressable/AddrInterface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "interface/UDPInterface_admin.h"
#ifdef HAS_ETH_INTERFACE
#include "interface/ETHInterface_admin.h"
#endif
#include "interface/tuntap/TUNInterface.h"
#include "interface/InterfaceConnector.h"
#include "interface/InterfaceController_admin.h"
#include "interface/FramingInterface.h"
#include "interface/ICMP6Generator.h"
#include "interface/RainflyClient.h"
#include "interface/RainflyClient_admin.h"
#include "interface/DNSServer.h"
#include "interface/addressable/PacketHeaderToUDPAddrInterface.h"
#include "io/ArrayReader.h"
#include "io/ArrayWriter.h"
#include "io/FileWriter.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator_admin.h"
#include "net/Ducttape.h"
#include "net/DefaultInterfaceController.h"
#include "net/SwitchPinger.h"
#include "net/SwitchPinger_admin.h"
#include "switch/SwitchCore.h"
#include "tunnel/IpTunnel.h"
#include "tunnel/IpTunnel_admin.h"
#include "util/events/Timeout.h"
#include "util/events/EventBase.h"
#include "util/events/Pipe.h"
#include "util/events/Timeout.h"
#include "util/Hex.h"
#include "util/log/FileWriterLog.h"
#include "util/log/IndirectLog.h"
#include "util/platform/netdev/NetDev.h"
#include "util/Security_admin.h"
#include "util/Security.h"
#include "util/version/Version.h"
#include "interface/SessionManager_admin.h"

#include <crypto_scalarmult_curve25519.h>

#include <stdlib.h>
#include <unistd.h>

// Failsafe: abort if more than 2^23 bytes are allocated (8MB)
#define ALLOCATOR_FAILSAFE (1<<23)

/** The number of milliseconds between attempting local maintenance searches. */
#define LOCAL_MAINTENANCE_SEARCH_MILLISECONDS 1000

/**
 * The number of milliseconds to pass between global maintainence searches.
 * These are searches for random targets which are used to discover new nodes.
 */
#define GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS 30000

#define RUMORMILL_CAPACITY 64

/**
 * The worst possible packet overhead.
 * assuming the packet needs to be handed off to another node
 * because we have no route to the destination.
 * and the CryptoAuths to both the destination and the handoff node are both timed out.
 */
#define WORST_CASE_OVERHEAD ( \
    /* TODO(cjd): Headers_IPv4_SIZE */ 20 \
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
  TODO(cjd): make sure we never hand off to a node for which the CA session is expired. */ \
)

static void parsePrivateKey(uint8_t privateKey[32],
                            struct Address* addr,
                            struct Except* eh)
{
    crypto_scalarmult_curve25519_base(addr->key, privateKey);
    AddressCalc_addressForPublicKey(addr->ip6.bytes, addr->key);
    if (!AddressCalc_validAddress(addr->ip6.bytes)) {
        Except_throw(eh, "Ip address outside of the FC00/8 range, invalid private key.");
    }
}

static void adminPing(Dict* input, void* vadmin, String* txid, struct Allocator* requestAlloc)
{
    Dict d = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("pong")), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

struct Context
{
    struct Allocator* allocator;
    struct Admin* admin;
    struct Log* logger;
    struct Hermes* hermes;
    struct EventBase* base;
    String* exitTxid;
};

static void shutdown(void* vcontext)
{
    struct Context* context = vcontext;
    Allocator_free(context->allocator);
}

static void onAngelExitResponse(Dict* message, void* vcontext)
{
    struct Context* context = vcontext;
    Log_info(context->logger, "Angel stopped");
    Log_info(context->logger, "Exiting");
    Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("none")), NULL);
    Admin_sendMessage(&d, context->exitTxid, context->admin);
    Timeout_setTimeout(shutdown, context, 1, context->base, context->allocator);
}

static void adminExit(Dict* input, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    Log_info(context->logger, "Got request to exit");
    Log_info(context->logger, "Stopping angel");
    context->exitTxid = String_clone(txid, context->allocator);
    Dict angelExit = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("Angel_exit")), NULL);
    Hermes_callAngel(&angelExit,
                     onAngelExitResponse,
                     context,
                     context->allocator,
                     NULL,
                     context->hermes);
}

static void angelDied(struct Pipe* p, int status)
{
    exit(1);
}

struct Core_Context
{
    struct Sockaddr* ipAddr;
    struct Ducttape* ducttape;
    struct Log* logger;
    struct Allocator* alloc;
    struct Admin* admin;
    struct EventBase* eventBase;
    struct IpTunnel* ipTunnel;
};

static void sendResponse(String* error,
                         struct Admin* admin,
                         String* txid,
                         struct Allocator* tempAlloc)
{
    Dict* output = Dict_new(tempAlloc);
    Dict_putString(output, String_CONST("error"), error, tempAlloc);
    Admin_sendMessage(output, txid, admin);
}

static void initTunnel(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Core_Context* const ctx = (struct Core_Context*) vcontext;

    struct Jmp jmp;
    Jmp_try(jmp) {
        Core_initTunnel(Dict_getString(args, String_CONST("desiredTunName")),
                        ctx->ipAddr,
                        8,
                        ctx->ducttape,
                        ctx->logger,
                        ctx->ipTunnel,
                        ctx->eventBase,
                        ctx->alloc,
                        &jmp.handler);
    } Jmp_catch {
        String* error = String_printf(requestAlloc, "Failed to configure tunnel [%s]", jmp.message);
        sendResponse(error, ctx->admin, txid, requestAlloc);
        return;
    }

    sendResponse(String_CONST("none"), ctx->admin, txid, requestAlloc);
}

void Core_admin_register(struct Sockaddr* ipAddr,
                         struct Ducttape* dt,
                         struct Log* logger,
                         struct IpTunnel* ipTunnel,
                         struct Allocator* alloc,
                         struct Admin* admin,
                         struct EventBase* eventBase)
{
    struct Core_Context* ctx = Allocator_malloc(alloc, sizeof(struct Core_Context));
    ctx->ipAddr = ipAddr;
    ctx->ducttape = dt;
    ctx->logger = logger;
    ctx->alloc = alloc;
    ctx->admin = admin;
    ctx->eventBase = eventBase;
    ctx->ipTunnel = ipTunnel;

    struct Admin_FunctionArg args[] = {
        { .name = "desiredTunName", .required = 0, .type = "String" }
    };
    Admin_registerFunction("Core_initTunnel", initTunnel, ctx, true, args, admin);
}


static Dict* getInitialConfig(struct Interface* iface,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Except* eh)
{
    struct Message* m = InterfaceWaiter_waitForData(iface, eventBase, alloc, eh);
    struct Reader* reader = ArrayReader_new(m->bytes, m->length, alloc);
    Dict* config = Dict_new(alloc);
    if (StandardBencSerializer_get()->parseDictionary(reader, alloc, config)) {
        Except_throw(eh, "Failed to parse initial configuration.");
    }

    return config;
}

void Core_initTunnel(String* desiredDeviceName,
                     struct Sockaddr* addr,
                     uint8_t addressPrefix,
                     struct Ducttape* dt,
                     struct Log* logger,
                     struct IpTunnel* ipTunnel,
                     struct EventBase* eventBase,
                     struct Allocator* alloc,
                     struct Except* eh)
{
    Log_debug(logger, "Initializing TUN device [%s]",
              (desiredDeviceName) ? desiredDeviceName->bytes : "<auto>");

    char assignedTunName[TUNInterface_IFNAMSIZ];
    char* desiredName = (desiredDeviceName) ? desiredDeviceName->bytes : NULL;
    struct Interface* tun =
        TUNInterface_new(desiredName, assignedTunName, eventBase, logger, eh, alloc);

    IpTunnel_setTunName(assignedTunName, ipTunnel);

    Ducttape_setUserInterface(dt, tun);

    NetDev_addAddress(assignedTunName, addr, addressPrefix, logger, eh);
    NetDev_setMTU(assignedTunName, DEFAULT_MTU, logger, eh);
}

/** This is a response from a call which is intended only to send information to the angel. */
static void angelResponse(Dict* resp, void* vNULL)
{
    // do nothing
}

void Core_init(struct Allocator* alloc,
               struct Log* logger,
               struct EventBase* eventBase,
               struct Interface* angelIface,
               struct Random* rand,
               struct Except* eh)
{
    struct Allocator* tempAlloc = Allocator_child(alloc);
    Dict* config = getInitialConfig(angelIface, eventBase, tempAlloc, eh);

    struct Hermes* hermes = Hermes_new(angelIface, eventBase, logger, alloc);

    String* privateKeyHex = Dict_getString(config, String_CONST("privateKey"));
    Dict* adminConf = Dict_getDict(config, String_CONST("admin"));
    String* pass = Dict_getString(adminConf, String_CONST("pass"));
    String* bind = Dict_getString(adminConf, String_CONST("bind"));
    if (!(pass && privateKeyHex && bind)) {
        if (!pass) {
            Except_throw(eh, "Expected 'pass'");
        }
        if (!bind) {
            Except_throw(eh, "Expected 'bind'");
        }
        if (!privateKeyHex) {
            Except_throw(eh, "Expected 'privateKey'");
        }
        Except_throw(eh, "Expected 'pass', 'privateKey' and 'bind' in configuration.");
    }
    Log_keys(logger, "Starting core with admin password [%s]", pass->bytes);
    uint8_t privateKey[32];
    if (privateKeyHex->len != 64
        || Hex_decode(privateKey, 32, (uint8_t*) privateKeyHex->bytes, 64) != 32)
    {
        Except_throw(eh, "privateKey must be 64 bytes of hex.");
    }

    struct Sockaddr_storage bindAddr;
    if (Sockaddr_parse(bind->bytes, &bindAddr)) {
        Except_throw(eh, "bind address [%s] unparsable", bind->bytes);
    }

    struct AddrInterface* udpAdmin =
        UDPAddrInterface_new(eventBase, &bindAddr.addr, alloc, eh, logger);

    struct Admin* admin = Admin_new(udpAdmin, alloc, logger, eventBase, pass);

    char* boundAddr = Sockaddr_print(udpAdmin->addr, tempAlloc);
    Dict adminResponse = Dict_CONST(
        String_CONST("bind"), String_OBJ(String_CONST(boundAddr)), NULL
    );
    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), Dict_CONST(
        String_CONST("admin"), Dict_OBJ(&adminResponse), NULL
    ));
    // This always times out because the angel doesn't respond.
    Hermes_callAngel(&response, angelResponse, NULL, alloc, eh, hermes);

    // --------------------- Setup the Logger --------------------- //
    Dict* logging = Dict_getDict(config, String_CONST("logging"));
    String* logTo = Dict_getString(logging, String_CONST("logTo"));
    if (logTo && String_equals(logTo, String_CONST("stdout"))) {
        // do nothing, continue logging to stdout.
    } else {
        struct Log* adminLogger = AdminLog_registerNew(admin, alloc, rand);
        IndirectLog_set(logger, adminLogger);
        logger = adminLogger;
    }

    // CryptoAuth
    struct Address addr = { .protocolVersion = Version_CURRENT_PROTOCOL };
    parsePrivateKey(privateKey, &addr, eh);
    struct CryptoAuth* cryptoAuth = CryptoAuth_new(alloc, privateKey, eventBase, logger, rand);

    struct Sockaddr* myAddr = Sockaddr_fromBytes(addr.ip6.bytes, Sockaddr_AF_INET6, alloc);

    struct SwitchCore* switchCore = SwitchCore_new(logger, alloc);
    struct DHTModuleRegistry* registry = DHTModuleRegistry_new(alloc);
    ReplyModule_register(registry, alloc);

    struct RumorMill* rumorMill = RumorMill_new(alloc, &addr, RUMORMILL_CAPACITY);

    struct NodeStore* nodeStore = NodeStore_new(&addr, alloc, logger, rumorMill);

    struct RouterModule* routerModule = RouterModule_register(registry,
                                                              alloc,
                                                              addr.key,
                                                              eventBase,
                                                              logger,
                                                              rand,
                                                              nodeStore);

    struct SearchRunner* searchRunner = SearchRunner_new(nodeStore,
                                                         logger,
                                                         eventBase,
                                                         routerModule,
                                                         addr.ip6.bytes,
                                                         rumorMill,
                                                         alloc);

    Janitor_new(LOCAL_MAINTENANCE_SEARCH_MILLISECONDS,
                GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS,
                routerModule,
                nodeStore,
                searchRunner,
                rumorMill,
                logger,
                alloc,
                eventBase,
                rand);

    EncodingSchemeModule_register(registry, logger, alloc);

    SerializationModule_register(registry, logger, alloc);

    struct IpTunnel* ipTun = IpTunnel_new(logger, eventBase, alloc, rand, hermes);

    struct Ducttape* dt = Ducttape_register(privateKey,
                                            registry,
                                            routerModule,
                                            searchRunner,
                                            switchCore,
                                            eventBase,
                                            alloc,
                                            logger,
                                            ipTun,
                                            rand);

    struct SwitchPinger* sp =
        SwitchPinger_new(&dt->switchPingerIf, eventBase, rand, logger, &addr, alloc);

    // Interfaces.
    struct InterfaceController* ifController =
        DefaultInterfaceController_new(cryptoAuth,
                                       switchCore,
                                       routerModule,
                                       rumorMill,
                                       logger,
                                       eventBase,
                                       sp,
                                       rand,
                                       alloc);

    // ------------------- DNS -------------------------//

    struct Sockaddr_storage rainflyAddr;
    Assert_true(!Sockaddr_parse("::", &rainflyAddr));
    struct AddrInterface* rainflyIface =
        UDPAddrInterface_new(eventBase, &rainflyAddr.addr, alloc, eh, logger);
    struct RainflyClient* rainfly = RainflyClient_new(rainflyIface, eventBase, rand, logger);
    Assert_true(!Sockaddr_parse("[fc00::1]:53", &rainflyAddr));
    struct AddrInterface* magicUDP =
        PacketHeaderToUDPAddrInterface_new(&dt->magicInterface, alloc, &rainflyAddr.addr);
    DNSServer_new(magicUDP, logger, rainfly);


    // ------------------- Register RPC functions ----------------------- //
    InterfaceController_admin_register(ifController, admin, alloc);
    SwitchPinger_admin_register(sp, admin, alloc);
    UDPInterface_admin_register(eventBase, alloc, logger, admin, ifController);
#ifdef HAS_ETH_INTERFACE
    ETHInterface_admin_register(eventBase, alloc, logger, admin, ifController);
#endif
    NodeStore_admin_register(nodeStore, admin, alloc);
    RouterModule_admin_register(routerModule, admin, alloc);
    SearchRunner_admin_register(searchRunner, admin, alloc);
    AuthorizedPasswords_init(admin, cryptoAuth, alloc);
    Admin_registerFunction("ping", adminPing, admin, false, NULL, admin);
    Core_admin_register(myAddr, dt, logger, ipTun, alloc, admin, eventBase);
    Security_admin_register(alloc, logger, admin);
    IpTunnel_admin_register(ipTun, admin, alloc);
    SessionManager_admin_register(dt->sessionManager, admin, alloc);
    RainflyClient_admin_register(rainfly, admin, alloc);
    Allocator_admin_register(alloc, admin);

    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .allocator = alloc,
        .admin = admin,
        .logger = logger,
        .hermes = hermes,
        .base = eventBase,
    }));
    Admin_registerFunction("Core_exit", adminExit, ctx, true, NULL, admin);
}


int Core_main(int argc, char** argv)
{
    struct Except* eh = NULL;

    if (argc != 3) {
        Except_throw(eh, "This is internal to cjdns and shouldn't started manually.");
    }

    struct Allocator* alloc = MallocAllocator_new(ALLOCATOR_FAILSAFE);
    struct Log* preLogger = FileWriterLog_new(stderr, alloc);
    struct EventBase* eventBase = EventBase_new(alloc);

    // -------------------- Setup the Pre-Logger ---------------------- //
    struct Log* logger = IndirectLog_new(alloc);
    IndirectLog_set(logger, preLogger);

    // -------------------- Setup the PRNG ---------------------- //
    struct Random* rand = LibuvEntropyProvider_newDefaultRandom(eventBase, logger, eh, alloc);

    // -------------------- Change Canary Value ---------------------- //
    Allocator_setCanary(alloc, (unsigned long)Random_uint64(rand));

    // The first read inside of getInitialConfig() will begin it waiting.
    struct Pipe* angelPipe = Pipe_named(argv[2], eventBase, eh, alloc);
    angelPipe->logger = logger;
    angelPipe->onClose = angelDied;

    struct Interface* angelIface = FramingInterface_new(65535, &angelPipe->iface, alloc);

    Core_init(alloc, logger, eventBase, angelIface, rand, eh);
    EventBase_beginLoop(eventBase);
    return 0;
}
