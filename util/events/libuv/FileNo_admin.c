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
#include "benc/List.h"
#include "benc/String.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/events/FileNo.h"
#include "util/events/libuv/FileNo_admin.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"

struct Context
{
    struct FileNo_admin pub;
    struct Admin* admin;
    struct Allocator* alloc;
    struct FileNo* fileno;
    struct Except* eh;
    struct EventBase* base;
    struct Log* logger;
    FileNo_callback onFileNoReceived;
    Identity
};

static void import(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* path = Dict_getString(args, String_CONST("path"));
    String* type = Dict_getString(args, String_CONST("type"));
    char* error = NULL;

    if (ctx->fileno && CString_strncmp(ctx->fileno->pipePath, path->bytes, path->len)) {
        error = "none";
    } else if (ctx->fileno) {
        error = "FileNo already exist";
    } else {
        int fdtype = FileNo_Type_NORMAL;
        if (type && !CString_strcmp(type->bytes, "android")) {
            fdtype = FileNo_Type_ANDROID;
        }
        ctx->fileno = FileNo_new(path->bytes, ctx->base, ctx->eh, ctx->logger, ctx->alloc,
                                 ctx->onFileNoReceived);
        ctx->fileno->type = fdtype;
        ctx->fileno->userData = ctx->pub.userData;
        error = "none";
    }

    Dict response = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST(error)), NULL
    );
    Admin_sendMessage(&response, txid, ctx->admin);
}

struct FileNo_admin* FileNo_admin_new(struct Admin* admin,
                                    struct Allocator* alloc,
                                    struct EventBase* base,
                                    struct Log* logger,
                                    struct Except* eh,
                                    FileNo_callback cb)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .eh = eh,
        .alloc = alloc,
        .logger = logger,
        .onFileNoReceived = cb,
        .base = base
    }));
    Identity_set(ctx);

    Admin_registerFunction("FileNo_import", import, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "type", .required = 0, .type = "String" }
        }), admin);

    return &ctx->pub;
}
