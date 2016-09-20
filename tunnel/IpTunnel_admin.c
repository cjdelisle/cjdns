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
    int64_t* ip6Prefix = Dict_getInt(args, String_CONST("ip6Prefix"));
    int64_t* ip6Alloc = Dict_getInt(args, String_CONST("ip6Alloc"));
    String* ip4Address = Dict_getString(args, String_CONST("ip4Address"));
    int64_t* ip4Prefix = Dict_getInt(args, String_CONST("ip4Prefix"));
    int64_t* ip4Alloc = Dict_getInt(args, String_CONST("ip4Alloc"));

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

    } else if (ip6Prefix && !ip6Address) {
        error = "Must specify ip6Address with ip6Prefix";
    } else if (ip6Alloc && !ip6Address) {
        error = "Must specify ip6Address with ip6Alloc";
    } else if (ip6Prefix && (*ip6Prefix > 128 || *ip6Prefix < 0)) {
        error = "ip6Prefix out of range: must be 0 to 128";
    } else if (ip6Alloc && (*ip6Alloc > 128 || *ip6Alloc < 1)) {
        error = "ip6Alloc out of range: must be 1 to 128";

    } else if (ip4Prefix && !ip4Address) {
        error = "Must specify ip4Address with ip4Prefix";
    } else if (ip4Alloc && !ip4Address) {
        error = "Must specify ip4Address with ip4Alloc";
    } else if (ip4Prefix && (*ip4Prefix > 32 || *ip4Prefix < 0)) {
        error = "ip4Prefix out of range: must be 0 to 32";
    } else if (ip4Alloc && (*ip4Alloc > 32 || *ip4Alloc < 1)) {
        error = "ip4Alloc out of range: must be 1 to 32";

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
                                            (ip6Prefix) ? (uint8_t) (*ip6Prefix) : 128,
                                            (ip6Alloc) ? (uint8_t) (*ip6Alloc) : 128,
                                            (ip4Address) ? &ip4ToGive.addr : NULL,
                                            (ip4Prefix) ? (uint8_t) (*ip4Prefix) : 32,
                                            (ip4Alloc) ? (uint8_t) (*ip4Alloc) : 32,
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

    int conn = (int) *(Dict_getInt(args, String_CONST("connection")));
    if (IpTunnel_removeConnection_NOT_FOUND == IpTunnel_removeConnection(conn, context->ipTun)) {
        sendError("not_found", txid, context->admin);
        return;
    }

    sendResponse(conn, txid, context->admin);
}

static void listConnections(Dict* args,
                            void* vcontext,
                            String* txid,
                            struct Allocator* alloc)
{
    struct Context* context = vcontext;
    List* l = List_new(alloc);
    for (int i = 0; i < (int)context->ipTun->connectionList.count; i++) {
        List_addInt(l, context->ipTun->connectionList.connections[i].number, alloc);
    }
    Dict* resp = Dict_new(alloc);
    Dict_putList(resp, String_CONST("connections"), l, alloc);
    Dict_putString(resp, String_CONST("error"), String_CONST("none"), alloc);
    Admin_sendMessage(resp, txid, context->admin);
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
        Bits_memcpy(address, conn->connectionIp6, 16);
        char* printedAddr = Sockaddr_print(addr, alloc);
        Dict_putString(d, String_CONST("ip6Address"), String_CONST(printedAddr), alloc);
        Dict_putInt(d, String_CONST("ip6Prefix"), conn->connectionIp6Prefix, alloc);
        Dict_putInt(d, String_CONST("ip6Alloc"), conn->connectionIp6Alloc, alloc);
    }

    if (!Bits_isZero(conn->connectionIp4, 4)) {
        struct Sockaddr* addr = Sockaddr_clone(Sockaddr_LOOPBACK, alloc);
        uint8_t* address;
        Assert_true(4 == Sockaddr_getAddress(addr, &address));
        Bits_memcpy(address, conn->connectionIp4, 4);
        char* printedAddr = Sockaddr_print(addr, alloc);
        Dict_putString(d, String_CONST("ip4Address"), String_CONST(printedAddr), alloc);
        Dict_putInt(d, String_CONST("ip4Prefix"), conn->connectionIp4Prefix, alloc);
        Dict_putInt(d, String_CONST("ip4Alloc"), conn->connectionIp4Alloc, alloc);
    }

    Dict_putString(d, String_CONST("key"),
                      Key_stringify(conn->routeHeader.publicKey, alloc), alloc);
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
            { .name = "ip6Prefix", .required = 0, .type = "Int" },
            { .name = "ip6Alloc", .required = 0, .type = "Int" },
            { .name = "ip4Address", .required = 0, .type = "String" },
            { .name = "ip4Prefix", .required = 0, .type = "Int" },
            { .name = "ip4Alloc", .required = 0, .type = "Int" },
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
