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
#include "client/AdminClient.h"
#include "client/Configurator.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "memory/Allocator.h"
#include "util/events/Event.h"
#include "util/events/UDPAddrIface.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/platform/Sockaddr.h"
#include "util/Defined.h"
#include "util/events/Timeout.h"

#include <stdlib.h>
#include <stdbool.h>

struct Context
{
    struct Log* logger;
    struct Allocator* alloc;
    struct AdminClient* client;

    struct Allocator* currentReqAlloc;
    struct AdminClient_Result* currentResult;

    struct EventBase* base;
};

static void rpcCallback(struct AdminClient_Promise* p, struct AdminClient_Result* res)
{
    struct Context* ctx = p->userData;
    Allocator_adopt(ctx->alloc, p->alloc);
    ctx->currentResult = res;
    EventBase_endLoop(ctx->base);
}

static void die(struct AdminClient_Result* res, struct Context* ctx, struct Allocator* alloc)
{
    Log_keys(ctx->logger, "message bytes = [%s]", res->messageBytes);
    #ifndef Log_KEYS
        Log_critical(ctx->logger, "enable Log_LEVEL=KEYS to see message content.");
    #endif

    Dict d = NULL;
    struct AdminClient_Promise* exitPromise =
        AdminClient_rpcCall(String_CONST("Core_exit"), &d, ctx->client, alloc);
    exitPromise->callback = rpcCallback;
    exitPromise->userData = ctx;

    EventBase_beginLoop(ctx->base);

    if (ctx->currentResult->err) {
        Log_critical(ctx->logger, "Failed to stop the core.");
    }
    Log_critical(ctx->logger, "Aborting.");
    exit(1);
}

static int rpcCall0(String* function,
                    Dict* args,
                    struct Context* ctx,
                    struct Allocator* alloc,
                    Dict** resultP,
                    bool exitIfError)
{
    ctx->currentReqAlloc = Allocator_child(alloc);
    ctx->currentResult = NULL;
    struct AdminClient_Promise* promise = AdminClient_rpcCall(function, args, ctx->client, alloc);
    promise->callback = rpcCallback;
    promise->userData = ctx;

    EventBase_beginLoop(ctx->base);

    struct AdminClient_Result* res = ctx->currentResult;
    Assert_true(res);

    if (res->err) {
        Log_critical(ctx->logger,
                      "Failed to make function call [%s], error: [%s]",
                      AdminClient_errorString(res->err),
                      function->bytes);
        die(res, ctx, alloc);
    }
    String* error = Dict_getStringC(res->responseDict, "error");
    int ret = 0;
    if (error && !String_equals(error, String_CONST("none"))) {
        if (exitIfError) {
            Log_critical(ctx->logger,
                         "Got error [%s] calling [%s]",
                         error->bytes,
                         function->bytes);
            die(res, ctx, alloc);
        }
        Log_warn(ctx->logger, "Got error [%s] calling [%s], ignoring.",
                 error->bytes, function->bytes);
        ret = 1;
    }

    if (resultP) {
        *resultP = res->responseDict;
    } else {
        Allocator_free(ctx->currentReqAlloc);
    }
    ctx->currentReqAlloc = NULL;

    return ret;
}

static void rpcCall(String* function, Dict* args, struct Context* ctx, struct Allocator* alloc)
{
    rpcCall0(function, args, ctx, alloc, NULL, true);
}

static void authorizedPasswords(List* list, struct Context* ctx)
{
    uint32_t count = List_size(list);
    for (uint32_t i = 0; i < count; i++) {
        Dict* d = List_getDict(list, i);
        Log_info(ctx->logger, "Checking authorized password %d.", i);
        if (!d) {
            Log_critical(ctx->logger, "Not a dictionary type %d.", i);
            exit(-1);
        }
        String* passwd = Dict_getStringC(d, "password");
        if (!passwd) {
            Log_critical(ctx->logger, "Must specify a password %d.", i);
            exit(-1);
        }
    }

    for (uint32_t i = 0; i < count; i++) {
        struct Allocator* child = Allocator_child(ctx->alloc);
        Dict* d = List_getDict(list, i);
        String* passwd = Dict_getStringC(d, "password");
        String* user = Dict_getStringC(d, "user");
        String* displayName = user;
        if (!displayName) {
            displayName = String_printf(child, "password [%d]", i);
        }
        //String* publicKey = Dict_getStringC(d, "publicKey");
        String* ipv6 = Dict_getStringC(d, "ipv6");
        Log_info(ctx->logger, "Adding authorized password #[%d] for user [%s].",
            i, displayName->bytes);
        Dict *args = Dict_new(child);
        uint32_t i = 1;
        Dict_putIntC(args, "authType", i, child);
        Dict_putStringC(args, "password", passwd, child);
        if (user) {
            Dict_putStringC(args, "user", user, child);
        }
        Dict_putStringC(args, "displayName", displayName, child);
        if (ipv6) {
            Log_info(ctx->logger,
                "  This connection password restricted to [%s] only.", ipv6->bytes);
            Dict_putStringC(args, "ipv6", ipv6, child);
        }
        rpcCall(String_CONST("AuthorizedPasswords_add"), args, ctx, child);
        Allocator_free(child);
    }
}

