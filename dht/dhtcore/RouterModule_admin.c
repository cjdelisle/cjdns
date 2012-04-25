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
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/Address.h"
#include "memory/Allocator.h"

struct Context {
    struct Admin* admin;
    struct Allocator* allocator;
    struct RouterModule* routerModule;
};

static void lookup(Dict* args, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;
    String* addrStr = Dict_getString(args, String_CONST("address"));
    String* err = NULL;
    String* result = NULL;
    uint8_t addr[16];
    if (addrStr->len != 39) {
        err = String_CONST("address wrong length");
    } else if (Address_parseIp(addr, (uint8_t*) addrStr->bytes)) {
        err = String_CONST("failed to parse address");
    } else {
        struct Node* n = RouterModule_lookup(addr, ctx->routerModule);
        if (!n) {
            result = String_CONST("not found");
        } else if (memcmp(addr, n->address.ip6.bytes, 16)) {
            uint8_t closest[60];
            Address_print(closest, &n->address);
            result = &(String) { .bytes = (char*) closest, .len = 59 };
        } else {
            uint8_t path[20];
            Address_printPath(path, Endian_bigEndianToHost64(n->address.networkAddress_be));
            result = &(String) { .bytes = (char*) path, .len = 19 };
        }
    }
    Dict response = (err)
        ? Dict_CONST(String_CONST("error"), String_OBJ(err), NULL)
        : Dict_CONST(String_CONST("result"), String_OBJ(result), NULL);
    Admin_sendMessage(&response, txid, ctx->admin);
}

void RouterModule_admin_register(struct RouterModule* module,
                                 struct Admin* admin,
                                 struct Allocator* alloc)
{
    struct Context* ctx = alloc->clone(sizeof(struct Context), alloc, &(struct Context) {
        .admin = admin,
        .allocator = alloc,
        .routerModule = module
    });

    struct Admin_FunctionArg adma[] = {
        { .name = "address", .required = 1, .type = "String" }
    };
    Admin_registerFunction("RouterModule_lookup", lookup, ctx, true, adma, admin);
}
