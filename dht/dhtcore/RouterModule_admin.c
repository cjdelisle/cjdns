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
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "memory/Allocator.h"
#include "util/AddrTools.h"

struct Context {
    struct Admin* admin;
    struct Allocator* allocator;
    struct RouterModule* router;
    Identity
};

static void lookup(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = vcontext;
    String* addrStr = Dict_getString(args, String_CONST("address"));
    char* err = NULL;
    uint8_t addr[16];
    uint8_t resultBuff[60];
    char* result = (char*) resultBuff;
    if (addrStr->len != 39) {
        err = "address wrong length";
    } else if (AddrTools_parseIp(addr, (uint8_t*) addrStr->bytes)) {
        err = "failed to parse address";
    } else {
        struct Node_Two* n = RouterModule_lookup(addr, ctx->router);
        if (!n) {
            result = "not found";
        } else if (Bits_memcmp(addr, n->address.ip6.bytes, 16)) {
            Address_print(resultBuff, &n->address);
        } else {
            AddrTools_printPath(resultBuff, n->address.path);
        }
    }
    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST((err) ? err : "none")), Dict_CONST(
        String_CONST("result"), String_OBJ(String_CONST(result)), NULL
    ));
    Admin_sendMessage(&response, txid, ctx->admin);
}

struct Ping
{
    String* txid;
    struct RouterModule_Promise* rp;
    struct Context* ctx;
    Identity
};

static void pingResponse(struct RouterModule_Promise* promise,
                         uint32_t lag,
                         struct Node_Two* node,
                         Dict* responseDict)
{
    struct Ping* ping = Identity_check((struct Ping*)promise->userData);

    uint8_t versionStr[40] = "old";
    String* version = String_CONST((char*)versionStr);
    String* versionBin = Dict_getString(responseDict, CJDHTConstants_VERSION);
    if (versionBin && versionBin->len == 20) {
        Hex_encode(versionStr, 40, (uint8_t*) versionBin->bytes, 20);
        version->len = 40;
    }
    int64_t* protocolVersion = Dict_getInt(responseDict, CJDHTConstants_PROTOCOL);
    int64_t pv = (protocolVersion) ? *protocolVersion : -1;

    Dict response = NULL;
    Dict verResponse = Dict_CONST(String_CONST("version"), String_OBJ(version), response);
    if (versionBin) {
        response = verResponse;
    }

    String* result = (responseDict) ? String_CONST("pong") : String_CONST("timeout");
    response = Dict_CONST(String_CONST("result"), String_OBJ(result), response);

    Dict protoResponse = Dict_CONST(String_CONST("protocol"), Int_OBJ(pv), response);
    if (protocolVersion) {
        response = protoResponse;
    }

    response = Dict_CONST(String_CONST("ms"), Int_OBJ(lag), response);

    char from[60] = "";
    if (node) {
        Address_print((uint8_t*)from, &node->address);
    }
    Dict fromResponse = Dict_CONST(String_CONST("from"), String_OBJ(String_CONST(from)), response);
    if (node) {
        response = fromResponse;
    }

    Admin_sendMessage(&response, ping->txid, ping->ctx->admin);
}

static void getPeersResponse(struct RouterModule_Promise* promise,
                             uint32_t lag,
                             struct Node_Two* node,
                             Dict* responseDict)
{
    struct Ping* ping = Identity_check((struct Ping*)promise->userData);
    Dict* out = Dict_new(promise->alloc);
    String* result = (responseDict) ? String_CONST("peers") : String_CONST("timeout");
    Dict_putString(out, String_CONST("result"), result, promise->alloc);

    if (responseDict) {
        struct Address_List* addrs =
            ReplySerializer_parse(&node->address, responseDict, NULL, promise->alloc);

        List* nodes = NULL;
        for (int i = 0; i < addrs->length; i++) {
            String* addr = Address_toString(&addrs->elems[i], promise->alloc);
            nodes = List_addString(nodes, addr, promise->alloc);
        }
        Dict_putList(out, String_CONST("peers"), nodes, promise->alloc);
    }

    Dict_putInt(out, String_CONST("ms"), lag, promise->alloc);

    Dict_putString(out, String_CONST("error"), String_CONST("none"), promise->alloc);

    Admin_sendMessage(out, ping->txid, ping->ctx->admin);
}