static void udpInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getListC(config, "UDPInterface");
    if (!ifaces) {
        ifaces = List_new(ctx->alloc);
        List_addDict(ifaces, Dict_getDictC(config, "UDPInterface"), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);
    for (uint32_t i = 0; i < count; i++) {
        Dict *udp = List_getDict(ifaces, i);
        if (!udp) {
            continue;
        }
        // Setup the interface.
        String* bindStr = Dict_getStringC(udp, "bind");
        Dict* d = Dict_new(ctx->alloc);
        if (bindStr) {
            Dict_putStringC(d, "bindAddress", bindStr, ctx->alloc);
        }
        int64_t* dscp = Dict_getIntC(udp, "dscp");
        if (dscp) {
            Dict_putIntC(d, "dscp", *dscp, ctx->alloc);
        }
        Dict* resp = NULL;
        rpcCall0(String_CONST("UDPInterface_new"), d, ctx, ctx->alloc, &resp, true);
        int ifNum = *(Dict_getIntC(resp, "interfaceNumber"));

        // Make the connections.
        Dict* connectTo = Dict_getDictC(udp, "connectTo");
        if (connectTo) {
            struct Dict_Entry* entry = *connectTo;
            struct Allocator* perCallAlloc = Allocator_child(ctx->alloc);
            while (entry != NULL) {
                String* key = (String*) entry->key;
                if (entry->val->type != Object_DICT) {
                    Log_critical(ctx->logger, "interfaces.UDPInterface.connectTo: entry [%s] "
                                               "is not a dictionary type.", key->bytes);
                    exit(-1);
                }
                Dict* value = entry->val->as.dictionary;
                Log_keys(ctx->logger, "Attempting to connect to node [%s].", key->bytes);
                key = String_clone(key, perCallAlloc);
                char* lastColon = CString_strrchr(key->bytes, ':');

                if (lastColon) {
                    if (!Sockaddr_parse(key->bytes, NULL)) {
                        // it's a sockaddr, fall through
                    } else {
                        // try it as a hostname.
                        Log_critical(ctx->logger, "Couldn't add connection [%s], "
                                                    "hostnames aren't supported.", key->bytes);
                        exit(-1);
                    }
                } else {
                    // it doesn't have a port
                    Log_critical(ctx->logger, "Connection [%s] must be $IP:$PORT, or "
                                                "[$IP]:$PORT for IPv6.", key->bytes);
                    exit(-1);
                }
                Dict_putIntC(value, "interfaceNumber", ifNum, perCallAlloc);
                Dict_putStringC(value, "address", key, perCallAlloc);
                rpcCall(String_CONST("UDPInterface_beginConnection"), value, ctx, perCallAlloc);

                // Make a IPTunnel exception for this node
                Dict* aed = Dict_new(perCallAlloc);
                *lastColon = '\0';
                Dict_putStringC(aed, "route", String_new(key->bytes, perCallAlloc),
                    perCallAlloc);
                *lastColon = ':';
                rpcCall(String_CONST("RouteGen_addException"), aed, ctx, perCallAlloc);

                entry = entry->next;
            }
            Allocator_free(perCallAlloc);
        }
    }
}

