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
#include "crypto/Key.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeStore_admin.h"
#include "memory/Allocator.h"
#include "switch/EncodingScheme.h"
#include "util/AddrTools.h"
#include "util/version/Version.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct NodeStore* store;
    Identity
};

static void dumpTable_send(struct Context* ctx,
                           struct List_Item* last,
                           bool isMore,
                           String* txid)
{
    Dict table = Dict_CONST(String_CONST("routingTable"), List_OBJ(&last), NULL);
    if (isMore) {
        table = Dict_CONST(String_CONST("more"), Int_OBJ(1), table);
    } else {
        int count = ctx->store->nodeCount;
        table = Dict_CONST(String_CONST("count"), Int_OBJ(count), table);
    }
    Admin_sendMessage(&table, txid, ctx->admin);
}

#define ENTRIES_PER_PAGE 8
static void dumpTable_addEntries(struct Context* ctx,
                                 int i,
                                 int j,
                                 struct List_Item* last,
                                 String* txid)
{
    uint8_t path[20];
    uint8_t ip[40];
    String* pathStr = &(String) { .len = 19, .bytes = (char*)path };
    String* ipStr = &(String) { .len = 39, .bytes = (char*)ip };
    Object* link = Int_OBJ(0xFFFFFFFF);
    Object* version = Int_OBJ(Version_DEFAULT_ASSUMPTION);
    Dict entry = Dict_CONST(
        String_CONST("ip"), String_OBJ(ipStr), Dict_CONST(
        String_CONST("link"), link, Dict_CONST(
        String_CONST("path"), String_OBJ(pathStr), Dict_CONST(
        String_CONST("version"), version, NULL
    ))));

    struct List_Item next = { .next = last, .elem = Dict_OBJ(&entry) };

    if (i >= ctx->store->nodeCount || j >= ENTRIES_PER_PAGE) {
        dumpTable_send(ctx, last, (j >= ENTRIES_PER_PAGE), txid);
        return;
    }

    struct Node_Two* n = NodeStore_dumpTable(ctx->store, i);
    link->as.number = Node_getReach(n);
    version->as.number = n->address.protocolVersion;
    Address_printIp(ip, &n->address);
    AddrTools_printPath(path, n->address.path);

    dumpTable_addEntries(ctx, i + 1, j + 1, &next, txid);
}

static void dumpTable(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    dumpTable_addEntries(ctx, i, 0, NULL, txid);
}

static int linkCount(struct Node_Two* parent)
{
    struct Node_Link* link = NULL;
    int i = 0;
    do {
        link = NodeStore_nextLink(parent, link);
        i++;
    } while (link);
    return i;
}

static struct Node_Link* getLinkByNum(struct Node_Two* parent, int linkNum)
{
    struct Node_Link* link = NULL;
    for (int i = 0; i <= linkNum; i++) {
        link = NodeStore_nextLink(parent, link);
        if (!link) { break; }
    }
    return link;
}

