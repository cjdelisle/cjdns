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
#include "admin/AdminClient.h"
#include "admin/Configurator.h"
#include "benc/String.h"
#include "benc/Dict.h"
#include "benc/Int.h"
#include "memory/Allocator.h"
#include "util/events/Event.h"
#include "util/platform/libc/strlen.h"
#include "util/log/Log.h"

struct Context
{
    struct Log* logger;
    struct Allocator* alloc;
    struct AdminClient* client;
};

static void die(struct AdminClient_Result* res, struct Context* ctx, struct Allocator* alloc)
{
    Log_keys(ctx->logger, "message bytes = [%s]", res->messageBytes);
    #ifndef Log_KEYS
        Log_critical(ctx->logger, "enable Log_LEVEL=KEYS to see message content.");
    #endif

    Dict d = NULL;
    struct AdminClient_Result* exitRes =
        AdminClient_rpcCall(String_CONST("Core_exit"), &d, ctx->client, alloc);

    if (exitRes->err) {
        Log_critical(ctx->logger, "Failed to stop the core.");
    }
    Log_critical(ctx->logger, "Aborting.");
    exit(1);
}

static int rpcCall0(String* function,
                    Dict* args,
                    struct Context* ctx,
                    struct Allocator* alloc,
                    bool exitIfError)
{
    struct AdminClient_Result* res = AdminClient_rpcCall(function, args, ctx->client, alloc);
    if (res->err) {
        Log_critical(ctx->logger,
                      "Failed to make function call [%s], error: [%s]",
                      AdminClient_errorString(res->err),
                      function->bytes);
        die(res, ctx, alloc);
    }
    String* error = Dict_getString(res->responseDict, String_CONST("error"));
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
        return 1;
    }
    return 0;
}

static void rpcCall(String* function, Dict* args, struct Context* ctx, struct Allocator* alloc)
{
    rpcCall0(function, args, ctx, alloc, true);
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
        String* passwd = Dict_getString(d, String_CONST("password"));
        if (!passwd) {
            Log_critical(ctx->logger, "Must specify a password %d.", i);
            exit(-1);
        }
    }

    Log_info(ctx->logger, "Flushing existing authorized passwords");
    rpcCall(String_CONST("AuthorizedPasswords_flush"), NULL, ctx, ctx->alloc);

    for (uint32_t i = 0; i < count; i++) {
        Dict* d = List_getDict(list, i);
        String* passwd = Dict_getString(d, String_CONST("password"));
        Log_info(ctx->logger, "Adding authorized password #[%d].", i);

        Dict args = Dict_CONST(
            String_CONST("authType"), Int_OBJ(1), Dict_CONST(
            String_CONST("password"), String_OBJ(passwd), NULL
        ));
        struct Allocator* child = Allocator_child(ctx->alloc);
        rpcCall(String_CONST("AuthorizedPasswords_add"), &args, ctx, child);
        Allocator_free(child);
    }
}

static void udpInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getList(config, String_CONST("UDPInterface"));
    if (!ifaces) {
        ifaces = List_addDict(ifaces,
                Dict_getDict(config, String_CONST("UDPInterface")), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);
    for (uint32_t i = 0; i < count; i++) {
        Dict *udp = List_getDict(ifaces, i);
        if (!udp) {
            continue;
        }
        // Setup the interface.
        String* bindStr = Dict_getString(udp, String_CONST("bind"));
        Dict* d = Dict_new(ctx->alloc);
        if (bindStr) {
            Dict_putString(d, String_CONST("bindAddress"), bindStr, ctx->alloc);
        }
        rpcCall(String_CONST("UDPInterface_new"), d, ctx, ctx->alloc);

        // Make the connections.
        Dict* connectTo = Dict_getDict(udp, String_CONST("connectTo"));
        if (connectTo) {
            struct Dict_Entry* entry = *connectTo;
            while (entry != NULL) {
                String* key = (String*) entry->key;
                if (entry->val->type != Object_DICT) {
                    Log_critical(ctx->logger, "interfaces.UDPInterface.connectTo: entry [%s] "
                                               "is not a dictionary type.", key->bytes);
                    exit(-1);
                }
                Dict* value = entry->val->as.dictionary;

                Log_keys(ctx->logger, "Attempting to connect to node [%s].", key->bytes);

                struct Allocator* perCallAlloc = Allocator_child(ctx->alloc);
                Dict_putString(value, String_CONST("address"), key, perCallAlloc);
                rpcCall(String_CONST("UDPInterface_beginConnection"), value, ctx, perCallAlloc);
                Allocator_free(perCallAlloc);

                entry = entry->next;
            }
        }
    }
}

static void tunInterface(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    String* ifaceType = Dict_getString(ifaceConf, String_CONST("type"));
    if (!String_equals(ifaceType, String_CONST("TUNInterface"))) {
        return;
    }

    // Setup the interface.
    String* device = Dict_getString(ifaceConf, String_CONST("tunDevice"));

    Dict* args = Dict_new(tempAlloc);
    if (device) {
        Dict_putString(args, String_CONST("desiredTunName"), device, tempAlloc);
    }
    rpcCall0(String_CONST("Core_initTunnel"), args, ctx, tempAlloc, false);
}

