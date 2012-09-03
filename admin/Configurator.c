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
#include "util/Log.h"

#include <event2/event.h>


struct Context
{
    struct Log* logger;
    struct Allocator* alloc;
    struct AdminClient* client;
};

static void showMsg(struct AdminClient_Result* res, struct Context* ctx)
{
    Log_keys(ctx->logger, "message bytes = [%s]", res->messageBytes);
    #ifndef Log_KEYS
        Log_critical(ctx->logger, "enable Log_LEVEL=KEYS to see message content.");
    #endif
}

static void rpcCall(String* function, Dict* args, struct Context* ctx, struct Allocator* alloc)
{
    struct AdminClient_Result* res = AdminClient_rpcCall(function, args, ctx->client, alloc);
    if (res->err) {
        Log_critical(ctx->logger,
                      "Failed to make function call [%s], error: [%s]",
                      AdminClient_errorString(res->err),
                      function->bytes);
        showMsg(res, ctx);
        exit(-1);
    }
    String* error = Dict_getString(res->responseDict, String_CONST("error"));
    if (error && !String_equals(error, String_CONST("none"))) {
        Log_critical(ctx->logger,
                      "Router responses with error: [%s]\nCalling function: [%s]",
                      error->bytes,
                      function->bytes);
        showMsg(res, ctx);
        exit(-1);
    }
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
        struct Allocator* child = ctx->alloc->child(ctx->alloc);
        rpcCall(String_CONST("AuthorizedPasswords_add"), &args, ctx, child);
        child->free(child);
    }
}

static void udpInterface(Dict* config, struct Context* ctx)
{
    Dict* udp = Dict_getDict(config, String_CONST("UDPInterface"));

    if (udp) {
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

                struct Allocator* perCallAlloc = ctx->alloc->child(ctx->alloc);
                Dict_putString(value, String_CONST("address"), key, perCallAlloc);
                rpcCall(String_CONST("UDPInterface_beginConnection"), value, ctx, perCallAlloc);
                perCallAlloc->free(perCallAlloc);

                entry = entry->next;
            }
        }
    }
}

static void ethInterface(Dict* config, struct Context* ctx)
{
    Dict* eth = Dict_getDict(config, String_CONST("ETHInterface"));

    if (eth) {
        // Setup the interface.
        String* deviceStr = Dict_getString(eth, String_CONST("bind"));
        Dict* d = Dict_new(ctx->alloc);
        if (deviceStr) {
            Dict_putString(d, String_CONST("bindDevice"), deviceStr, ctx->alloc);
        }
        rpcCall(String_CONST("ETHInterface_new"), d, ctx, ctx->alloc);

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

                struct Allocator* perCallAlloc = ctx->alloc->child(ctx->alloc);
                Dict_putString(value, String_CONST("device"), key, perCallAlloc);
                rpcCall(String_CONST("ETHInterface_beginConnection"), value, ctx, perCallAlloc);
                perCallAlloc->free(perCallAlloc);

                entry = entry->next;
            }
        }
    }
}

void Configurator_config(Dict* config,
                         struct sockaddr_storage* addr,
                         int addrLen,
                         String* adminPassword,
                         struct event_base* eventBase,
                         struct Log* logger,
                         struct Allocator* alloc)
{
    struct Allocator* tempAlloc = alloc->child(alloc);
    struct AdminClient* client =
        AdminClient_new(addr, addrLen, adminPassword, eventBase, logger, tempAlloc);

    struct Context ctx = { .logger = logger, .alloc = tempAlloc, .client = client };

    List* authedPasswords = Dict_getList(config, String_CONST("authorizedPasswords"));
    if (authedPasswords) {
        authorizedPasswords(authedPasswords, &ctx);
    }

    Dict* ifaces = Dict_getDict(config, String_CONST("interfaces"));
    udpInterface(ifaces, &ctx);
    ethInterface(ifaces, &ctx);

    tempAlloc->free(tempAlloc);
}
