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

#include "admin/angel/Core_admin.h"
#include "admin/angel/Core.h"
#include "memory/BufferAllocator.h"
#include "exception/Jmp.h"


struct Context
{
    uint8_t ipAddr[16];
    struct Ducttape* ducttape;
    struct Log* logger;
    struct Allocator* alloc;
    struct Admin* admin;
    struct EventBase* eventBase;
};

static void sendResponse(String* error,
                         struct Admin* admin,
                         String* txid,
                         struct Allocator* tempAlloc)
{
    Dict* output = Dict_new(tempAlloc);
    Dict_putString(output, String_CONST("error"), error, tempAlloc);
    Admin_sendMessage(output, txid, admin);
}

static void initTunnel(Dict* args, void* vcontext, String* txid)
{
    struct Context* const ctx = (struct Context*) vcontext;
    #define BUFFERSZ 1024
    uint8_t buffer[BUFFERSZ];
    struct Allocator* const alloc = BufferAllocator_new(buffer, BUFFERSZ);

    struct Jmp jmp;
    Jmp_try(jmp) {
        Core_initTunnel(Dict_getString(args, String_CONST("desiredTunName")),
                        ctx->ipAddr,
                        8,
                        ctx->ducttape,
                        ctx->logger,
                        ctx->eventBase,
                        ctx->alloc,
                        &jmp.handler);
    } Jmp_catch {
        String* error = String_printf(alloc, "Failed to configure tunnel [%s]", jmp.message);
        sendResponse(error, ctx->admin, txid, alloc);
        return;
    }

    sendResponse(String_CONST("none"), ctx->admin, txid, alloc);
}

void Core_admin_register(uint8_t ipAddr[16],
                         struct Ducttape* dt,
                         struct Log* logger,
                         struct Allocator* alloc,
                         struct Admin* admin,
                         struct EventBase* eventBase)
{
    struct Context* ctx = alloc->malloc(sizeof(struct Context), alloc);
    Bits_memcpyConst(ctx->ipAddr, ipAddr, 16);
    ctx->ducttape = dt;
    ctx->logger = logger;
    ctx->alloc = alloc;
    ctx->admin = admin;
    ctx->eventBase = eventBase;

    struct Admin_FunctionArg args[] = {
        { .name = "desiredTunName", .required = 0, .type = "String" }
    };
    Admin_registerFunction("Core_initTunnel", initTunnel, ctx, true, args, admin);
}
