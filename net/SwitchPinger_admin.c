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
#include "benc/Int.h"
#include "dht/Address.h"
#include "net/SwitchPinger.h"
#include "util/Endian.h"
#include "util/AddrTools.h"

#define DEFAULT_TIMEOUT 2000

struct Context
{
    struct SwitchPinger* switchPinger;
    struct Admin* admin;
    struct Allocator* alloc;
};

struct Ping
{
    struct Context* context;
    String* txid;
    String* path;
};

static void adminPingOnResponse(enum SwitchPinger_Result result,
                                uint64_t label,
                                String* data,
                                uint32_t millisecondsLag,
                                uint32_t version,
                                void* vping)
{
    struct Ping* ping = vping;
    String* resultStr = SwitchPinger_resultString(result);
    uint8_t path[20];
    AddrTools_printPath(path, label);
    String* pathStr = &(String) { .bytes = (char*) path, .len = 19 };

    Dict response = Dict_CONST(
        String_CONST("version"), Int_OBJ(version), NULL
    );

    Dict d = Dict_CONST(String_CONST("rpath"), String_OBJ(pathStr), response);
    if (result == SwitchPinger_Result_LABEL_MISMATCH) {
        response = d;
    }

    response = Dict_CONST(String_CONST("result"), String_OBJ(resultStr), response);

    response = Dict_CONST(String_CONST("path"), String_OBJ(ping->path), response);

    response = Dict_CONST(String_CONST("ms"), Int_OBJ(millisecondsLag), response);

    d = Dict_CONST(String_CONST("data"), String_OBJ(data), response);
    if (data) {
        response = d;
    }

    Admin_sendMessage(&response, ping->txid, ping->context->admin);
}

static void adminPing(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    String* pathStr = Dict_getString(args, String_CONST("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    String* data = Dict_getString(args, String_CONST("data"));
    uint32_t timeout = (timeoutPtr) ? *timeoutPtr : DEFAULT_TIMEOUT;
    uint64_t path;
    String* err = NULL;
    if (pathStr->len != 19 || AddrTools_parsePath(&path, (uint8_t*) pathStr->bytes)) {
        err = String_CONST("path was not parsable.");
    } else {
        struct SwitchPinger_Ping* ping = SwitchPinger_newPing(path,
                                                              data,
                                                              timeout,
                                                              adminPingOnResponse,
                                                              context->alloc,
                                                              context->switchPinger);
        if (!ping) {
            err = String_CONST("no open slots to store ping, try later.");
        } else {
            ping->onResponseContext = Allocator_clone(ping->pingAlloc, (&(struct Ping) {
                .context = context,
                .txid = String_clone(txid, ping->pingAlloc),
                .path = String_clone(pathStr, ping->pingAlloc)
            }));
        }
    }

    if (err) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(err), NULL);
        Admin_sendMessage(&d, txid, context->admin);
    }
}

void SwitchPinger_admin_register(struct SwitchPinger* sp,
                                 struct Admin* admin,
                                 struct Allocator* alloc)
{
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .switchPinger = sp,
        .alloc = alloc,
        .admin = admin
    }));

    Admin_registerFunction("SwitchPinger_ping", adminPing, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "timeout", .required = 0, .type = "Int" },
            { .name = "data", .required = 0, .type = "String" }
        }), admin);
}
