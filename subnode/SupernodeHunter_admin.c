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
#include "benc/List.h"
#include "crypto/Key.h"
#include "subnode/SupernodeHunter.h"
#include "subnode/SupernodeHunter_admin.h"
#include "util/Identity.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct SupernodeHunter* snh;
    Identity
};

static struct Address* getAddr(Dict* args, struct Allocator* alloc)
{
    struct Address addr;
    Bits_memset(&addr, 0, Address_SIZE);
    if (Key_parse(Dict_getStringC(args, "key"), addr.key, addr.ip6.bytes)) { return NULL; }
    return Address_clone(&addr, alloc);
}

static void sendError(struct Context* ctx, String* txid, struct Allocator* requestAlloc, char* err)
{
    Dict* out = Dict_new(requestAlloc);
    Dict_putStringCC(out, "error", err, requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void addSnode(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    struct Address* addr = getAddr(args, requestAlloc);
    if (!addr) {
        sendError(ctx, txid, requestAlloc, "parse_error");
        return;
    }
    int ret = SupernodeHunter_addSnode(ctx->snh, addr);
    char* err;
    switch (ret) {
        case SupernodeHunter_addSnode_EXISTS: {
            err = "SupernodeHunter_addSnode_EXISTS";
            break;
        }
        case 0: {
            err = "none";
            break;
        }
        default: {
            err = "UNKNOWN";
        }
    }
    sendError(ctx, txid, requestAlloc, err);
}

static void removeSnode(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    struct Address* addr = getAddr(args, requestAlloc);
    if (!addr) {
        sendError(ctx, txid, requestAlloc, "parse_error");
        return;
    }

    int ret = SupernodeHunter_removeSnode(ctx->snh, addr);
    char* err;
    switch (ret) {
        case SupernodeHunter_removeSnode_NONEXISTANT: {
            err = "SupernodeHunter_removeSnode_NONEXISTANT";
            break;
        }
        case 0: {
            err = "none";
            break;
        }
        default: {
            err = "UNKNOWN";
        }
    }
    sendError(ctx, txid, requestAlloc, err);
}

#define NODES_PER_PAGE 8
static void listSnodes(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    int page = 0;
    int64_t* pageP = Dict_getIntC(args, "page");
    if (pageP && *pageP > 0) { page = *pageP; }

    struct Address** snodes;
    int count = SupernodeHunter_listSnodes(ctx->snh, &snodes, requestAlloc);
    List* snodeList = List_new(requestAlloc);
    for (int i = page * NODES_PER_PAGE, j = 0; i < count && j < NODES_PER_PAGE; i++, j++) {
        List_addString(snodeList, Key_stringify(snodes[i]->key, requestAlloc), requestAlloc);
    }
    Dict* out = Dict_new(requestAlloc);
    Dict_putListC(out, "snodes", snodeList, requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void SupernodeHunter_admin_register(struct SupernodeHunter* snh,
                                    struct Admin* admin,
                                    struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .snh = snh
    }));
    Identity_set(ctx);

    Admin_registerFunction("SupernodeHunter_addSnode", addSnode, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "key", .required = true, .type = "String" }
        }), admin);
    Admin_registerFunction("SupernodeHunter_listSnodes", listSnodes, ctx, false,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = false, .type = "Int" }
        }), admin);
    Admin_registerFunction("SupernodeHunter_removeSnode", removeSnode, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "key", .required = true, .type = "String" }
        }), admin);
}
