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
#define string_strlen
#define string_strcpy

#include "admin/Admin.h"
#include "benc/Int.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Object.h"
#include "dht/dhtcore/NodeHeader.h"
#include "dht/dhtcore/NodeStore_admin.h"
#include "dht/dhtcore/NodeStore_pvt.h"
#include "util/platform/libc/string.h"
#include "util/version/Version.h"

struct Context
{
  struct Admin* admin;
  struct NodeStore* store;
};

static void sendEntries(struct Context* context,
                        struct List_Item* last,
                        bool isMore,
                        String* txid)
{
    Dict table = Dict_CONST(String_CONST("routingTable"), List_OBJ(&last), NULL);
    if (isMore) {
        table = Dict_CONST(String_CONST("more"), Int_OBJ(1), table);
    } else {
        // the self route is synthetic so add 1 to the count.
        table = Dict_CONST(String_CONST("count"), Int_OBJ(context->store->size + 1), table);
    }
    Admin_sendMessage(&table, txid, context->admin);
}

#define ENTRIES_PER_PAGE 8
static void addRoutingTableEntries(struct Context* context,
                                   int i,
                                   int j,
                                   struct List_Item* last,
                                   String* txid)
{
    struct NodeStore* store = context->store;
    uint8_t path[20];
    uint8_t ip[40];
    String* pathStr = &(String) { .len = 19, .bytes = (char*)path };
    String* ipStr = &(String) { .len = 39, .bytes = (char*)ip };
    Object* link = Int_OBJ(0xFFFFFFFF);
    Object* version = Int_OBJ(Version_CURRENT_PROTOCOL);
    Dict entry = Dict_CONST(
        String_CONST("ip"), String_OBJ(ipStr), Dict_CONST(
        String_CONST("link"), link, Dict_CONST(
        String_CONST("path"), String_OBJ(pathStr), Dict_CONST(
        String_CONST("version"), version, NULL
    ))));

    struct List_Item next = { .next = last, .elem = Dict_OBJ(&entry) };

    if (i >= store->size || j >= ENTRIES_PER_PAGE) {
        if (i > j) {
            sendEntries(context, last, (j >= ENTRIES_PER_PAGE), txid);
            return;
        }

        Address_printIp(ip, store->thisNodeAddress);
        strcpy((char*)path, "0000.0000.0000.0001");
        sendEntries(context, &next, (j >= ENTRIES_PER_PAGE), txid);
        return;
    }

    link->as.number = store->headers[i].reach;
    version->as.number = store->headers[i].version;
    Address_printIp(ip, &store->nodes[i].address);
    AddrTools_printPath(path, store->nodes[i].address.path);

    addRoutingTableEntries(context, i + 1, j + 1, &next, txid);
}

static void dumpTable(Dict* args, void* vcontext, String* txid)
{
    struct Context* context = vcontext;
    int64_t* page = Dict_getInt(args, String_CONST("page"));
    int i = (page) ? *page * ENTRIES_PER_PAGE : 0;
    addRoutingTableEntries(context, i, 0, NULL, txid);
}

void NodeStore_admin_register(struct NodeStore* store,
                              struct Admin* admin,
                              struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .store = store,
        .admin = admin
    }));
    struct Admin_FunctionArg adma[1] = {
        { .name = "page", .required = 1, .type = "Int" },
    };
    Admin_registerFunction("NodeStore_dumpTable", dumpTable, ctx, false, adma, admin);
}
