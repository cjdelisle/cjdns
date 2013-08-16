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
#include "memory/Allocator.h"
#include "interface/Interface.h"
#include "util/platform/libc/strlen.h"
#include "util/events/Pipe.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"

#include <uv.h>
#include <inttypes.h>
#include <stdio.h>

struct Pipe_WriteRequest_pvt;

struct Pipe_pvt
{
    struct Pipe pub;

    uv_pipe_t server;
    uv_pipe_t peer;

    /** The memory for the shutdown needs to be preallocated so onFree does not allocate memory. */
    uv_shutdown_t serverShutdownReq;
    uv_shutdown_t peerShutdownReq;

    /**
     * If the output pipe is same as the input, this points to peer.
     * Otherwise it points to server which is reused.
     */
    uv_pipe_t* out;

    /** 1 when the pipe becomes active. */
    int isActive;

    int queueLen;

    /**
     * If the pipe's allocator is freed while this pointer is set,
     * the pointed to int is set to 1.
     */
    int* freeing;

    /** only non-null before the connection is setup. */
    struct Pipe_WriteRequest_pvt* bufferedRequest;

    struct Allocator* alloc;

    Identity
};

struct Pipe_WriteRequest_pvt {
    uv_write_t uvReq;
    struct Pipe_pvt* pipe;
    struct Message* msg;
    struct Allocator* alloc;
    Identity
};

static void sendMessageCallback(uv_write_t* uvReq, int error)
{
    struct Pipe_WriteRequest_pvt* req = Identity_cast((struct Pipe_WriteRequest_pvt*) uvReq);
    if (error) {
        Log_info(req->pipe->pub.logger, "Failed to write to pipe [%s] [%s]",
                 req->pipe->pub.fullName, uv_err_name(uv_last_error(req->pipe->out->loop)) );
    }
    req->pipe->queueLen -= req->msg->length;
    Assert_true(req->pipe->queueLen >= 0);
    Allocator_free(req->alloc);
}

static uint8_t sendMessage2(struct Pipe_WriteRequest_pvt* req)
{
    struct Pipe_pvt* pipe = req->pipe;
    struct Message* m = req->msg;

    uv_buf_t buffers[] = {
        { .base = (char*)m->bytes, .len = m->length }
    };

    if (uv_write(&req->uvReq, (uv_stream_t*) pipe->out, buffers, 1, sendMessageCallback)) {
        Log_info(pipe->pub.logger, "Failed writing to pipe [%s] [%s]",
                 pipe->pub.fullName, uv_err_name(uv_last_error(pipe->out->loop)) );
        Allocator_free(req->alloc);
        return Error_UNDELIVERABLE;
    }
    pipe->queueLen += m->length;

    return Error_NONE;
}

static uint8_t sendMessage(struct Message* m, struct Interface* iface)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) iface);

    if (pipe->queueLen > 50000) {
        return Error_LINK_LIMIT_EXCEEDED;
    }

    // This allocator will hold the message allocator in existance after it is freed.
    struct Allocator* reqAlloc = Allocator_child(pipe->alloc);
    Allocator_adopt(reqAlloc, m->alloc);

    struct Pipe_WriteRequest_pvt* req =
        Allocator_clone(reqAlloc, (&(struct Pipe_WriteRequest_pvt) {
            .pipe = pipe,
            .msg = m,
            .alloc = reqAlloc
        }));
    Identity_set(req);

    if (pipe->isActive) {
        sendMessage2(req);
    } else {
        if (!pipe->bufferedRequest) {
            Log_debug(pipe->pub.logger, "Buffering a message");
            pipe->bufferedRequest = req;
        } else {
            Log_debug(pipe->pub.logger, "Appending to the buffered message");
            uint8_t* buff =
                Allocator_malloc(reqAlloc, m->length + pipe->bufferedRequest->msg->length);
            Bits_memcpy(buff,
                        pipe->bufferedRequest->msg->bytes,
                        pipe->bufferedRequest->msg->length);
            Bits_memcpy(buff+pipe->bufferedRequest->msg->length, m->bytes, m->length);
            m->length += pipe->bufferedRequest->msg->length;
            m->capacity = m->length;
            m->bytes = buff;

            Allocator_free(pipe->bufferedRequest->alloc);
            pipe->bufferedRequest = req;
        }
    }
    return Error_NONE;
}

#define PADDING (Pipe_PADDING_AMOUNT + sizeof(uintptr_t))
#define ALLOC(buff) (((struct Allocator**) &(buff[-PADDING]))[0])

