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
#include "benc/String.h"
#include "benc/Dict.h"
#include "exception/Except.h"
#include "exception/Jmp.h"
#include "util/log/Log.h"
#include "util/Security.h"

#include <string.h>

struct Context
{
    struct Log* logger;
    struct Admin* admin;
};

static void sendError(char* errorMessage, String* txid, struct Admin* admin)
{
    Dict error = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(errorMessage)), NULL);
    Admin_sendMessage(&error, txid, admin);
}

static void setUser(Dict* args, void* vcontext, String* txid)
{
    struct Context* const ctx = (struct Context*) vcontext;
    struct Jmp jmp;
    Jmp_try(jmp) {
        String* user = Dict_getString(args, String_CONST("user"));
        Security_setUser(user->bytes, ctx->logger, &jmp.handler);
    } Jmp_catch {
        sendError(jmp.message, txid, ctx->admin);
        return;
    }
    sendError("none", txid, ctx->admin);
}

static void noFiles(Dict* args, void* vcontext, String* txid)
{
    struct Context* const ctx = (struct Context*) vcontext;
    struct Jmp jmp;
    Jmp_try(jmp) {
        Security_noFiles(&jmp.handler);
    } Jmp_catch {
        sendError(jmp.message, txid, ctx->admin);
        return;
    }
    sendError("none", txid, ctx->admin);
}

void Security_admin_register(struct Allocator* alloc, struct Log* logger, struct Admin* admin)
{
    struct Context* ctx = alloc->clone(sizeof(struct Context), alloc, &(struct Context) {
        .logger = logger,
        .admin = admin
    });
    struct Admin_FunctionArg setUserArgs[] = {
        { .name = "user", .required = 1, .type = "String" }
    };
    Admin_registerFunction("Security_setUser", setUser, ctx, true, setUserArgs, admin);
    Admin_registerFunction("Security_noFiles", noFiles, ctx, true, NULL, admin);
}