static void ipTunnel(Dict* ifaceConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    List* incoming = Dict_getList(ifaceConf, String_CONST("allowedConnections"));
    Dict* d;
    for (int i = 0; (d = List_getDict(incoming, i)) != NULL; i++) {
        String* key = Dict_getString(d, String_CONST("publicKey"));
        String* ip4 = Dict_getString(d, String_CONST("ip4Address"));
        String* ip6 = Dict_getString(d, String_CONST("ip6Address"));
        if (!key) {
            Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                      "'publicKey' required.", i);
            exit(1);
        }
        if (!ip4 && !ip6) {
            Log_critical(ctx->logger, "In router.ipTunnel.allowedConnections[%d]"
                                       "either ip4Address or ip6Address required.", i);
            exit(1);
        }
        Log_debug(ctx->logger, "Allowing IpTunnel connections from [%s]", key->bytes);
        Dict_putString(d, String_CONST("publicKeyOfAuthorizedNode"), key, tempAlloc);
        rpcCall0(String_CONST("IpTunnel_allowConnection"), d, ctx, tempAlloc, true);
    }

    List* outgoing = Dict_getList(ifaceConf, String_CONST("outgoingConnections"));
    String* s;
    for (int i = 0; (s = List_getString(outgoing, i)) != NULL; i++) {
        Log_debug(ctx->logger, "Initiating IpTunnel connection to [%s]", s->bytes);
        Dict requestDict =
            Dict_CONST(String_CONST("publicKeyOfNodeToConnectTo"), String_OBJ(s), NULL);
        rpcCall0(String_CONST("IpTunnel_connectTo"), &requestDict, ctx, tempAlloc, true);
    }
}

static void routerConfig(Dict* routerConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    tunInterface(Dict_getDict(routerConf, String_CONST("interface")), tempAlloc, ctx);
    ipTunnel(Dict_getDict(routerConf, String_CONST("ipTunnel")), tempAlloc, ctx);
}

#ifdef HAS_ETH_INTERFACE
static void ethInterface(Dict* config, struct Context* ctx)
{
    List* ifaces = Dict_getList(config, String_CONST("ETHInterface"));
    if (!ifaces) {
        ifaces = List_addDict(ifaces,
                              Dict_getDict(config, String_CONST("ETHInterface")), ctx->alloc);
    }

    uint32_t count = List_size(ifaces);
    for (uint32_t i = 0; i < count; i++) {
        Dict *eth = List_getDict(ifaces, i);
        if (!eth) {
            continue;
        }
        // Setup the interface.
        String* deviceStr = Dict_getString(eth, String_CONST("bind"));
        Dict* d = Dict_new(ctx->alloc);
        if (deviceStr) {
            Dict_putString(d, String_CONST("bindDevice"), deviceStr, ctx->alloc);
        }
        if (rpcCall0(String_CONST("ETHInterface_new"), d, ctx, ctx->alloc, false)) {
            continue;
        }

        // Make the connections.
        Dict* connectTo = Dict_getDict(eth, String_CONST("connectTo"));
        if (connectTo) {
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
                Dict_putString(value, String_CONST("macAddress"), key, perCallAlloc);
                rpcCall(String_CONST("ETHInterface_beginConnection"), value, ctx, perCallAlloc);
                Allocator_free(perCallAlloc);

                entry = entry->next;
            }
        }

        int64_t* beaconP = Dict_getInt(eth, String_CONST("beacon"));
        if (beaconP) {
            int64_t beacon = *beaconP;
            if (beacon > 3 || beacon < 0) {
                Log_error(ctx->logger, "interfaces.ETHInterface.beacon may only be 0, 1,or 2");
            } else {
                Dict d = Dict_CONST(String_CONST("state"), Int_OBJ(beacon), NULL);
                rpcCall(String_CONST("ETHInterface_beacon"), &d, ctx, ctx->alloc);
            }
        }
    }
}
#endif

static void security(List* securityConf, struct Allocator* tempAlloc, struct Context* ctx)
{
    bool noFiles = false;
    for (int i = 0; i < List_size(securityConf); i++) {
        if (String_equals(String_CONST("nofiles"), List_getString(securityConf, i))) {
            noFiles = true;
        } else {
            Dict* userDict = List_getDict(securityConf, i);
            String* userName = Dict_getString(userDict, String_CONST("setuser"));
            if (userName) {
                Dict d = Dict_CONST(String_CONST("user"), String_OBJ(userName), NULL);
                // If this call returns an error, it is ok.
                rpcCall0(String_CONST("Security_setUser"), &d, ctx, tempAlloc, false);
            }
        }
    }
    if (noFiles) {
        Dict d = NULL;
        rpcCall(String_CONST("Security_noFiles"), &d, ctx, tempAlloc);
    }
}

void Configurator_config(Dict* config,
                         struct Sockaddr* sockAddr,
                         String* adminPassword,
                         struct EventBase* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc)
{
    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct AdminClient* client =
        AdminClient_new(sockAddr, adminPassword, eventBase, logger, tempAlloc);

    struct Context ctx = { .logger = logger, .alloc = tempAlloc, .client = client };

    List* authedPasswords = Dict_getList(config, String_CONST("authorizedPasswords"));
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &ctx);
    }

    Dict* ifaces = Dict_getDict(config, String_CONST("interfaces"));
    udpInterface(ifaces, &ctx);

    #ifdef HAS_ETH_INTERFACE
        ethInterface(ifaces, &ctx);
    #endif

    Dict* routerConf = Dict_getDict(config, String_CONST("router"));
    routerConfig(routerConf, tempAlloc, &ctx);

    List* securityList = Dict_getList(config, String_CONST("security"));
    security(securityList, tempAlloc, &ctx);

    Allocator_free(tempAlloc);
}