static void incoming(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) stream->data);

    // Grab out the allocator which was placed there by allocate()
    struct Allocator* alloc = buf.base ? ALLOC(buf.base) : NULL;
    int freeing = 0;
    pipe->freeing = &freeing;

    Assert_true(!alloc || alloc->free == pipe->alloc->free);

    if (nread < 0) {
        if (uv_last_error(pipe->peer.loop).code == UV_EOF) {
            Log_info(pipe->pub.logger, "Pipe closed with EOF [%s]", pipe->pub.fullName);
        } else {
            Log_warn(pipe->pub.logger, "Pipe encountered error [%s] [%s]",
                     pipe->pub.fullName, uv_err_name(uv_last_error(pipe->peer.loop)) );
        }
        if (pipe->pub.onClose) {
            pipe->pub.onClose(&pipe->pub, uv_last_error(pipe->peer.loop).code);
        }
        uv_close((uv_handle_t*) stream, NULL);

    } else if (nread == 0) {
        // This is common.
        //Log_debug(pipe->pub.logger, "Pipe 0 length read [%s]", pipe->pub.fullName);

    } else if (pipe->pub.iface.receiveMessage) {
        Assert_true(alloc);
        struct Message* m = Allocator_malloc(alloc, sizeof(struct Message));
        m->length = nread;
        m->padding = Pipe_PADDING_AMOUNT;
        m->capacity = buf.len;
        m->bytes = (uint8_t*)buf.base;
        m->alloc = alloc;
        Interface_receiveMessage(&pipe->pub.iface, m);
    }

    if (alloc && !freeing) {
        pipe->freeing = NULL;
        Allocator_free(alloc);
    }
}

static uv_buf_t allocate(uv_handle_t* handle, size_t size)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) handle->data);
    size = Pipe_BUFFER_CAP;
    size_t fullSize = size + PADDING;

    struct Allocator* child = Allocator_child(pipe->alloc);
    char* buff = Allocator_malloc(child, fullSize);
    buff += PADDING;

    ALLOC(buff) = child;

    return (uv_buf_t) { .base = buff, .len = size };
}

static void connected(uv_connect_t* req, int status)
{
    uv_stream_t* link = req->handle;
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) link->data);
    Log_debug(pipe->pub.logger, "Pipe [%s] established connection", pipe->pub.fullName);

    if (status) {
        Log_info(pipe->pub.logger, "uv_pipe_connect() failed for pipe [%s] [%s]",
                 pipe->pub.fullName, uv_err_name(uv_last_error(link->loop)) );
        uv_close((uv_handle_t*) &pipe->peer, NULL);

    } else if (uv_read_start((uv_stream_t*)&pipe->peer, allocate, incoming)) {
        Log_info(pipe->pub.logger, "uv_read_start() failed for pipe [%s] [%s]",
                 pipe->pub.fullName, uv_err_name(uv_last_error(link->loop)) );
        uv_close((uv_handle_t*) &pipe->peer, NULL);

    } else {
        pipe->isActive = 1;

        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(&pipe->pub, status);
        }

        // If there's anything buffered then send it.
        if (pipe->bufferedRequest) {
            Log_debug(pipe->pub.logger, "Sending buffered message");
            sendMessage2(pipe->bufferedRequest);
            pipe->bufferedRequest = NULL;
        }
    }
}

static void listenCallback(uv_stream_t* server, int status)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) server->data);
    if (pipe->isActive) {
        // first connection wins.
        return;
    }
    if (status == -1) {
        Log_info(pipe->pub.logger, "failed to accept pipe connection [%s] [%s]",
                 pipe->pub.fullName, uv_err_name(uv_last_error(server->loop)) );

        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(&pipe->pub, status);
        }
        return;
    }

    if (uv_accept(server, (uv_stream_t*) &pipe->peer)) {
        Log_warn(pipe->pub.logger, "uv_accept() failed: pipe [%s] [%s]",
                 pipe->pub.fullName, uv_err_name(uv_last_error(pipe->server.loop)) );
        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(&pipe->pub, -1);
        }
        uv_close((uv_handle_t*) &pipe->peer, NULL);
    } else {
        uv_connect_t req = { .handle = (uv_stream_t*) &pipe->peer };
        connected(&req, 0);
    }

    uv_close((uv_handle_t*) &pipe->server, NULL);

    #ifndef Windows
        // get rid of the pipe after it has been connected.
        uv_fs_t req;
        uv_fs_unlink(pipe->peer.loop, &req, pipe->pub.fullName, NULL);
    #endif
}