static void tunInterface(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    String* ifaceType = Dict_getStringC(ifaceConf, "type");
    if (!String_equals(ifaceType, String_CONST("TUNInterface"))) {
        return;
    }

    // Setup the interface.
    String* tunfd = Dict_getStringC(ifaceConf, "tunfd");
    String* device = Dict_getStringC(ifaceConf, "tunDevice");

    Dict* args = Dict_new(tempAlloc);
    if (tunfd && device) {
        Dict_putStringC(args, "path", device, tempAlloc);
        Dict_putStringC(args, "type",
                       String_new(tunfd->bytes, tempAlloc), tempAlloc);
        Dict* res = NULL;
        rpcCall0(String_CONST("FileNo_import"), args, ctx, tempAlloc, &res, false);
        if (res) {
            Dict* args = Dict_new(tempAlloc);
            int64_t* tunfd = Dict_getIntC(res, "tunfd");
            int64_t* type = Dict_getIntC(res, "type");
            Dict_putIntC(args, "tunfd", *tunfd, tempAlloc);
            Dict_putIntC(args, "type", *type, tempAlloc);
            rpcCall0(String_CONST("Core_initTunfd"), args, ctx, tempAlloc, NULL, false);
        }
    } else {
        if (device) {
            Dict_putStringC(args, "desiredTunName", device, tempAlloc);
        }
        rpcCall0(String_CONST("Core_initTunnel"), args, ctx, tempAlloc, NULL, false);
    }
}

static void ipTunnel(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    List* incoming = Dict_getListC(ifaceConf, "allowedConnections");
    if (incoming) {
        Dict* d;
        for (int i = 0; (d = List_getDict(incoming, i)) != NULL; i++) {
            String* key = Dict_getStringC(d, "publicKey");
            String* ip4 = Dict_getStringC(d, "ip4Address");
            // Note that the prefix length has to be a proper int in the config
            // (not quoted!)
            int64_t* ip4Prefix = Dict_getIntC(d, "ip4Prefix");
            String* ip6 = Dict_getStringC(d, "ip6Address");
            int64_t* ip6Prefix = Dict_getIntC(d, "ip6Prefix");
            if (!key) {
                Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                          "'publicKey' required.", i);
                exit(1);
            }
            if (!ip4 && !ip6) {
                Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                          "either 'ip4Address' or 'ip6Address' required.", i);
                exit(1);
            } else if (ip4Prefix && !ip4) {
                Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                          "'ip4Address' required with 'ip4Prefix'.", i);
                exit(1);
            } else if (ip6Prefix && !ip6) {
                Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                          "'ip6Address' required with 'ip6Prefix'.", i);
                exit(1);
            }
            Log_debug(ctx->logger, "Allowing IpTunnel connections from [%s]", key->bytes);

            if (ip4) {
                Log_debug(ctx->logger, "Issue IPv4 address %s", ip4->bytes);
                if (ip4Prefix) {
                    Log_debug(ctx->logger, "Issue IPv4 netmask/prefix length /%d",
                        (int) *ip4Prefix);
                } else {
                    Log_debug(ctx->logger, "Use default netmask/prefix length /0");
                }
            }

            if (ip6) {
                Log_debug(ctx->logger, "Issue IPv6 address [%s]", ip6->bytes);
                if (ip6Prefix) {
                    Log_debug(ctx->logger, "Issue IPv6 netmask/prefix length /%d",
                        (int) *ip6Prefix);
                } else {
                    Log_debug(ctx->logger, "Use default netmask/prefix length /0");
                }
            }

            Dict_putStringC(d, "publicKeyOfAuthorizedNode", key, tempAlloc);
            rpcCall0(String_CONST("IpTunnel_allowConnection"), d, ctx, tempAlloc, NULL, true);
        }
    }

    List* outgoing = Dict_getListC(ifaceConf, "outgoingConnections");
    if (outgoing) {
        String* s;
        for (int i = 0; (s = List_getString(outgoing, i)) != NULL; i++) {
            Log_debug(ctx->logger, "Initiating IpTunnel connection to [%s]", s->bytes);
            Dict requestDict =
                Dict_CONST(String_CONST("publicKeyOfNodeToConnectTo"), String_OBJ(s), NULL);
            rpcCall0(String_CONST("IpTunnel_connectTo"), &requestDict, ctx, tempAlloc, NULL, true);
        }
    }
}

static void supernodes(List* supernodes, struct Allocator* tempAlloc, struct Context* ctx)
{
    if (!supernodes) { return; }
    String* s;
    for (int i = 0; (s = List_getString(supernodes, i)) != NULL; i++) {
        Log_debug(ctx->logger, "Loading supernode connection to [%s]", s->bytes);
        Dict reqDict = Dict_CONST(String_CONST("key"), String_OBJ(s), NULL);
        rpcCall0(String_CONST("SupernodeHunter_addSnode"), &reqDict, ctx, tempAlloc, NULL, true);
    }
}