static struct Address* getNode(String* pathStr,
                               struct Context* ctx,
                               char** errOut,
                               struct Allocator* alloc)
{
    struct Address addr = {.path=0};

    if (pathStr->len == 19 && !AddrTools_parsePath(&addr.path, pathStr->bytes)) {
        struct Node_Two* n = RouterModule_nodeForPath(addr.path, ctx->router);
        if (!n) {
            *errOut = "not_found";
            return NULL;
        } else {
            Bits_memcpyConst(&addr, &n->address, sizeof(struct Address));
        }
    } else if (pathStr->len == 39 && !AddrTools_parseIp(addr.ip6.bytes, pathStr->bytes)) {
        struct Node_Two* n = RouterModule_lookup(addr.ip6.bytes, ctx->router);
        if (!n || Bits_memcmp(addr.ip6.bytes, n->address.ip6.bytes, 16)) {
            *errOut = "not_found";
            return NULL;
        } else {
            Bits_memcpyConst(&addr, &n->address, sizeof(struct Address));
        }
    } else {
        struct Address* a = Address_fromString(pathStr, alloc);
        if (a) { return a; }
        *errOut = "parse_path";
        return NULL;
    }

    return Allocator_clone(alloc, &addr);
}

static void pingNode(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    String* pathStr = Dict_getString(args, String_CONST("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    uint32_t timeout = (timeoutPtr && *timeoutPtr > 0) ? *timeoutPtr : 0;

    char* err = NULL;
    struct Address* addr = getNode(pathStr, ctx, &err, requestAlloc);

    if (err) {
        Dict errDict = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
        Admin_sendMessage(&errDict, txid, ctx->admin);
        return;
    }

    struct RouterModule_Promise* rp =
        RouterModule_pingNode(addr, timeout, ctx->router, ctx->allocator);
    struct Ping* ping = Allocator_calloc(rp->alloc, sizeof(struct Ping), 1);
    Identity_set(ping);
    ping->txid = String_clone(txid, rp->alloc);
    ping->rp = rp;
    ping->ctx = ctx;
    rp->userData = ping;
    rp->callback = pingResponse;
}

static void getPeers(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vctx);
    String* nearbyLabelStr = Dict_getString(args, String_CONST("nearbyLabel"));
    String* pathStr = Dict_getString(args, String_CONST("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    uint32_t timeout = (timeoutPtr && *timeoutPtr > 0) ? *timeoutPtr : 0;

    char* err = NULL;
    struct Address* addr = getNode(pathStr, ctx, &err, requestAlloc);

    uint64_t nearbyLabel = 0;
    if (!err && nearbyLabelStr) {
        if (nearbyLabelStr->len != 19 || AddrTools_parsePath(&nearbyLabel, nearbyLabelStr->bytes)) {
            err = "parse_nearbyLabel";
        }
    }

    if (err) {
        Dict errDict = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
        Admin_sendMessage(&errDict, txid, ctx->admin);
        return;
    }

    struct RouterModule_Promise* rp =
        RouterModule_getPeers(addr, nearbyLabel, timeout, ctx->router, ctx->allocator);

    struct Ping* ping = Allocator_calloc(rp->alloc, sizeof(struct Ping), 1);
    Identity_set(ping);
    ping->txid = String_clone(txid, rp->alloc);
    ping->rp = rp;
    ping->ctx = ctx;
    rp->userData = ping;
    rp->callback = getPeersResponse;
}

void RouterModule_admin_register(struct RouterModule* module,
                                 struct Admin* admin,
                                 struct Allocator* alloc)
{
    // for improved reporting
    alloc = Allocator_child(alloc);
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .allocator = alloc,
        .router = module
    }));
    Identity_set(ctx);

    Admin_registerFunction("RouterModule_lookup", lookup, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "address", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("RouterModule_pingNode", pingNode, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "timeout", .required = 0, .type = "Int" },
        }), admin);

    Admin_registerFunction("RouterModule_getPeers", getPeers, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "timeout", .required = 0, .type = "Int" },
            { .name = "nearbyPath", .required = 0, .type = "String" }
        }), admin);
}
