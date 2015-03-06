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

static void setUser(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
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

static void dropPermissions(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = (struct Context*) vctx;
    struct Jmp jmp;
    Jmp_try(jmp) {
        Security_dropPermissions(requestAlloc, ctx->logger, &jmp.handler);
    } Jmp_catch {
        sendError(jmp.message, txid, ctx->admin);
        return;
    }
    sendError("none", txid, ctx->admin);
}

static void checkPermissionsB(struct Except* eh,
                              String* txid,
                              struct Admin* admin,
                              struct Allocator* requestAlloc)
{
    struct Security_Permissions* sp = Security_checkPermissions(requestAlloc, eh);
    Dict* out = Dict_new(requestAlloc);
    Dict_putInt(out, String_CONST("noOpenFiles"), sp->noOpenFiles, requestAlloc);
    Dict_putInt(out, String_CONST("seccompExists"), sp->seccompExists, requestAlloc);
    Dict_putInt(out, String_CONST("seccompEnforcing"), sp->seccompEnforcing, requestAlloc);
    Dict_putInt(out, String_CONST("memoryLimitBytes"), sp->memoryLimitBytes, requestAlloc);
    Dict_putString(out, String_CONST("error"), String_CONST("none"), requestAlloc);
    Admin_sendMessage(out, txid, admin);
}

static void checkPermissions(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = (struct Context*) vctx;
    struct Jmp jmp;
    Jmp_try(jmp) {
        checkPermissionsB(&jmp.handler, txid, ctx->admin, requestAlloc);
    } Jmp_catch {
        sendError(jmp.message, txid, ctx->admin);
        return;
    }
}

static void nofiles(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}

static void noforks(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}

static void chroot(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}

static void aslimit(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}

static void seccomp(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}

static void letRun(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    Assert_failure("unimplemented");
}


void Security_admin_register(struct Allocator* alloc, struct Log* logger, struct Admin* admin)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .logger = logger,
        .admin = admin
    }));

    Admin_registerFunction("Security_nofiles", nofiles, ctx, true, NULL, admin);
    Admin_registerFunction("Security_noforks", noforks, ctx, true, NULL, admin);
    Admin_registerFunction("Security_chroot", chroot, ctx, true, ((struct Admin_FunctionArg[]) {
        { .name = "root", .required = 1, .type = "String" }
    }), admin);
    Admin_registerFunction("Security_setUser", setUser, ctx, true, ((struct Admin_FunctionArg[]) {
        { .name = "user", .required = 1, .type = "String" }
    }), admin);
    Admin_registerFunction("Security_aslimit", aslimit, ctx, true, NULL, admin);
    Admin_registerFunction("Security_seccomp", seccomp, ctx, true, NULL, admin);
    Admin_registerFunction("Security_letRun", letRun, ctx, true, NULL, admin);
////
//    Admin_registerFunction("Security_setUser", setUser, ctx, true, setUserArgs, admin);
//    Admin_registerFunction("Security_dropPermissions", dropPermissions, ctx, true, NULL, admin);
//    Admin_registerFunction("Security_checkPermissions", checkPermissions, ctx, true, NULL, admin);
}
