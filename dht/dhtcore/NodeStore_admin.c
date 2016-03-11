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


#define ENTRIES_PER_PAGE 4
static void dumpTable(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int ctr = (page) ? *page * ENTRIES_PER_PAGE : 0;

    Dict* out = Dict_new(requestAlloc);
    List* table = List_new(requestAlloc);
    struct Node_Two* nn = NULL;
    for (int i = 0; i < ctr+ENTRIES_PER_PAGE; i++) {
        nn = NodeStore_getNextNode(ctx->store, nn);
        if (!nn) { break; }
        if (i < ctr) { continue; }
        Dict* nodeDict = Dict_new(requestAlloc);

        String* ip = String_newBinary(NULL, 39, requestAlloc);
        Address_printIp(ip->bytes, &nn->address);
        Dict_putString(nodeDict, String_CONST("ip"), ip, requestAlloc);

        String* addr = Address_toString(&nn->address, requestAlloc);
        Dict_putString(nodeDict, String_CONST("addr"), addr, requestAlloc);

        String* path = String_newBinary(NULL, 19, requestAlloc);
        AddrTools_printPath(path->bytes, nn->address.path);
        Dict_putString(nodeDict, String_CONST("path"), path, requestAlloc);

        Dict_putInt(nodeDict, String_CONST("link"), Node_getCost(nn), requestAlloc);
        Dict_putInt(nodeDict, String_CONST("version"), nn->address.protocolVersion, requestAlloc);

        Dict_putInt(nodeDict,
                    String_CONST("time"),
                    NodeStore_timeSinceLastPing(ctx->store, nn),
                    requestAlloc);

        Dict_putInt(nodeDict,
                    String_CONST("bucket"),
                    NodeStore_bucketForAddr(ctx->store->selfAddress, &nn->address),
                    requestAlloc);

        List_addDict(table, nodeDict, requestAlloc);
    }
    Dict_putList(out, String_CONST("routingTable"), table, requestAlloc);

    if (nn) {
        Dict_putInt(out, String_CONST("more"), 1, requestAlloc);
    }
    Dict_putInt(out, String_CONST("count"), ctx->store->nodeCount, requestAlloc);
    Dict_putInt(out, String_CONST("peers"), ctx->store->peerCount, requestAlloc);

    Dict_putString(out, String_CONST("deprecation"),
        String_CONST("ip,path,version will soon be removed"), requestAlloc);

    Admin_sendMessage(out, txid, ctx->admin);
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
    if (ipStr && ipStr->len) {
        uint8_t ip[16];
        if (AddrTools_parseIp(ip, ipStr->bytes)) {
            Dict_remove(ret, String_CONST("result"));
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("parse_parent", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;

        } else if (!(node = NodeStore_nodeForAddr(ctx->store, ip))) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("not_found", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        } else if (!(link = getLinkByNum(node, *linkNum))) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("unknown", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        }
    } else {
        for (int i = 0; i <= *linkNum; i++) {
            link = NodeStore_getNextLink(ctx->store, link);
            if (!link) { break; }
        }
        if (!link) {
            Dict_putString(ret,
                           String_new("error", alloc),
                           String_new("not_found", alloc),
                           alloc);
            Admin_sendMessage(ret, txid, ctx->admin);
            return;
        }
    }

    Dict_putInt(result,
                String_new("inverseLinkEncodingFormNumber", alloc),
                link->inverseLinkEncodingFormNumber,
                alloc);
    Dict_putInt(result, String_new("linkCost", alloc), link->linkCost, alloc);

    Dict_putInt(result, String_new("isOneHop", alloc), Node_isOneHopLink(link), alloc);

    int bestParent = (Node_getBestParent(link->child) == link);
    Dict_putInt(result, String_new("bestParent", alloc), bestParent, alloc);

    String* cannonicalLabel = String_newBinary(NULL, 19, alloc);
    AddrTools_printPath(cannonicalLabel->bytes, link->cannonicalLabel);
    Dict_putString(result, String_new("cannonicalLabel", alloc), cannonicalLabel, alloc);

    String* parent = Address_toString(&link->parent->address, alloc);
    Dict_putString(result, String_new("parent", alloc), parent, alloc);

    String* child = Address_toString(&link->child->address, alloc);
    Dict_putString(result, String_new("child", alloc), child, alloc);

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
        if (AddrTools_parseIp(ip, ipStr->bytes)) {
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

    Dict_putInt(result, String_new("cost", alloc), Node_getCost(node), alloc);

    List* encScheme = EncodingScheme_asList(node->encodingScheme, alloc);
    Dict_putList(result, String_new("encodingScheme", alloc), encScheme, alloc);

    Dict* bestParent = Dict_new(alloc);
    String* parentIp = String_newBinary(NULL, 39, alloc);
    AddrTools_printIp(parentIp->bytes, Node_getBestParent(node)->parent->address.ip6.bytes);
    Dict_putString(bestParent, String_CONST("ip"), parentIp, alloc);

    String* parentChildLabel = String_newBinary(NULL, 19, alloc);
    AddrTools_printPath(parentChildLabel->bytes, Node_getBestParent(node)->cannonicalLabel);
    Dict_putString(bestParent, String_CONST("parentChildLabel"), parentChildLabel, alloc);

    int isOneHop = Node_isOneHopLink(Node_getBestParent(node));
    Dict_putInt(bestParent, String_CONST("isOneHop"), isOneHop, alloc);

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
            { .name = "page", .required = true, .type = "Int" },
        }), admin);

    Admin_registerFunction("NodeStore_getLink", getLink, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "parent", .required = false, .type = "String" },
            { .name = "linkNum", .required = true, .type = "Int" },
        }), admin);
    Admin_registerFunction("NodeStore_nodeForAddr", nodeForAddr, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "ip", .required = false, .type = "String" },
        }), admin);
    Admin_registerFunction("NodeStore_getRouteLabel", getRouteLabel, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "pathToParent", .required = true, .type = "String" },
            { .name = "pathParentToChild", .required = true, .type = "String" }
        }), admin);
}
