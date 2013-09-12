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
#include "dht/dhtcore/NodeStore_admin.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_pvt.h"
#include "dht/Address.h"
#include "dht/CJDHTConstants.h"
#include "memory/Allocator.h"

struct Context {
    struct Admin* admin;
    struct Allocator* allocator;
    struct RouterModule* router;
    Identity
};

static void lookup(Dict* args, void* vcontext, String* txid)
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
        struct Node* n = RouterModule_lookup(addr, ctx->router);
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
                         struct Node* node,
                         Dict* responseDict)
{
    struct Ping* ping = Identity_cast((struct Ping*)promise->userData);

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

static void pingNode(Dict* args, void* vctx, String* txid)
{
    struct Context* ctx = Identity_cast((struct Context*) vctx);
    String* pathStr = Dict_getString(args, String_CONST("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    uint32_t timeout = (timeoutPtr && *timeoutPtr > 0) ? *timeoutPtr : 0;

    char* err = NULL;

    struct Address addr = {.path=0};
    struct Node* n = NULL;

    if (pathStr->len == 19 && !AddrTools_parsePath(&addr.path, (uint8_t*) pathStr->bytes)) {
        n = RouterModule_getNode(addr.path, ctx->router);
    } else if (!AddrTools_parseIp(addr.ip6.bytes, (uint8_t*) pathStr->bytes)) {
        n = RouterModule_lookup(addr.ip6.bytes, ctx->router);
        if (n && Bits_memcmp(addr.ip6.bytes, n->address.ip6.bytes, 16)) {
            n = NULL;
        }
    } else {
        err = "Unexpected address, must be either an ipv6 address "
              "eg: 'fc4f:d:e499:8f5b:c49f:6e6b:1ae:3120', 19 char path eg: '0123.4567.89ab.cdef'";
    }

    if (!err) {
        if (!n) {
            err = "could not find node to ping";
        } else {
            struct RouterModule_Promise* rp =
                RouterModule_pingNode(n, timeout, ctx->router, ctx->allocator);
            struct Ping* ping = Allocator_calloc(rp->alloc, sizeof(struct Ping), 1);
            Identity_set(ping);
            ping->txid = String_clone(txid, rp->alloc);
            ping->rp = rp;
            ping->ctx = ctx;
            rp->userData = ping;
            rp->callback = pingResponse;
        }
    }

    if (err) {
        Dict errDict = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
        Admin_sendMessage(&errDict, txid, ctx->admin);
    }
}

void RouterModule_admin_register(struct RouterModule* module,
                                 struct Admin* admin,
                                 struct Allocator* alloc)
{
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

    NodeStore_admin_register(module->nodeStore, admin, alloc);
}
