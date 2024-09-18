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
#include "benc/String.h"
#include "benc/Dict.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "util/Security_admin.h"

struct Context
{
    struct Log* logger;
    struct Admin* admin;
    struct Security* sec;
    Identity
};

static void sendError(RTypes_Error_t* er, String* txid, struct Admin* admin, Allocator_t* requestAlloc)
{
    char* msg = "none";
    if (er) {
        msg = Rffi_printError(er, requestAlloc);
    }
    Dict error = Dict_CONST(String_CONST("error"),
        String_OBJ(String_CONST(msg)), NULL);
    Admin_sendMessage(&error, txid, admin);
}

static void setUser(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    int64_t* user = Dict_getIntC(args, "uid");
    int64_t* group = Dict_getIntC(args, "gid");
    int gid = group ? (int)*group : 0;
    int64_t* keepNetAdmin = Dict_getIntC(args, "keepNetAdmin");
    RTypes_Error_t* er = Security_setUser(*user, gid, *keepNetAdmin, ctx->logger, requestAlloc);
    if (er) {
        sendError(er, txid, ctx->admin, requestAlloc);
        return;
    }
    sendError(NULL, txid, ctx->admin, requestAlloc);
}

static void checkPermissions(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    struct Security_Permissions* sp = NULL;
    RTypes_Error_t* er = Security_checkPermissions(&sp, requestAlloc);
    if (er) {
        sendError(er, txid, ctx->admin, requestAlloc);
        return;
    }
    Dict* out = Dict_new(requestAlloc);
    Dict_putIntC(out, "noOpenFiles", sp->noOpenFiles, requestAlloc);
    Dict_putIntC(out, "userId", sp->uid, requestAlloc);
    Dict_putStringCC(out, "error", "none", requestAlloc);
    Admin_sendMessage(out, txid, ctx->admin);
}

static void nofiles(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    RTypes_Error_t* er = Security_nofiles(requestAlloc);
    if (er) {
        sendError(er, txid, ctx->admin, requestAlloc);
        return;
    }
    sendError(NULL, txid, ctx->admin, requestAlloc);
}

static void noforks(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    RTypes_Error_t* er = Security_noforks(requestAlloc);
    if (er) {
        sendError(er, txid, ctx->admin, requestAlloc);
        return;
    }
    sendError(NULL, txid, ctx->admin, requestAlloc);
}

static void chroot(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    String* root = Dict_getStringC(args, "root");
    RTypes_Error_t* er = Security_chroot(root->bytes, requestAlloc);
    if (er) {
        sendError(er, txid, ctx->admin, requestAlloc);
        return;
    }
    sendError(NULL, txid, ctx->admin, requestAlloc);
}

static void setupComplete(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    Security_setupComplete(ctx->sec);
    sendError(NULL, txid, ctx->admin, requestAlloc);
}

static void getUser(Dict* args, void* vctx, String* txid, struct Allocator* requestAlloc)
{
    struct Context* const ctx = Identity_check((struct Context*) vctx);
    String* user = Dict_getStringC(args, "user");
    Dict* ret = Security_getUser((user) ? user->bytes : NULL, requestAlloc);
    Admin_sendMessage(ret, txid, ctx->admin);
}

void Security_admin_register(struct Allocator* alloc,
                             struct Log* logger,
                             struct Security* sec,
                             struct Admin* admin)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .logger = logger,
        .admin = admin
    }));
    Identity_set(ctx);
    ctx->sec = sec;

    Admin_registerFunction("Security_nofiles", nofiles, ctx, true, NULL, admin);
    Admin_registerFunction("Security_noforks", noforks, ctx, true, NULL, admin);
    Admin_registerFunction("Security_chroot", chroot, ctx, true, ((struct Admin_FunctionArg[]) {
        { .name = "root", .required = 1, .type = "String" }
    }), admin);
    Admin_registerFunction("Security_setUser", setUser, ctx, true, ((struct Admin_FunctionArg[]) {
        { .name = "uid", .required = 1, .type = "Int" },
        { .name = "gid", .required = 0, .type = "Int" },
        { .name = "keepNetAdmin", .required = 1, .type = "Int" },
    }), admin);
    Admin_registerFunction("Security_getUser", getUser, ctx, true, ((struct Admin_FunctionArg[]) {
        { .name = "user", .required = 0, .type = "String" }
    }), admin);
    Admin_registerFunction("Security_setupComplete", setupComplete, ctx, true, NULL, admin);
    Admin_registerFunction("Security_checkPermissions", checkPermissions, ctx, true, NULL, admin);
}
