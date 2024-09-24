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
#include "benc/Int.h"
#include "dht/Address.h"
#include "net/SwitchPinger.h"
#include "net/SwitchPinger_admin.h"
#include "util/Endian.h"
#include "util/AddrTools.h"
#include "crypto/Key.h"
#include "util/Hex.h"
#include "util/platform/Sockaddr.h"

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
        Dict_putStringC(rd, "rpath", pathStr, pingAlloc);
    }

    Dict_putIntC(rd, "version", resp->version, pingAlloc);
    Dict_putIntC(rd, "ms", resp->milliseconds, pingAlloc);
    Dict_putStringC(rd, "result", SwitchPinger_resultString(resp->res), pingAlloc);
    Dict_putStringC(rd, "path", ping->path, pingAlloc);
    if (resp->data) {
        Dict_putStringC(rd, "data", resp->data, pingAlloc);
    }

    if (!Bits_isZero(resp->key, 32)) {
        Dict_putStringC(rd, "key", Key_stringify(resp->key, pingAlloc), pingAlloc);
    }

    if (!Bits_isZero(&resp->lladdr, sizeof resp->lladdr)) {
        struct Sockaddr_storage ss;
        if (resp->lladdr.addr.udp4.type == Control_LlAddr_Udp4_TYPE) {
            Sockaddr_t* sa = Sockaddr_initFromBytes(
                &ss, resp->lladdr.addr.udp4.addr, Sockaddr_AF_INET);
            Sockaddr_setPort(sa, Endian_bigEndianToHost16(resp->lladdr.addr.udp4.port_be));
            Dict_putStringCC(
                rd, "lladdr", Sockaddr_print(sa, pingAlloc), pingAlloc);
        } else if (resp->lladdr.addr.udp4.type == Control_LlAddr_Udp6_TYPE) {
            Sockaddr_t* sa = Sockaddr_initFromBytes(
                &ss, resp->lladdr.addr.udp6.addr, Sockaddr_AF_INET6);
            Sockaddr_setPort(sa, Endian_bigEndianToHost16(resp->lladdr.addr.udp6.port_be));
            Dict_putStringCC(
                rd, "lladdr", Sockaddr_print(sa, pingAlloc), pingAlloc);
        } else {
            Dict_putStringCC(
                rd,
                "lladdrUnknown",
                Hex_print(&resp->lladdr, sizeof resp->lladdr, pingAlloc),
                pingAlloc
            );
        }
    }

    Admin_sendMessage(rd, ping->txid, ping->context->admin);
}

static void adminPing(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    String* pathStr = Dict_getStringC(args, "path");
    int64_t* timeoutPtr = Dict_getIntC(args, "timeout");
    String* data = Dict_getStringC(args, "data");
    int64_t* keyPing = Dict_getIntC(args, "keyPing");
    int64_t* lladdr = Dict_getIntC(args, "lladdr");
    uint32_t timeout = (timeoutPtr) ? *timeoutPtr : DEFAULT_TIMEOUT;
    uint64_t path;
    char* err = NULL;
    if (keyPing && *keyPing && lladdr && *lladdr) {
        err = "Cannot be both keyping and lladdr";
    } else if (pathStr->len != 19 || AddrTools_parsePath(&path, (uint8_t*) pathStr->bytes)) {
        err = "path was not parsable.";
    } else {
        struct SwitchPinger_Ping* ping = SwitchPinger_newPing(path,
                                                              data,
                                                              timeout,
                                                              adminPingOnResponse,
                                                              context->alloc,
                                                              context->switchPinger);
        if (keyPing && *keyPing) {
            ping->type = SwitchPinger_Type_KEYPING;
        } else if (lladdr && *lladdr) {
            ping->type = SwitchPinger_Type_LLADDR;
        }
        if (!ping) {
            err = "no open slots to store ping, try later.";
        } else {
            ping->onResponseContext = Allocator_clone(ping->pingAlloc, (&(struct Ping) {
                .context = context,
                .txid = String_clone(txid, ping->pingAlloc),
                .path = String_clone(pathStr, ping->pingAlloc)
            }));
        }
    }

    if (err) {
        Dict d = Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(err)), NULL);
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
            { .name = "keyPing", .required = 0, .type = "Int" },
            { .name = "lladdr", .required = 0, .type = "Int" },
        }), admin);
}