/** Asynchronous allocator freeing. */
static void onClosed(uv_shutdown_t* shutdown, int status)
{
    struct Allocator_OnFreeJob* job = Identity_cast((struct Allocator_OnFreeJob*) shutdown->data);
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*)job->userData);
    if (Bits_isZero(&pipe->server, sizeof(uv_pipe_t))
        && Bits_isZero(&pipe->peer, sizeof(uv_pipe_t)))
    {
        job->complete(job);
    }
}
static int onFree(struct Allocator_OnFreeJob* job)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*)job->userData);
    if (pipe->freeing) {
        *pipe->freeing = 1;
    }
    int skip = 0;
    pipe->serverShutdownReq.data = pipe->peerShutdownReq.data = job;
    if (uv_is_closing((uv_handle_t*) &pipe->server)
        || uv_shutdown(&pipe->serverShutdownReq, (uv_stream_t*) &pipe->server, onClosed))
    {
        Bits_memset(&pipe->serverShutdownReq, 0, sizeof(uv_shutdown_t));
        skip++;
    }
    if (uv_is_closing((uv_handle_t*) &pipe->peer)
        || uv_shutdown(&pipe->peerShutdownReq, (uv_stream_t*) &pipe->peer, onClosed))
    {
        Bits_memset(&pipe->peerShutdownReq, 0, sizeof(uv_shutdown_t));
        skip++;
    }
    if (skip == 2) {
        return 0;
    }
    return Allocator_ONFREE_ASYNC;
}

static struct Pipe_pvt* newPipe(struct EventBase* eb,
                                const char* name,
                                struct Except* eh,
                                struct Allocator* userAlloc)
{
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eb);
    struct Allocator* alloc = Allocator_child(userAlloc);

    #ifdef Windows
        #define PREFIX "\\\\.\\pipe\\cjdns_pipe_"
    #else
        #define PREFIX "/tmp/cjdns_pipe_"
    #endif

    char* cname = Allocator_malloc(alloc, strlen(PREFIX)+strlen(name)+1);
    Bits_memcpy(cname, PREFIX, strlen(PREFIX));
    Bits_memcpy(cname+strlen(PREFIX), name, strlen(name)+1);

    struct Pipe_pvt* out = Allocator_clone(alloc, (&(struct Pipe_pvt) {
        .pub = {
            .iface = {
                .sendMessage = sendMessage,
                .allocator = userAlloc
            },
            .fullName = cname,
            .name = &cname[sizeof(PREFIX) - 1],
            .base = eb
        },
        .alloc = alloc
    }));

    if (uv_pipe_init(ctx->loop, &out->peer, 0) || uv_pipe_init(ctx->loop, &out->server, 0)) {
        Except_raise(eh, -1, "uv_pipe_init() failed [%s]", uv_err_name(uv_last_error(ctx->loop)));
    }

    #ifdef Windows
        out->pub.fd = &out->peer.handle;
    #else
        out->pub.fd = &out->peer.io_watcher.fd;
    #endif

    Allocator_onFree(alloc, onFree, out);

    out->peer.data = out;
    out->server.data = out;
    out->out = &out->peer;
    Identity_set(out);

    return out;
}

struct Pipe* Pipe_forFiles(int inFd,
                           int outFd,
                           struct EventBase* eb,
                           struct Except* eh,
                           struct Allocator* userAlloc)
{
    char buff[32] = {0};
    snprintf(buff, 31, "forFiles(%d,%d)", inFd, outFd);

    struct Pipe_pvt* out = newPipe(eb, buff, eh, userAlloc);
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eb);

    if (uv_pipe_open(&out->peer, inFd)) {
        Except_raise(eh, -1, "uv_pipe_open(inFd) failed [%s]",
                     uv_err_name(uv_last_error(ctx->loop)));
    }

    if (inFd != outFd) {
        out->out = &out->server;
        if (uv_pipe_open(out->out, outFd)) {
            Except_raise(eh, -1, "uv_pipe_open(outFd) failed [%s]",
                         uv_err_name(uv_last_error(ctx->loop)));
        }
    }

    uv_connect_t req = { .handle = (uv_stream_t*) &out->peer };
    connected(&req, 0);

    return &out->pub;
}

struct Pipe* Pipe_named(const char* name,
                        struct EventBase* eb,
                        struct Except* eh,
                        struct Allocator* userAlloc)
{
    struct Pipe_pvt* out = newPipe(eb, name, eh, userAlloc);
    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eb);

    // Attempt to create pipe.
    if (!uv_pipe_bind(&out->server, out->pub.fullName)) {
        if (uv_listen((uv_stream_t*) &out->server, 1, listenCallback)) {
            Except_raise(eh, -1, "uv_listen() failed [%s] for pipe [%s]",
                         uv_err_name(uv_last_error(ctx->loop)), out->pub.fullName);
        }
        return &out->pub;
    }

    if (uv_last_error(ctx->loop).code == UV_EADDRINUSE) {
        // Pipe exists, connect to it.
        uv_connect_t* req = Allocator_malloc(out->alloc, sizeof(uv_connect_t));
        req->data = out;
        uv_pipe_connect(req, &out->peer, out->pub.fullName, connected);
        return &out->pub;
    }

    Except_raise(eh, -1, "uv_pipe_bind() failed [%s] for pipe [%s]",
                 uv_err_name(uv_last_error(ctx->loop)), out->pub.fullName);

    return &out->pub;
}