static void routerConfig(Dict* routerConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    tunInterface(Dict_getDictC(routerConf, "interface"), tempAlloc, ctx);
    ipTunnel(Dict_getDictC(routerConf, "ipTunnel"), tempAlloc, ctx);
    supernodes(Dict_getListC(routerConf, "supernodes"), tempAlloc, ctx);
}

static void ethInterfaceSetBeacon(int ifNum, Dict* eth, struct Context* ctx)
{
    int64_t* beaconP = Dict_getIntC(eth, "beacon");
    if (beaconP) {
        int64_t beacon = *beaconP;
        if (beacon > 3 || beacon < 0) {
            Log_error(ctx->logger, "interfaces.ETHInterface.beacon may only be 0, 1,or 2");
        } else {
            // We can cast beacon to an int here because we know it's small enough
            Log_info(ctx->logger, "Setting beacon mode on ETHInterface to [%d].", (int) beacon);
            Dict d = Dict_CONST(String_CONST("interfaceNumber"), Int_OBJ(ifNum),
                     Dict_CONST(String_CONST("state"), Int_OBJ(beacon), NULL));
            rpcCall(String_CONST("ETHInterface_beacon"), &d, ctx, ctx->alloc);
        }
    }
}

static void ethInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getListC(config, "ETHInterface");
    if (!ifaces) {
        ifaces = List_new(ctx->alloc);
        List_addDict(ifaces, Dict_getDictC(config, "ETHInterface"), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);

    for (uint32_t i = 0; i < count; i++) {
        Dict *eth = List_getDict(ifaces, i);
        if (!eth) { continue; }
        String* deviceStr = Dict_getStringC(eth, "bind");
        if (!deviceStr || !String_equals(String_CONST("all"), deviceStr)) { continue; }
        Log_info(ctx->logger, "Setting up all ETHInterfaces...");
        Dict* res = NULL;
        Dict* d = Dict_new(ctx->alloc);
        if (rpcCall0(String_CONST("ETHInterface_listDevices"), d, ctx, ctx->alloc, &res, false)) {
            Log_info(ctx->logger, "Getting device list failed");
            break;
        }
        List* devs = Dict_getListC(res, "devices");
        uint32_t devCount = List_size(devs);
        for (uint32_t j = 0; j < devCount; j++) {
            Dict* d = Dict_new(ctx->alloc);
            String* deviceName = List_getString(devs, j);
            // skip loopback...
            if (String_equals(String_CONST("lo"), deviceName)) { continue; }
            Dict_putStringC(d, "bindDevice", deviceName, ctx->alloc);
            Dict* resp;
            Log_info(ctx->logger, "Creating new ETHInterface [%s]", deviceName->bytes);
            if (rpcCall0(String_CONST("ETHInterface_new"), d, ctx, ctx->alloc, &resp, false)) {
                Log_warn(ctx->logger, "Failed to create ETHInterface.");
                continue;
            }
            int ifNum = *(Dict_getIntC(resp, "interfaceNumber"));
            ethInterfaceSetBeacon(ifNum, eth, ctx);
        }
        return;
    }

    for (uint32_t i = 0; i < count; i++) {
        Dict *eth = List_getDict(ifaces, i);
        if (!eth) { continue; }
        // Setup the interface.
        String* deviceStr = Dict_getStringC(eth, "bind");
        Log_info(ctx->logger, "Setting up ETHInterface [%d].", i);
        Dict* d = Dict_new(ctx->alloc);
        if (deviceStr) {
            Log_info(ctx->logger, "Binding to device [%s].", deviceStr->bytes);
            Dict_putStringC(d, "bindDevice", deviceStr, ctx->alloc);
        }
        Dict* resp = NULL;
        if (rpcCall0(String_CONST("ETHInterface_new"), d, ctx, ctx->alloc, &resp, false)) {
            Log_warn(ctx->logger, "Failed to create ETHInterface.");
            continue;
        }
        int ifNum = *(Dict_getIntC(resp, "interfaceNumber"));
        ethInterfaceSetBeacon(ifNum, eth, ctx);

        // Make the connections.
        Dict* connectTo = Dict_getDictC(eth, "connectTo");
        if (connectTo) {
            Log_info(ctx->logger, "ETHInterface should connect to a specific node.");
            struct Dict_Entry* entry = *connectTo;
            while (entry != NULL) {
                String* key = (String*) entry->key;
                if (entry->val->type != Object_DICT) {
                    Log_critical(ctx->logger, "interfaces.ETHInterface.connectTo: entry [%s] "
                                               "is not a dictionary type.", key->bytes);
                    exit(-1);
                }
                Dict* value = entry->val->as.dictionary;

                Log_keys(ctx->logger, "Attempting to connect to node [%s].", key->bytes);

                struct Allocator* perCallAlloc = Allocator_child(ctx->alloc);
                // Turn the dict from the config into our RPC args dict by filling in all
                // the arguments,
                Dict_putStringC(value, "macAddress", key, perCallAlloc);
                Dict_putIntC(value, "interfaceNumber", ifNum, perCallAlloc);
                rpcCall(String_CONST("ETHInterface_beginConnection"), value, ctx, perCallAlloc);
                Allocator_free(perCallAlloc);

                entry = entry->next;
            }
        }
    }
}

