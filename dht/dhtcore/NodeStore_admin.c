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
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeStore_admin.h"
#include "memory/Allocator.h"
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
    Object* version = Int_OBJ(Version_CURRENT_VERSION);
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

static void dumpTable(Dict* args, void* vcontext, String* txid)
{
    struct Context* ctx = Identity_cast((struct Context*) vcontext);
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    dumpTable_addEntries(ctx, i, 0, NULL, txid);
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
}
