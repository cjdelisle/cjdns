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
#include "util/events/libuv/UvWrapper.h"

#include "benc/String.h"
#include "exception/Er.h"
#include "memory/Allocator.h"
#include "util/events/Pipe.h"
#include "util/events/PipeServer.h"
#include "util/events/libuv/Pipe_pvt.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/CString.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <inttypes.h>
#include <libgen.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>

struct PipeServer_pvt;
struct Client
{
    struct Iface iface;
    struct Pipe* pipe;
    struct PipeServer_pvt* psp;
    struct Allocator* alloc;

    struct Sockaddr addr;
    Identity
};

#define Map_NAME Clients
#define Map_ENABLE_HANDLES
#define Map_VALUE_TYPE struct Client*
#include "util/Map.h"

struct PipeServer_pvt
{
    struct PipeServer pub;

    uv_pipe_t server;

    Iface_t iface;

    struct Map_Clients clients;

    struct Allocator_OnFreeJob* closeHandlesOnFree;

    struct Allocator* alloc;

    struct EventBase_pvt* base;

    struct Log* log;

    uint32_t nextHandle;

    Identity
};

static Iface_DEFUN sendMessage(struct Message* m, struct Iface* iface)
{
    struct PipeServer_pvt* psp = Identity_containerOf(iface, struct PipeServer_pvt, iface);
    struct Sockaddr* addr = Er_assert(AddrIface_popAddr(m));
    uint32_t handle = Sockaddr_addrHandle(addr);
    int idx = Map_Clients_indexForHandle(handle, &psp->clients);
    if (idx < 0) {
        Log_warn(psp->log, "Attempted to send a message to client [0x%x] which is gone", handle);
        return Error(m, "UNHANDLED");
    }
    struct Client* cli = psp->clients.values[idx];
    return Iface_next(&cli->iface, m);
}

static Iface_DEFUN incomingFromClient(struct Message* msg, struct Iface* iface)
{
    struct Client* cli = Identity_containerOf(iface, struct Client, iface);
    struct PipeServer_pvt* psp = Identity_check(cli->psp);
    Er_assert(AddrIface_pushAddr(msg, &cli->addr));
    return Iface_next(psp->pub.iface.iface, msg);
}

/** Asynchronous allocator freeing. */
static void onClose(uv_handle_t* handle)
{
    struct PipeServer_pvt* psp = Identity_check((struct PipeServer_pvt*)handle->data);
    handle->data = NULL;
    if (psp->closeHandlesOnFree && !psp->server.data) {
        Allocator_onFreeComplete((struct Allocator_OnFreeJob*) psp->closeHandlesOnFree);
    }
}

static struct Pipe* getPipe(struct PipeServer_pvt* psp, struct Allocator* alloc)
{
    struct Er_Ret* er = NULL;
    struct Pipe* out = Er_check(&er, Pipe_serverAccept(
            &psp->server, psp->pub.fullName, &psp->base->pub, psp->log, alloc));
    if (er) {
        Log_warn(psp->log, "failed to connect to client on pipe [%s] [%s]",
                 psp->pub.fullName, er->message);
    }
    return out;
}

static int removeClientOnFree(struct Allocator_OnFreeJob* job)
{
    struct Client* cli = Identity_check((struct Client*)job->userData);
    struct PipeServer_pvt* psp = Identity_check(cli->psp);
    uint32_t handle = Sockaddr_addrHandle(&cli->addr);
    int idx = Map_Clients_indexForHandle(handle, &psp->clients);
    if (idx > -1) {
        Map_Clients_remove(idx, &psp->clients);
    }
    return 0;
}

static void pipeOnClose(struct Pipe* p, int status)
{
    struct Client* cli = Identity_check((struct Client*) p->userData);
    struct PipeServer_pvt* psp = Identity_check(cli->psp);
    if (psp->pub.onDisconnection) {
        psp->pub.onDisconnection(&psp->pub, &cli->addr);
    }
    Allocator_free(cli->alloc);
}

