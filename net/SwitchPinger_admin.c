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
#include "crypto/Key.h"

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

static void adminPingOnResponse(struct SwitchPinger_Response* resp, void* vping)
{
    struct Allocator* pingAlloc = resp->ping->pingAlloc;
    struct Ping* ping = vping;

    Dict* rd = Dict_new(pingAlloc);

    if (resp->res == SwitchPinger_Result_LABEL_MISMATCH) {
        uint8_t path[20] = {0};
        AddrTools_printPath(path, resp->label);
        String* pathStr = String_new(path, pingAlloc);
        Dict_putString(rd, String_CONST("rpath"), pathStr, pingAlloc);
    }

    Dict_putInt(rd, String_CONST("version"), resp->version, pingAlloc);
    Dict_putInt(rd, String_CONST("ms"), resp->milliseconds, pingAlloc);
    Dict_putString(rd, String_CONST("result"), SwitchPinger_resultString(resp->res), pingAlloc);
    Dict_putString(rd, String_CONST("path"), ping->path, pingAlloc);
    if (resp->data) {
        Dict_putString(rd, String_CONST("data"), resp->data, pingAlloc);
    }

    if (!Bits_isZero(resp->key, 32)) {
        Dict_putString(rd, String_CONST("key"), Key_stringify(resp->key, pingAlloc), pingAlloc);
    }

    Admin_sendMessage(rd, ping->txid, ping->context->admin);
}

static void adminPing(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    String* pathStr = Dict_getString(args, String_CONST("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    String* data = Dict_getString(args, String_CONST("data"));
    int64_t* keyPing = Dict_getInt(args, String_CONST("keyPing"));
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
        if (keyPing && *keyPing) { ping->keyPing = true; }
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
                                 struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct Context* ctx = Allocator_clone(alloc, (&(struct Context) {
        .switchPinger = sp,
        .alloc = alloc,
        .admin = admin
    }));

    Admin_registerFunction("SwitchPinger_ping", adminPing, ctx, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "path", .required = 1, .type = "String" },
            { .name = "timeout", .required = 0, .type = "Int" },
            { .name = "data", .required = 0, .type = "String" },
            { .name = "keyPing", .required = 0, .type = "Int" }
        }), admin);
}
