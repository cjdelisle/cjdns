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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "admin/Admin.h"
#include "benc/List.h"
#include "crypto/Key.h"
#include "subnode/ReachabilityCollector.h"
#include "util/AddrTools.h"
#include "util/Identity.h"

struct Context {
    struct Admin* admin;
    struct Allocator* alloc;
    struct ReachabilityCollector* rc;
    Identity
};

#define NODES_PER_PAGE 8
static void getPeerInfo(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    int page = 0;
    int64_t* pageP = Dict_getIntC(args, "page");
    if (pageP && *pageP > 0) { page = *pageP; }

    List* peerList = List_new(requestAlloc);
    for (int i = page * NODES_PER_PAGE, j = 0; j < NODES_PER_PAGE; i++, j++) {
        struct ReachabilityCollector_PeerInfo* pi = ReachabilityCollector_getPeerInfo(ctx->rc, i);
        if (!pi) { break; }
        Dict* pid = Dict_new(requestAlloc);
        Dict_putStringC(pid, "addr", Address_toString(&pi->addr, requestAlloc), requestAlloc);
        uint8_t rpath[20];
        AddrTools_printPath(rpath, pi->pathThemToUs);
        Dict_putStringC(
            pid, "pathThemToUs", String_newBinary(rpath, 19, requestAlloc), requestAlloc);
        Dict_putIntC(pid, "querying", pi->querying, requestAlloc);
        List_addDict(peerList, pid, requestAlloc);
    }
    Dict* out = Dict_new(requestAlloc);
    Dict_putListC(out, "peers", peerList, requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

void ReachabilityCollector_admin_register(struct ReachabilityCollector* rc,
                                          struct Admin* admin,
                                          struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .alloc = alloc,
        .rc = rc
    }));
    Identity_set(ctx);

    Admin_registerFunction("ReachabilityCollector_getPeerInfo", getPeerInfo, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "page", .required = true, .type = "Int" }
        }), admin);
}
