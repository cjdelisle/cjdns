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
    struct Except* eh;
    struct EventBase* base;
    struct Log* logger;
    Identity
};

struct FileNoRequest
{
    String* txid;
    struct FileNo_Promise* fp;
    struct Context* ctx;
    struct FileNo* fileno;
    enum FileNo_Type type;
    Identity
};

static void onResponse(struct FileNo_Promise* promise, int tunfd)
{
    struct FileNoRequest* fr= Identity_check((struct FileNoRequest*)promise->userData);
    Dict* resp = Dict_new(promise->alloc);

    Dict_putInt(resp, String_CONST("tunfd"), tunfd, promise->alloc);
    Dict_putInt(resp, String_CONST("type"), fr->type, promise->alloc);
    Dict_putString(resp, String_CONST("error"), String_CONST("none"), promise->alloc);
    Admin_sendMessage(resp, fr->txid, fr->ctx->admin);
}

static void import(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* ctx = Identity_check((struct Context*) vcontext);
    String* path = Dict_getString(args, String_CONST("path"));
    String* type = Dict_getString(args, String_CONST("type"));
    char* err = NULL;

    if (Defined(win32)) {
        err = "Do not support win32";
        Dict errDict = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
        Admin_sendMessage(&errDict, txid, ctx->admin);
        return;
    }

    int fdtype = FileNo_Type_NORMAL;
    if (type && !CString_strcmp(type->bytes, "android")) {
        fdtype = FileNo_Type_ANDROID;
    }

    struct FileNo_Promise* fp = FileNo_import(path->bytes, ctx->base, ctx->eh, ctx->logger,
                                              ctx->alloc);
    struct FileNoRequest* fr = Allocator_calloc(fp->alloc, sizeof(struct FileNoRequest), 1);
    Identity_set(fr);
    fr->txid = String_clone(txid, fp->alloc);
    fr->fp = fp;
    fr->type = fdtype;
    fr->ctx = ctx;

    fp->userData = fr;
    fp->callback = onResponse;
}

void FileNo_admin_register(struct Admin* admin,
                           struct Allocator* alloc,
                           struct EventBase* base,
                           struct Log* logger,
                           struct Except* eh)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .eh = eh,
        .alloc = alloc,
        .logger = logger,
        .base = base
    }));
    Identity_set(ctx);

    Admin_registerFunction("FileNo_import", import, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "type", .required = 0, .type = "String" }
        }), admin);
}
