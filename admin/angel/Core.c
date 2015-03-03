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
#include "benc/serialization/standard/BencMessageReader.h"
#include "crypto/AddressCalc.h"
#include "crypto/random/Random.h"
#include "crypto/random/libuv/LibuvEntropyProvider.h"
#include "dht/Pathfinder.h"
#include "exception/Jmp.h"
#include "interface/Iface.h"
#include "util/events/UDPAddrIface.h"
#include "interface/tuntap/TUNInterface.h"
#include "interface/UDPInterface_admin.h"
#ifdef HAS_ETH_INTERFACE
#include "interface/ETHInterface_admin.h"
#endif
#include "net/InterfaceController_admin.h"
#include "interface/addressable/PacketHeaderToUDPAddrIface.h"
#include "interface/FramingIface.h"
#include "interface/RainflyClient.h"
#include "interface/RainflyClient_admin.h"
#include "interface/DNSServer.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator_admin.h"
#include "net/SwitchPinger_admin.h"
#include "tunnel/IpTunnel_admin.h"
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
#include "net/SessionManager_admin.h"
#include "wire/SwitchHeader.h"
#include "wire/CryptoHeader.h"
#include "wire/Headers.h"
#include "net/NetCore.h"

#include <crypto_scalarmult_curve25519.h>

#include <stdlib.h>
#include <unistd.h>

// Failsafe: abort if more than 2^23 bytes are allocated (8MB)
#define ALLOCATOR_FAILSAFE (1<<23)

// TODO(cjd): we need to begin detecting MTU and informing the OS properly!
/**
 * The worst possible packet overhead, we're in session setup with the endpoint.
 */
#define WORST_CASE_OVERHEAD ( \
      Headers_IP4Header_SIZE \
    + Headers_UDPHeader_SIZE \
    + 4 /* Nonce */ \
    + 16 /* Poly1305 authenticator */ \
    + SwitchHeader_SIZE \
    + CryptoHeader_SIZE \
    + 4 /* Handle */ \
    + DataHeader_SIZE \
)

/** The default MTU, assuming the external MTU is 1492 (common for PPPoE DSL) */
#define DEFAULT_MTU ( 1492 - WORST_CASE_OVERHEAD )

static void adminPing(Dict* input, void* vadmin, String* txid, struct Allocator* requestAlloc)
{
    Dict d = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("pong")), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

static void adminPid(Dict* input, void* vadmin, String* txid, struct Allocator* requestAlloc)
{
    int pid = getpid();
    Dict d = Dict_CONST(String_CONST("pid"), Int_OBJ(pid), NULL);
    Admin_sendMessage(&d, txid, (struct Admin*) vadmin);
}

struct Context
{
    struct Allocator* alloc;
    struct Admin* admin;
    struct Log* logger;
    struct Hermes* hermes;
    struct EventBase* base;
    struct NetCore* nc;
    struct IpTunnel* ipTunnel;
    String* exitTxid;
    Identity
};

static void shutdown(void* vcontext)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    Allocator_free(context->alloc);
}

static void onAngelExitResponse(Dict* message, void* vcontext)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    Log_info(context->logger, "Angel stopped");
    Log_info(context->logger, "Exiting");
    Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST("none")), NULL);
    Admin_sendMessage(&d, context->exitTxid, context->admin);
    Timeout_setTimeout(shutdown, context, 1, context->base, context->alloc);
}

static void adminExit(Dict* input, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = Identity_check((struct Context*) vcontext);
    Log_info(context->logger, "Got request to exit");
    Log_info(context->logger, "Stopping angel");
    context->exitTxid = String_clone(txid, context->alloc);
    Dict angelExit = Dict_CONST(String_CONST("q"), String_OBJ(String_CONST("Angel_exit")), NULL);
    Hermes_callAngel(&angelExit,
                     onAngelExitResponse,
                     context,
                     context->alloc,
                     NULL,
                     context->hermes);
}

static void angelDied(struct Pipe* p, int status)
{
    exit(1);
}

static void sendResponse(String* error,
                         struct Admin* admin,
                         String* txid,
                         struct Allocator* tempAlloc)
{
    Dict* output = Dict_new(tempAlloc);
    Dict_putString(output, String_CONST("error"), error, tempAlloc);
    Admin_sendMessage(output, txid, admin);
}

static void initTunnel2(String* desiredDeviceName,
                        struct Context* ctx,
                        uint8_t addressPrefix,
                        struct Except* eh)
{
    Log_debug(ctx->logger, "Initializing TUN device [%s]",
              (desiredDeviceName) ? desiredDeviceName->bytes : "<auto>");

    char assignedTunName[TUNInterface_IFNAMSIZ];
    char* desiredName = (desiredDeviceName) ? desiredDeviceName->bytes : NULL;

    struct Iface* tun = TUNInterface_new(
        desiredName, assignedTunName, 0, ctx->base, ctx->logger, eh, ctx->alloc);

    Iface_plumb(tun, &ctx->nc->tunAdapt->tunIf);

    IpTunnel_setTunName(assignedTunName, ctx->ipTunnel);

