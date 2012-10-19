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
#include "admin/testframework/AdminTestFramework.h"

#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "memory/Allocator.h"
#include "util/Assert.h"

#include <errno.h>
#include <string.h>
#include <event2/event.h>

struct Context {
    struct AdminTestFramework* framework;

    /* slow client recv buffer */
    struct {
        char buf[256];
        unsigned int have;
    } slowClient;

    bool called;
};

static void pendingTimeout(evutil_socket_t socket, short eventType, void* vcontext)
{
    event_base_loopexit(((struct Context*)vcontext)->framework->eventBase, NULL);
}

static void handlePendingEvents(struct Context* ctx)
{
    struct event* timeoutEvent = evtimer_new(ctx->framework->eventBase, pendingTimeout, ctx);
    evtimer_add(timeoutEvent, (&(struct timeval) { .tv_sec = 0, .tv_usec = 10000 }));

    event_base_dispatch(ctx->framework->eventBase);

    evtimer_del(timeoutEvent);
}

static void slowClientIncoming(evutil_socket_t socket, short eventType, void* vcontext)
{
    struct Context* ctx = vcontext;

    ssize_t length = recv(socket,
                          ctx->slowClient.buf + ctx->slowClient.have,
                          sizeof(ctx->slowClient.buf) - ctx->slowClient.have, 0);

    if (length < 0 && EAGAIN == errno) {
        return;
    }

    Assert_always(length > 0);

    ctx->slowClient.have += length;
    Assert_always(ctx->slowClient.have < sizeof(ctx->slowClient.buf));
    ctx->slowClient.buf[ctx->slowClient.have] = 0;

    Assert_always(ctx->slowClient.have > 0);

    if (ctx->slowClient.have > 11
        && ctx->slowClient.buf[ctx->slowClient.have-1] == 'e') {

        Log_error(ctx->framework->logger, "Got cookie response '%s'", ctx->slowClient.buf);
        Assert_always(0 == memcmp("d6:cookie", ctx->slowClient.buf, 9));

        event_base_loopexit(ctx->framework->eventBase, NULL);
    }
}

static void slowClient(struct Context* ctx)
{
    struct sockaddr_storage* addr;
    int addrLen;

    Admin_getConnectInfo(&addr, &addrLen, NULL, ctx->framework->admin);

    int sock = socket(addr->ss_family, SOCK_STREAM, 0);

    Assert_always(sock >= 0);
    Assert_always(0 == connect(sock, (struct sockaddr*) addr, addrLen));

    evutil_make_socket_nonblocking(sock);

    struct event* socketEvent =
        event_new(ctx->framework->eventBase, sock, EV_READ | EV_PERSIST, slowClientIncoming, ctx);
    event_add(socketEvent, NULL);

    send(sock, "d1:q6", 5, 0);
    handlePendingEvents(ctx);
    send(sock, ":cookie", 7, 0);
    handlePendingEvents(ctx);
    send(sock, "e", 1, 0);

    event_base_dispatch(ctx->framework->eventBase);

    event_del(socketEvent);
    EVUTIL_CLOSESOCKET(sock);
}



static void adminFunc(Dict* input, void* vcontext, String* txid)
{
    struct Context* ctx = vcontext;
    ctx->called = true;
    Dict d = Dict_CONST(String_CONST("called!"), Int_OBJ(1), NULL);
    Admin_sendMessage(&d, txid, ctx->framework->admin);
}

static void standardClient(struct Context* ctx)
{
    ctx->called = false;
    struct AdminClient_Result* res =
        AdminClient_rpcCall(String_CONST("adminFunc"),
                            NULL,
                            ctx->framework->client,
                            ctx->framework->alloc);

    Assert_always(!res->err);
    Assert_always(Dict_getInt(res->responseDict, String_CONST("called!")));
    Assert_always(ctx->called);
}

int main()
{
    struct AdminTestFramework* framework = AdminTestFramework_setUp();
    struct Context ctx = {
        .framework = framework
    };
    Admin_registerFunction("adminFunc", adminFunc, &ctx, true, NULL, framework->admin);
/*
    struct Context ctx;
    memset(&ctx, 0, sizeof(ctx));

    ctx.alloc = MallocAllocator_new(1<<20);
    ctx.logger.writer = FileWriter_new(stdout, ctx.alloc);
    ctx.eventBase = event_base_new();

    struct sockaddr_storage addr;
    int addrLen = sizeof(struct sockaddr_storage);
    memset(&addr, 0, sizeof(struct sockaddr_storage));

    String* password = String_CONST("abcdefg12345");
    ctx.admin = Admin_new(&addr,
                          addrLen,
                          password,
                          NULL,
                          ctx.eventBase,
                          AbortHandler_INSTANCE,
                          &ctx.logger,
                          ctx.alloc);
*/




    //Assert_always(String_equals(password, ctx.password));

    standardClient(&ctx);
    slowClient(&ctx);
}