static void getLink(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);

    struct Node_Link* link = NULL;
    struct Node_Two* node = NULL;

    String* ipStr = Dict_getString(args, String_new("parent", alloc));
    int64_t* linkNum = Dict_getInt(args, String_new("linkNum", alloc));
    uint8_t ip[16];
    if (ipStr->len != 39 || AddrTools_parseIp(ip, ipStr->bytes)) {
        Dict_remove(ret, String_CONST("result"));
        Dict_putString(ret,
                       String_new("error", alloc),
                       String_new("parse_parent", alloc),
                       alloc);

    } else if (!(node = NodeStore_nodeForAddr(ctx->store, ip))) {
        Dict_putString(ret,
                       String_new("error", alloc),
                       String_new("not_found", alloc),
                       alloc);

    } else if ((link = getLinkByNum(node, *linkNum))) {
        Dict_putInt(result,
                    String_new("inverseLinkEncodingFormNumber", alloc),
                    link->inverseLinkEncodingFormNumber,
                    alloc);
        Dict_putInt(result, String_new("linkState", alloc), link->linkState, alloc);

        Dict_putInt(result, String_new("isOneHop", alloc), Node_isOneHopLink(link), alloc);

        String* cannonicalLabel = String_newBinary(NULL, 19, alloc);
        AddrTools_printPath(cannonicalLabel->bytes, link->cannonicalLabel);
        Dict_putString(result, String_new("cannonicalLabel", alloc), cannonicalLabel, alloc);

        String* parent = String_newBinary(NULL, 39, alloc);
        AddrTools_printIp(parent->bytes, link->parent->address.ip6.bytes);
        Dict_putString(result, String_new("parent", alloc), parent, alloc);

        String* child = String_newBinary(NULL, 39, alloc);
        AddrTools_printIp(child->bytes, link->child->address.ip6.bytes);
        Dict_putString(result, String_new("child", alloc), child, alloc);
    }

    Admin_sendMessage(ret, txid, ctx->admin);
}
static void nodeForAddr(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);

    // no ipStr specified --> return self-node
    struct Node_Two* node = ctx->store->selfNode;

    String* ipStr = Dict_getString(args, String_new("ip", alloc));
    uint8_t ip[16];
    while (ipStr) {
        if (ipStr->len != 39 || AddrTools_parseIp(ip, ipStr->bytes)) {
            Dict_remove(ret, String_CONST("result"));
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("parse_ip", alloc),
                           alloc);

        } else if (!(node = NodeStore_nodeForAddr(ctx->store, ip))) {
            // not found
        } else {
            break;
        }

        Admin_sendMessage(ret, txid, ctx->admin);
        return;
    }

    Dict_putInt(result, String_new("protocolVersion", alloc), node->address.protocolVersion, alloc);

    String* key = Key_stringify(node->address.key, alloc);
    Dict_putString(result, String_new("key", alloc), key, alloc);

    uint32_t count = linkCount(node);
    Dict_putInt(result, String_new("linkCount", alloc), count, alloc);

    Dict_putInt(result, String_new("reach", alloc), Node_getReach(node), alloc);

    List* encScheme = EncodingScheme_asList(node->encodingScheme, alloc);
    Dict_putList(result, String_new("encodingScheme", alloc), encScheme, alloc);

    Dict* bestParent = Dict_new(alloc);
    String* parentIp = String_newBinary(NULL, 39, alloc);
    AddrTools_printIp(parentIp->bytes, Node_getBestParent(node)->parent->address.ip6.bytes);
    Dict_putString(bestParent, String_CONST("ip"), parentIp, alloc);

    String* parentChildLabel = String_newBinary(NULL, 19, alloc);
    AddrTools_printPath(parentChildLabel->bytes, Node_getBestParent(node)->cannonicalLabel);
    Dict_putString(bestParent, String_CONST("parentChildLabel"), parentChildLabel, alloc);

    Dict_putDict(result, String_CONST("bestParent"), bestParent, alloc);

    String* bestLabel = String_newBinary(NULL, 19, alloc);
    AddrTools_printPath(bestLabel->bytes, node->address.path);
    Dict_putString(result, String_CONST("routeLabel"), bestLabel, alloc);

    Admin_sendMessage(ret, txid, ctx->admin);
}

static void getRouteLabel(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);

    char* err = NULL;

    String* pathToParentS = Dict_getString(args, String_CONST("pathToParent"));
    uint64_t pathToParent = 0;
    if (pathToParentS->len != 19 || AddrTools_parsePath(&pathToParent, pathToParentS->bytes)) {
        err = "parse_pathToParent";
    }

    String* pathParentToChildS = Dict_getString(args, String_CONST("pathParentToChild"));
    uint64_t pathParentToChild = 0;
    if (pathParentToChildS->len != 19
        || AddrTools_parsePath(&pathParentToChild, pathParentToChildS->bytes))
    {
        err = "parse_pathParentToChild";
    }

    uint64_t label = UINT64_MAX;
    if (!err) {
        label = NodeStore_getRouteLabel(ctx->store, pathToParent, pathParentToChild);
        err = NodeStore_getRouteLabel_strerror(label);
    }
    Dict* response = Dict_new(requestAlloc);
    if (!err) {
        String* printedPath = String_newBinary(NULL, 19, requestAlloc);
        AddrTools_printPath(printedPath->bytes, label);
        Dict_putString(response, String_new("result", requestAlloc), printedPath, requestAlloc);
        Dict_putString(response,
                       String_new("error", requestAlloc),
                       String_new("none", requestAlloc),
                       requestAlloc);
        Admin_sendMessage(response, txid, ctx->admin);
    } else {
        Dict_putString(response,
                       String_new("error", requestAlloc),
                       String_new(err, requestAlloc),
                       requestAlloc);
        Admin_sendMessage(response, txid, ctx->admin);
    }
}

void NodeStore_admin_register(struct NodeStore* nodeStore,
                              struct Admin* admin,
                              struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .store = nodeStore
    }));
    Identity_set(ctx);

    Admin_registerFunction("NodeStore_dumpTable", dumpTable, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = 1, .type = "Int" },
        }), admin);

    Admin_registerFunction("NodeStore_getLink", getLink, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "parent", .required = 1, .type = "String" },
            { .name = "linkNum", .required = 1, .type = "Int" },
        }), admin);
    Admin_registerFunction("NodeStore_nodeForAddr", nodeForAddr, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ip", .required = 0, .type = "String" },
        }), admin);
    Admin_registerFunction("NodeStore_getRouteLabel", getRouteLabel, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "pathToParent", .required = 1, .type = "String" },
            { .name = "pathParentToChild", .required = 1, .type = "String" }
        }), admin);
}
