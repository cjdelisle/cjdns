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
#include "benc/Int.h"
#include "benc/Dict.h"
#include "benc/List.h"
#include "crypto/Key.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "tunnel/IpTunnel_admin.h"
#include "util/platform/Sockaddr.h"

#include <stddef.h>

struct Context
{
    struct IpTunnel* ipTun;
    struct Admin* admin;
};

static void sendResponse(int conn, String* txid, struct Admin* admin)
{
    Dict resp = Dict_CONST(
        String_CONST("connection"), Int_OBJ(conn), Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), NULL
    ));
    Admin_sendMessage(&resp, txid, admin);
}

static void sendError(char* error, String* txid, struct Admin* admin)
{
    Dict resp = Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST(error)), NULL
    );
    Admin_sendMessage(&resp, txid, admin);
}

static void allowConnection(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* requestAlloc)
{
    struct Context* context = (struct Context*) vcontext;
    String* publicKeyOfAuthorizedNode =
        Dict_getString(args, String_CONST("publicKeyOfAuthorizedNode"));
    String* ip6Address = Dict_getString(args, String_CONST("ip6Address"));
    String* ip4Address = Dict_getString(args, String_CONST("ip4Address"));
    uint8_t pubKey[32];
    uint8_t ip6Addr[16];

    struct Sockaddr_storage ip6ToGive;
    struct Sockaddr_storage ip4ToGive;

    char* error;
    int ret;
    if (!ip6Address && !ip4Address) {
        error = "Must specify ip6Address or ip4Address";
    } else if ((ret = Key_parse(publicKeyOfAuthorizedNode, pubKey, ip6Addr)) != 0) {
        error = Key_parse_strerror(ret);
    } else if (ip6Address
        && (Sockaddr_parse(ip6Address->bytes, &ip6ToGive)
            || Sockaddr_getFamily(&ip6ToGive.addr) != Sockaddr_AF_INET6))
    {
        error = "malformed ip6Address";
    } else if (ip4Address
        && (Sockaddr_parse(ip4Address->bytes, &ip4ToGive)
            || Sockaddr_getFamily(&ip4ToGive.addr) != Sockaddr_AF_INET))
    {
        error = "malformed ip4Address";
    } else {
        int conn = IpTunnel_allowConnection(pubKey,
                                            (ip6Address) ? &ip6ToGive.addr : NULL,
                                            (ip4Address) ? &ip4ToGive.addr : NULL,
                                            context->ipTun);
        sendResponse(conn, txid, context->admin);
        return;
    }

    sendError(error, txid, context->admin);
}


static void connectTo(Dict* args, void* vcontext, String* txid, struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
    String* publicKeyOfNodeToConnectTo =
        Dict_getString(args, String_CONST("publicKeyOfNodeToConnectTo"));

    uint8_t pubKey[32];
    uint8_t ip6[16];
    int ret;
    if ((ret = Key_parse(publicKeyOfNodeToConnectTo, pubKey, ip6)) != 0) {
        sendError(Key_parse_strerror(ret), txid, context->admin);
        return;
    }
    int conn = IpTunnel_connectTo(pubKey, context->ipTun);
    sendResponse(conn, txid, context->admin);
}

static void removeConnection(Dict* args,
                             void* vcontext,
                             String* txid,
                             struct Allocator* requestAlloc)
{
    struct Context* context = vcontext;
/*
    int conn = (int) *(Dict_getInt(args, String_CONST("connection")));
    char* error = "none";
    if (IpTunnel_removeConnection_NOT_FOUND == IpTunnel_removeConnection(conn, context->ipTun)) {
        error = "not found";
    }
*/
    sendError("not implemented", txid, context->admin);
}

static void listConnections(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* alloc)
{
    struct Context* context = vcontext;
    List* l = NULL;
    for (int i = 0; i < (int)context->ipTun->connectionList.count; i++) {
        l = List_addInt(l, context->ipTun->connectionList.connections[i].number, alloc);
    }
    Dict resp = Dict_CONST(
        String_CONST("connections"), List_OBJ(l), Dict_CONST(
        String_CONST("error"), String_OBJ(String_CONST("none")), NULL
    ));
    Admin_sendMessage(&resp, txid, context->admin);
}

static void showConn(struct IpTunnel_Connection* conn,
                     String* txid,
                     struct Admin* admin,
                     struct Allocator* alloc)
{
    Dict* d = Dict_new(alloc);

    if (!Bits_isZero(conn->connectionIp6, 16)) {
        struct Sockaddr* addr = Sockaddr_clone(Sockaddr_LOOPBACK6, alloc);
        uint8_t* address;
        Assert_true(16 == Sockaddr_getAddress(addr, &address));
        Bits_memcpyConst(address, conn->connectionIp6, 16);
        char* printedAddr = Sockaddr_print(addr, alloc);
        Dict_putString(d, String_CONST("ip6Address"), String_CONST(printedAddr), alloc);
    }

    if (!Bits_isZero(conn->connectionIp4, 4)) {
        struct Sockaddr* addr = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
        uint8_t* address;
        Assert_true(4 == Sockaddr_getAddress(addr, &address));
        Bits_memcpyConst(address, conn->connectionIp4, 4);
        char* printedAddr = Sockaddr_print(addr, alloc);
        Dict_putString(d, String_CONST("ip4Address"), String_CONST(printedAddr), alloc);
    }

    Dict_putString(d, String_CONST("key"), Key_stringify(conn->header.nodeKey, alloc), alloc);
    Dict_putInt(d, String_CONST("outgoing"), (conn->isOutgoing) ? 1 : 0, alloc);
    Dict_putString(d, String_CONST("error"), String_CONST("none"), alloc);

    Admin_sendMessage(d, txid, admin);
}

static void showConnection(Dict* args, void* vcontext, String* txid, struct Allocator* alloc)
{
    struct Context* context = vcontext;
    int connNum = (int) *(Dict_getInt(args, String_CONST("connection")));

    for (int i = 0; i < (int)context->ipTun->connectionList.count; i++) {
        if (connNum == context->ipTun->connectionList.connections[i].number) {
            showConn(&context->ipTun->connectionList.connections[i], txid, context->admin, alloc);
            return;
        }
    }
    sendError("connection not found", txid, context->admin);
}

void IpTunnel_admin_register(struct IpTunnel* ipTun, struct Admin* admin, struct Allocator* alloc)
{
    struct Context* context = Allocator_clone(alloc, (&(struct Context) {
        .admin = admin,
        .ipTun = ipTun
    }));

    Admin_registerFunction("IpTunnel_allowConnection", allowConnection, context, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "publicKeyOfAuthorizedNode", .required = 1, .type = "String" },
            { .name = "ip6Address", .required = 0, .type = "String" },
            { .name = "ip4Address", .required = 0, .type = "String" },
        }), admin);

    Admin_registerFunction("IpTunnel_connectTo", connectTo, context, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "publicKeyOfNodeToConnectTo", .required = 1, .type = "String" }
        }), admin);

    Admin_registerFunction("IpTunnel_removeConnection", removeConnection, context, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "connection", .required = 1, .type = "Int" }
        }), admin);

    Admin_registerFunction("IpTunnel_showConnection", showConnection, context, true,
        ((struct Admin_FunctionArg[]) {
            { .name = "connection", .required = 1, .type = "Int" }
        }), admin);

    Admin_registerFunction("IpTunnel_listConnections", listConnections, context, true, NULL, admin);
}
