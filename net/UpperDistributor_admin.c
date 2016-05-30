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
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/List.h"
#include "admin/Admin.h"
#include "memory/Allocator.h"
#include "net/UpperDistributor.h"
#include "net/UpperDistributor_admin.h"

struct Context
{
    struct Admin* admin;
    struct Allocator* alloc;
    struct UpperDistributor* ud;
    Identity
};

static void sendError(struct Context* ctx, String* txid, struct Allocator* requestAlloc, char* err)
{
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", err, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void unregisterHandler(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    int64_t* udpPort = Dict_getIntC(args, "udpPort");
    char* err;
    if (*udpPort < 1 && *udpPort > 65536) {
        err = "invalid_udpPort";
    } else {
        int ret = UpperDistributor_unregisterHandler(ctx->ud, (int) *udpPort);
        if (!ret) {
            err = "none";
        } else if (ret == UpperDistributor_unregisterHandler_NONEXISTANT) {
            err = "UpperDistributor_unregisterHandler_NONEXISTANT";
        } else {
            err = "UNKNOWN";
        }
    }
    sendError(ctx, txid, requestAlloc, err);
}

#define HANDLERS_PER_PAGE 8
static void listHandlers(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    int page = 0;
    int64_t* pageP = Dict_getIntC(args, "page");
    if (pageP && *pageP > 0) { page = *pageP; }

    struct UpperDistributor_Handler* handlers;
    int count = UpperDistributor_listHandlers(ctx->ud, &handlers, requestAlloc);
    List* handlerList = List_new(requestAlloc);
    for (int i = page * HANDLERS_PER_PAGE, j = 0; i < count && j < HANDLERS_PER_PAGE; i++, j++) {
        Dict* d = Dict_new(requestAlloc);
        Dict_putIntC(d, "udpPort", handlers[i].udpPort, requestAlloc);
        Dict_putIntC(d, "type", handlers[i].type, requestAlloc);
        List_addDict(handlerList, d, requestAlloc);
    }
    Dict* out = Dict_new(requestAlloc);
    Dict_putListC(out, "handlers", handlerList, requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void registerHandler(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    int64_t* udpPort = Dict_getIntC(args, "udpPort");
    if (*udpPort < 1 || *udpPort > 65535) {
        sendError(ctx, txid, requestAlloc, "invalid_udpPort");
        return;
    }
    int64_t* contentTypeP = Dict_getIntC(args, "contentType");
    if (*contentTypeP < 0 || *contentTypeP > ContentType_MAX) {
        sendError(ctx, txid, requestAlloc, "invalid_contentType");
        return;
    }
    enum ContentType ct = (enum ContentType) *contentTypeP;

    int ret = UpperDistributor_registerHandler(ctx->ud, ct, (int) *udpPort);
    if (ret < 0) {
        char* err = "UNKNOWN";
        if (ret == UpperDistributor_registerHandler_PORT_REGISTERED) {
            err = "UpperDistributor_registerHandler_PORT_REGISTERED";
        }
        sendError(ctx, txid, requestAlloc, err);
        return;
    }
    sendError(ctx, txid, requestAlloc, "none");
}

void UpperDistributor_admin_register(struct UpperDistributor* ud,
                                     struct Admin* admin,
                                     struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .ud = ud
    }));
    Identity_set(ctx);

    Admin_registerFunction("UpperDistributor_unregisterHandler", unregisterHandler, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "udpPort", .required = true, .type = "Int" }
        }), admin);
    Admin_registerFunction("UpperDistributor_listHandlers", listHandlers, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = false, .type = "Int" }
        }), admin);
    Admin_registerFunction("UpperDistributor_registerHandler", registerHandler, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "contentType", .required = true, .type = "Int" },
            { .name = "udpPort", .required = true, .type = "Int" },
        }), admin);
}