static void listenCallback(uv_stream_t* server, int status)
{
    uv_pipe_t* pServer = (uv_pipe_t*) server;
    struct PipeServer_pvt* psp = Identity_containerOf(pServer, struct PipeServer_pvt, server);
    if (status == -1) {
        Log_info(psp->log, "failed to accept pipe connection [%s] [%s]",
                 psp->pub.fullName, uv_strerror(status));
        return;
    }
    struct Allocator* pipeAlloc = Allocator_child(psp->alloc);
    struct Pipe* p = getPipe(psp, pipeAlloc);
    if (p == NULL) {
        Allocator_free(pipeAlloc);
        return;
    }
    struct Client* cli = Allocator_calloc(pipeAlloc, sizeof(struct Client), 1);
    cli->iface.send = incomingFromClient;
    Iface_plumb(&cli->iface, &p->iface);
    cli->alloc = pipeAlloc;
    cli->pipe = p;
    cli->psp = psp;
    p->userData = cli;
    Identity_set(cli);
    int idx = Map_Clients_put(&cli, &psp->clients);
    uint32_t handle = psp->clients.handles[idx];
    Sockaddr_addrFromHandle(&cli->addr, handle);
    {
        // assertions
        Assert_true(handle == Sockaddr_addrHandle(&cli->addr));
        //printf("Handle is %x index is %d\n", handle, idx);
        int idx2 = Map_Clients_indexForHandle(handle, &psp->clients);
        Assert_true(idx2 == idx);
    }
    Allocator_onFree(pipeAlloc, removeClientOnFree, cli);
    if (psp->pub.onConnection) {
        psp->pub.onConnection(&psp->pub, &cli->addr);
    }
    cli->pipe->onClose = pipeOnClose;
}

static int closeHandlesOnFree(struct Allocator_OnFreeJob* job)
{
    struct PipeServer_pvt* psp = Identity_check((struct PipeServer_pvt*)job->userData);
    psp->closeHandlesOnFree = job;
    if (psp->server.data) {
        uv_close((uv_handle_t*) &psp->server, onClose);
        EventBase_wakeup(psp->base);
        return Allocator_ONFREE_ASYNC;
    }
    return 0;
}

static struct PipeServer_pvt* newPipeAny(struct EventBase* eb,
                                  const char* fullPath,
                                  struct Except* eh,
                                  struct Log* log,
                                  struct Allocator* userAlloc)
{
    struct EventBase_pvt* ctx = EventBase_privatize(eb);
    struct Allocator* alloc = Allocator_child(userAlloc);

    struct PipeServer_pvt* psp = Allocator_clone(alloc, (&(struct PipeServer_pvt) {
        .pub = {
            .iface = { .alloc = alloc },
            .fullName = CString_strdup(fullPath, alloc)
        },
        .iface = { .send = sendMessage },
        .clients = { .allocator = alloc },
        .base = ctx,
        .alloc = alloc,
        .log = log,
    }));
    psp->pub.iface.iface = &psp->iface;

    int ret = uv_pipe_init(ctx->loop, &psp->server, 0);
    if (ret) {
        Except_throw(eh, "uv_pipe_init() failed [%s]", uv_strerror(ret));
    }

    Allocator_onFree(alloc, closeHandlesOnFree, psp);

    psp->server.data = psp;
    //out->out = &out->peer;
    Identity_set(psp);

    return psp;
}

struct PipeServer* PipeServer_named(const char* fullPath,
                                    struct EventBase* eb,
                                    struct Except* eh,
                                    struct Log* log,
                                    struct Allocator* userAlloc)
{
    struct PipeServer_pvt* out = newPipeAny(eb, fullPath, eh, log, userAlloc);

    int ret = uv_pipe_bind(&out->server, out->pub.fullName);
    if (ret) {
        Except_throw(eh, "uv_pipe_bind() failed [%s] for pipe [%s]",
            uv_strerror(ret), out->pub.fullName);
    }
    ret = uv_listen((uv_stream_t*) &out->server, 1, listenCallback);
    if (ret) {
        Except_throw(eh, "uv_listen() failed [%s] for pipe [%s]",
                        uv_strerror(ret), out->pub.fullName);
    }
    return &out->pub;
}