static void security(struct Allocator* tempAlloc, List* conf, struct Log* log, struct Context* ctx)
{
    int seccomp = 1;
    int nofiles = 0;
    int noforks = 1;
    int chroot = 1;
    int setupComplete = 1;
    int setuser = 1;
    if (Defined(win32)) {
        setuser = 0;
    }

    int uid = -1;
    int64_t* group = NULL;
    int keepNetAdmin = 1;

    do {
        Dict* d = Dict_new(tempAlloc);
        Dict_putStringCC(d, "user", "nobody", tempAlloc);
        if (!Defined(win32)) {
            Dict* ret = NULL;
            rpcCall0(String_CONST("Security_getUser"), d, ctx, tempAlloc, &ret, true);
            uid = *Dict_getIntC(ret, "uid");
            group = Dict_getIntC(ret, "gid");
        }
    } while (0);

    for (int i = 0; conf && i < List_size(conf); i++) {
        Dict* elem = List_getDict(conf, i);
        String* s;
        if (elem && (s = Dict_getStringC(elem, "setuser"))) {
            if (setuser == 0) { continue; }
            Dict* d = Dict_new(tempAlloc);
            Dict_putStringC(d, "user", s, tempAlloc);
            Dict* ret = NULL;
            rpcCall0(String_CONST("Security_getUser"), d, ctx, tempAlloc, &ret, true);
            uid = *Dict_getIntC(ret, "uid");
            group = Dict_getIntC(ret, "gid");
            int64_t* nka = Dict_getIntC(elem, "keepNetAdmin");
            int64_t* exemptAngel = Dict_getIntC(elem, "exemptAngel");
            keepNetAdmin = ((nka) ? *nka : ((exemptAngel) ? *exemptAngel : 0));
            continue;
        }
        if (elem && (s = Dict_getStringC(elem, "chroot"))) {
            Log_debug(log, "Security_chroot(%s)", s->bytes);
            Dict* d = Dict_new(tempAlloc);
            Dict_putStringC(d, "root", s, tempAlloc);
            rpcCall0(String_CONST("Security_chroot"), d, ctx, tempAlloc, NULL, false);
            chroot = 0;
            continue;
        }
        uint64_t* x;
        if (elem && (x = Dict_getIntC(elem, "nofiles"))) {
            if (!*x) { continue; }
            nofiles = 1;
            continue;
        }
        if (elem && (x = Dict_getIntC(elem, "setuser"))) {
            if (!*x) { setuser = 0; }
            continue;
        }
        if (elem && (x = Dict_getIntC(elem, "seccomp"))) {
            if (!*x) { seccomp = 0; }
            continue;
        }
        if (elem && (x = Dict_getIntC(elem, "noforks"))) {
            if (!*x) { noforks = 0; }
            continue;
        }
        if (elem && (x = Dict_getIntC(elem, "chroot"))) {
            if (!*x) { chroot = 0; }
            continue;
        }
        if (elem && (x = Dict_getIntC(elem, "setupComplete"))) {
            if (!*x) { setupComplete = 0; }
            continue;
        }
        Log_info(ctx->logger, "Unrecognized entry in security at index [%d]", i);
    }

    if (chroot) {
        Log_debug(log, "Security_chroot(/var/run)");
        Dict* d = Dict_new(tempAlloc);
        Dict_putStringCC(d, "root", "/var/run/", tempAlloc);
        rpcCall0(String_CONST("Security_chroot"), d, ctx, tempAlloc, NULL, false);
    }
    /* FIXME(sdg): moving noforks after setuser might make nproc <- 0,0 work
     on older kernels, where doing it before causes setuid to fail w EAGAIN. */
    if (noforks) {
        Log_debug(log, "Security_noforks()");
        Dict* d = Dict_new(tempAlloc);
        rpcCall(String_CONST("Security_noforks"), d, ctx, tempAlloc);
    }
    if (setuser) {
        Log_debug(log, "Security_setUser(uid:%d, keepNetAdmin:%d)", uid, keepNetAdmin);
        Dict* d = Dict_new(tempAlloc);
        Dict_putIntC(d, "uid", uid, tempAlloc);
        if (group) {
            Dict_putIntC(d, "gid", (int)*group, tempAlloc);
        }
        Dict_putIntC(d, "keepNetAdmin", keepNetAdmin, tempAlloc);
        rpcCall0(String_CONST("Security_setUser"), d, ctx, tempAlloc, NULL, false);
    }
    if (nofiles) {
        Log_debug(log, "Security_nofiles()");
        Dict* d = Dict_new(tempAlloc);
        rpcCall(String_CONST("Security_nofiles"), d, ctx, tempAlloc);
    }
    if (seccomp) {
        Log_debug(log, "Security_seccomp()");
        Dict* d = Dict_new(tempAlloc);
        rpcCall(String_CONST("Security_seccomp"), d, ctx, tempAlloc);
    }
    if (setupComplete) {
        Log_debug(log, "Security_setupComplete()");
        Dict* d = Dict_new(tempAlloc);
        rpcCall(String_CONST("Security_setupComplete"), d, ctx, tempAlloc);
    }
}

