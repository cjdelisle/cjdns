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
#define string_strcpy
#define string_strlen
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
#include "util/version/Version.h"
#include "util/platform/libc/string.h"

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
        // the self route is synthetic so add 1 to the count.
        table = Dict_CONST(String_CONST("count"), Int_OBJ(ctx->store->size + 1), table);
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

    if (i >= ctx->store->size || j >= ENTRIES_PER_PAGE) {
        if (i > j) {
            dumpTable_send(ctx, last, (j >= ENTRIES_PER_PAGE), txid);
            return;
        }

        Address_printIp(ip, ctx->store->selfAddress);
        strcpy((char*)path, "0000.0000.0000.0001");
        dumpTable_send(ctx, &next, (j >= ENTRIES_PER_PAGE), txid);
        return;
    }

    struct Node* n = NodeStore_dumpTable(ctx->store, i);
    link->as.number = n->reach;
    version->as.number = n->version;
    Address_printIp(ip, &n->address);
    AddrTools_printPath(path, n->address.path);

    dumpTable_addEntries(ctx, i + 1, j + 1, &next, txid);
}

static void dumpTable(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    dumpTable_addEntries(ctx, i, 0, NULL, txid);
}

static void getLink(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);

    struct Node_Link* link;

    String* ipStr = Dict_getString(args, String_new("parent", alloc));
    int64_t* linkNum = Dict_getInt(args, String_new("linkNum", alloc));
    uint8_t ip[16];
    if (ipStr->len != 39 || AddrTools_parseIp(ip, ipStr->bytes)) {
        Dict_remove(ret, String_CONST("result"));
        Dict_putString(ret,
                       String_new("error", alloc),
                       String_new("Could not parse ip", alloc),
                       alloc);

    } else if ((link = NodeStore_getLink(ctx->store, ip, *linkNum))) {
        Dict_putInt(result,
                    String_new("encodingFormNumber", alloc),
                    link->encodingFormNumber,
                    alloc);
        Dict_putInt(result, String_new("linkState", alloc), link->linkState, alloc);

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
static void getNode(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);

    Dict* ret = Dict_new(alloc);
    Dict* result = Dict_new(alloc);
    Dict_putDict(ret, String_new("result", alloc), result, alloc);
    Dict_putString(ret, String_new("error", alloc), String_new("none", alloc), alloc);

    struct Node_Two* node;

    String* ipStr = Dict_getString(args, String_new("ip", alloc));
    uint8_t ip[16];
    if (ipStr->len != 39 || AddrTools_parseIp(ip, ipStr->bytes)) {
        Dict_remove(ret, String_CONST("result"));
        Dict_putString(ret,
                       String_new("error", alloc),
                       String_new("Could not parse ip", alloc),
                       alloc);

    } else if ((node = NodeStore_getNode2(ctx->store, ip))) {
        Dict_putInt(result, String_new("protocolVersion", alloc), node->version, alloc);

        String* key = Key_stringify(node->address.key, alloc);
        Dict_putString(result, String_new("key", alloc), key, alloc);

        uint32_t linkCount = NodeStore_linkCount(node);
        Dict_putInt(result, String_new("linkCount", alloc), linkCount, alloc);

        List* encScheme = EncodingScheme_asList(node->encodingScheme, alloc);
        Dict_putList(result, String_new("encodingScheme", alloc), encScheme, alloc);
    }

    Admin_sendMessage(ret, txid, ctx->admin);
}

static void getRouteLabel(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);
    List* addresses = Dict_getList(args, String_CONST("addresses"));
    int count = List_size(addresses);
    uint8_t* addrBuff = Allocator_calloc(requestAlloc, List_size(addresses), 16);
    char* err = NULL;
    for (int i = 0; i < count; i++) {
        String* addr = List_getString(addresses, i);
        if (!addr) {
            err = "Element in addresses list which is not a string";
        } else if (addr->len != 39) {
            err = "Address of incorrect length, must be a 39 character full ipv6 address";
        } else if (AddrTools_parseIp(&addrBuff[i*16], addr->bytes)) {
            err = "Failed to parse address";
        } else {
            continue;
        }
        break;
    }
    uint64_t label;
    if (!err) {
        label = NodeStore_getRouteLabel(ctx->store, addrBuff, count);
        if (label == NodeStore_getRouteLabel_NODE_NOT_FOUND) {
            err = "NodeStore_getRouteLabel_NODE_NOT_FOUND";
        } else if (label == NodeStore_getRouteLabel_LINK_NOT_FOUND) {
            err = "NodeStore_getRouteLabel_LINK_NOT_FOUND";
        }
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
    Admin_registerFunction("NodeStore_getNode", getNode, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "ip", .required = 1, .type = "String" },
        }), admin);
    Admin_registerFunction("NodeStore_getRouteLabel", getRouteLabel, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "addresses", .required = 1, .type = "List" },
        }), admin);
}