    struct Sockaddr* myAddr =
        Sockaddr_fromBytes(ctx->nc->myAddress->ip6.bytes, Sockaddr_AF_INET6, ctx->alloc);
    NetDev_addAddress(assignedTunName, myAddr, addressPrefix, ctx->logger, eh);
    NetDev_setMTU(assignedTunName, DEFAULT_MTU, ctx->logger, eh);
}

static void initTunnel(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vcontext);

    struct Jmp jmp;
    Jmp_try(jmp) {
        String* desiredName = Dict_getString(args, String_CONST("desiredTunName"));
        initTunnel2(desiredName, ctx, 8, &jmp.handler);
    } Jmp_catch {
        String* error = String_printf(requestAlloc, "Failed to configure tunnel [%s]", jmp.message);
        sendResponse(error, ctx->admin, txid, requestAlloc);
        return;
    }

    sendResponse(String_CONST("none"), ctx->admin, txid, requestAlloc);
}

static Dict* getInitialConfig(struct Iface* iface,
                              struct EventBase* eventBase,
                              struct Allocator* alloc,
                              struct Except* eh)
{
    struct Message* m = InterfaceWaiter_waitForData(iface, eventBase, alloc, eh);
    return BencMessageReader_read(m, alloc, eh);
}

/** This is a response from a call which is intended only to send information to the angel. */
static void angelResponse(Dict* resp, void* vNULL)
{
    // do nothing
}

void Core_init(struct Allocator* alloc,
               struct Log* logger,
               struct EventBase* eventBase,
               struct Iface* angelIface,
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

    struct UDPAddrIface* udpAdmin = UDPAddrIface_new(eventBase, &bindAddr.addr, alloc, eh, logger);
    struct Admin* admin = Admin_new(&udpAdmin->generic, logger, eventBase, pass);

    char* boundAddr = Sockaddr_print(udpAdmin->generic.addr, tempAlloc);
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
        struct Log* adminLogger = AdminLog_registerNew(admin, alloc, rand, eventBase);
        IndirectLog_set(logger, adminLogger);
        logger = adminLogger;
    }

    struct NetCore* nc = NetCore_new(privateKey, alloc, eventBase, rand, logger);

    struct IpTunnel* ipTunnel = IpTunnel_new(logger, eventBase, alloc, rand, hermes);
    Iface_plumb(&nc->tunAdapt->ipTunnelIf, &ipTunnel->tunInterface);
    Iface_plumb(&nc->upper->ipTunnelIf, &ipTunnel->nodeInterface);

    Pathfinder_register(alloc, logger, eventBase, rand, admin, nc->ee);

    // ------------------- DNS -------------------------//

    struct Sockaddr_storage rainflyAddr;
    Assert_true(!Sockaddr_parse("::", &rainflyAddr));
    struct UDPAddrIface* rainflyIface =
        UDPAddrIface_new(eventBase, &rainflyAddr.addr, alloc, eh, logger);
    struct RainflyClient* rainfly =
        RainflyClient_new(&rainflyIface->generic, eventBase, rand, logger);
    Assert_true(!Sockaddr_parse("[fc00::1]:53", &rainflyAddr));
    struct PacketHeaderToUDPAddrIface* magicUDP =
        PacketHeaderToUDPAddrIface_new(alloc, &rainflyAddr.addr);
//    Iface_plumb(&magicUDP->headerIf, &dtAAAAAAAAAAAAAA->magicIf);
    DNSServer_new(&magicUDP->udpIf, logger, rainfly);


    // ------------------- Register RPC functions ----------------------- //
    InterfaceController_admin_register(nc->ifController, admin, alloc);
    SwitchPinger_admin_register(nc->sp, admin, alloc);
    UDPInterface_admin_register(eventBase, alloc, logger, admin, nc->ifController);
#ifdef HAS_ETH_INTERFACE
    ETHInterface_admin_register(eventBase, alloc, logger, admin, nc->ifController, hermes);
#endif

    AuthorizedPasswords_init(admin, nc->ca, alloc);
    Admin_registerFunction("ping", adminPing, admin, false, NULL, admin);
//    Core_admin_register(myAddr, logger, ipTun, alloc, admin, eventBase);
    Security_admin_register(alloc, logger, admin);
    IpTunnel_admin_register(ipTunnel, admin, alloc);
    SessionManager_admin_register(nc->sm, admin, alloc);
    RainflyClient_admin_register(rainfly, admin, alloc);
    Allocator_admin_register(alloc, admin);

    struct Context* ctx = Allocator_calloc(alloc, sizeof(struct Context), 1);
    Identity_set(ctx);
    ctx->alloc = alloc;
    ctx->admin = admin;
    ctx->logger = logger;
    ctx->hermes = hermes;
    ctx->base = eventBase;
    ctx->ipTunnel = ipTunnel;
    ctx->nc = nc;

    Admin_registerFunction("Core_exit", adminExit, ctx, true, NULL, admin);

    Admin_registerFunction("Core_pid", adminPid, admin, false, NULL, admin);

    Admin_registerFunction("Core_initTunnel", initTunnel, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "desiredTunName", .required = 0, .type = "String" }
        }), admin);
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

    struct Iface* angelIface = FramingIface_new(65535, &angelPipe->iface, alloc);

    Core_init(alloc, logger, eventBase, angelIface, rand, eh);
    EventBase_beginLoop(eventBase);
    return 0;
}