static int tryPing(struct Allocator* tempAlloc, struct Context* ctx)
{
    Dict* resp = NULL;
    Dict* d = Dict_new(tempAlloc);
    rpcCall0(String_CONST("ping"), d, ctx, tempAlloc, &resp, false);
    if (!resp) { return -1; }
    String* q = Dict_getStringC(resp, "q");
    if (String_equals(q, String_CONST("pong"))) {
        return true;
    }
    return false;
}

static void awaken(void* vcontext)
{
    struct Context* ctx = vcontext;
    EventBase_endLoop(ctx->base);
}

static void sleep(int milliseconds, struct Context* ctx, struct Allocator* temp)
{
    Timeout_setTimeout(awaken, ctx, milliseconds, ctx->base, temp);
    EventBase_beginLoop(ctx->base);
}

static void waitUntilPong(struct Context* ctx)
{
    for (int i = 0; i < 10; i++) {
        struct Allocator* temp = Allocator_child(ctx->alloc);
        if (tryPing(temp, ctx)) {
            Allocator_free(temp);
            return;
        }
        sleep(200, ctx, temp);
        Allocator_free(temp);
    }
    Assert_failure("Failed connecting to core (perhaps you have a firewall on loopback device?)");
}

void Configurator_config(Dict* config,
                         struct Sockaddr* sockAddr,
                         String* adminPassword,
                         struct EventBase* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct UDPAddrIface* udp = UDPAddrIface_new(eventBase, NULL, alloc, NULL, logger);
    struct AdminClient* client =
        AdminClient_new(&udp->generic, sockAddr, adminPassword, eventBase, logger, tempAlloc);

    struct Context ctx = {
        .logger = logger,
        .alloc = tempAlloc,
        .client = client,
        .base = eventBase,
    };

    waitUntilPong(&ctx);

    List* authedPasswords = Dict_getListC(config, "authorizedPasswords");
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &ctx);
    }

    Dict* ifaces = Dict_getDictC(config, "interfaces");
    udpInterface(ifaces, &ctx);

    if (Defined(HAS_ETH_INTERFACE)) {
        ethInterface(ifaces, &ctx);
    }

    Dict* routerConf = Dict_getDictC(config, "router");
    routerConfig(routerConf, tempAlloc, &ctx);

    List* secList = Dict_getListC(config, "security");
    security(tempAlloc, secList, logger, &ctx);

    Log_debug(logger, "Cjdns started in the background");

    Allocator_free(tempAlloc);
}
