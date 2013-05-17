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
#include "util/platform/Pipe.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"

#include <uv.h>

struct Pipe_pvt
{
    struct Pipe pub;

    uv_pipe_t server;
    uv_pipe_t peer;

    int isActive;

    Identity
};

struct Pipe_WriteRequest_pvt {
    uv_write_t uvReq;
    struct Pipe_pvt* pipe;
    struct Message* msg;
    Identity
};

static void sendMessageCallback(uv_write_t* req, int error)
{
    struct Pipe_WriteRequest_pvt* req = Identity_cast((struct Pipe_WriteRequest_pvt*) req);
    if (error) {
        Log_info(req->pipe.pub->logger, "Failed to write to pipe [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
    }
    // This is the aocator which we claimed, we are freeing our claim but the other claim
    // will still hold if it has not been freed.
    Allocator_free(req->msg->alloc);
}

static uint8_t sendMessage(struct Message* m, struct Interface* iface)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) iface);
    if (!pipe->isActive) {
        Log_debug(pipe->pub.logger, "Attempted to write to inactive pipe [%"PRIx64"]", pipe);
        return Error_UNDELIVERABLE;
    }

    struct Pipe_WriteRequest_pvt* req =
        Allocator_calloc(m->alloc, sizeof(struct Pipe_WriteRequest_pvt), 1);
    req->pipe = pipe;
    req->msg = m;

    uv_buf_t buffers[] = {
        { .base = m->base, .len = m->length }
    };

    // This adds a claim to the allocator so when it's creator frees it, it will not
    // be reclaimed until we call Allocator_free() on it as well.
    Allocator_claim(m->alloc);

    if (uv_write(&req->uvReq, (uv_stream_t*) &pipe->peer, buffers, 1, sendMessageCallback)) {
        Log_info(pipe->pub.logger, "Failed writing to pipe [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
        Allocator_free(m->alloc);
        return;
    }
    pipe->queueLen++;
}

static void incoming(uv_stream_t* stream, ssize_t nread, uv_buf_t buf)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) stream->data);
    // Grab out the allocator which was placed there by allocate()
    struct Allocator* alloc = (struct Allocator*) ((uintptr_t*)buf.base)[-1];
    Assert_true(alloc->free == pipe->pub.base->bufferAlloc->free);

    if (nread < 0) {
        if (uv_last_error(loop).code == UV_EOF) {
            Log_info(pipe->pub.logger, "Pipe closed with EOF [%"PRIx64"]", pipe);
        } else {
            Log_warn(pipe->pub.logger, "Pipe encountered error [%"PRIx64"] [%s]", pipe,
                     uv_err_name(uv_last_error(ctx->loop)) );
        }
        uv_close((uv_handle_t*) stream, NULL);

    } else if (nread == 0) {
        Log_debug(pipe->pub.logger, "Pipe 0 length read [%"PRIx64"]", pipe);

    } else if (pipe->pub.iface.receiveMessage) {
        struct Message* m = Allocator_malloc(alloc, sizeof(struct Message));
        m->length = nread;
        m->padding = PADDING_AMOUNT;
        m->capacity = BUFFER_CAP;
        m->base = buf.base;
        pipe->pub.iface.receiveMessage(m, pipe->pub.iface);
    }
    Allocator_free(alloc);
}

static uv_buff_t allocate(uv_handle_t* handle, size_t size)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) handle->data);
    size = (size > 2000) ? 2000 : size;
    struct Allocator* child = Allocator_child(pipe->pub.base->bufferAlloc);
    uintptr_t* buff = Allocator_malloc(child, size);
    // sneaking the allocator into the buffer...
    buff[0] = child;
    return (uv_buff_t) { .base = (char*) (buff[1]), .len = size - sizeof(uintptr_t) };
}

static void connected(uv_stream_t* link, int status)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) server->data);
    Log_debug(pipe->pub.logger, "Pipe [%"PRIx64"] established connection", pipe);

    if (pipe->pub.onConnection) {
        pipe->pub.onConnection(pipe->pub.onConnectionContext, status);
    } else {
        Log_info(pipe->pub.logger, "No onConnection callback for pipe [%"PRIx64"]", pipe);
    }

    if (status) {
        Log_info(pipe->pub.logger, "uv_pipe_connect() failed for pipe [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
        uv_close((uv_handle_t*) &pipe->peer, NULL);

    } else if (uv_read_start((uv_stream_t*)&pipe->pipe, allocate, incoming)) {
        Log_info(pipe->pub.logger, "uv_read_start() failed for pipe [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
        uv_close((uv_handle_t*) &pipe->peer, NULL);
    }


    uv_close((uv_handle_t*) &pipe->server, NULL);

    #ifndef WIN32
        // get rid of the pipe after it has been connected.
        uv_fs_t req;
        uv_fs_unlink(pipe->loop, &req, pipe->name, NULL);
    #endif
}

static void listenCallback(uv_stream_t* server, int status)
{
    struct Pipe_pvt* pipe = Identity_cast((struct Pipe_pvt*) server->data);
    if (pipe->isActive) {
        // first connection wins.
        return;
    }
    if (status == -1) {
        Log_info(pipe->pub.logger, "failed to accept pipe connection [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(pipe->pub.onConnectionContext, status);
        }
        return;
    }

    uv_pipe_init(server->loop, &pipe->peer, 0);
    pipe->peer.data = pipe;

    if (uv_accept(server, (uv_stream_t*) &pipe->peer)) {
        Log_warn(pipe->pub.logger, "uv_accept() failed: pipe [%"PRIx64"] [%s]", pipe,
                 uv_err_name(uv_last_error(ctx->loop)) );
        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(pipe->pub.onConnectionContext, -1);
        }
        uv_close((uv_handle_t*) &pipe->peer, NULL);
        uv_close((uv_handle_t*) &pipe->server, NULL);
    } else {
        connected(&pipe->peer, 0);
    }
}

static struct Pipe* newPipe(struct EventBase* eb,
                            const char* name,
                            struct Except* eh,
                            struct Allocator* alloc)
{
    #ifdef WIN32
        #define PREFIX ""
    #else
        #define PREFIX "/tmp/"
    #endif

    struct EventBase_pvt* ctx = Identity_cast((struct EventBase_pvt*) eb);
    char* cname = Allocator_malloc(alloc, strlen(PREFIX)+strlen(name)+1);
    Bits_memcpy(cname, PREFIX, strlen(PREFIX));
    Bits_memcpy(cname+strlen(PREFIX), name, strlen(name)+1);

    struct Pipe_pvt* out = Allocator_clone(alloc, (&(struct Pipe_pvt) {
        .pub = {
            .iface = {
                .sendMessage = sendMessage
            },
            .name = cname,
            .base = eb
        }
    });

    if (uv_pipe_init(ctx->loop, &out->pipe, 0)) {
        Except_raise(eh, -1 "uv_pipe_init() failed [%s]", uv_err_name(uv_last_error(ctx->loop)));
    }
    out->pipe.data = out;
    Identity_set(out);
}

struct Pipe* Pipe_new(struct EventBase* eb,
                      const char* name,
                      struct Except* eh,
                      struct Allocator* alloc)
{
    struct Pipe* out = newPipe(eb, name, eh, alloc);

    if (uv_pipe_bind(&out->pipe, out->pub.name)) {
        Except_raise(eh, -1 "uv_pipe_bind() failed [%s]", uv_err_name(uv_last_error(ctx->loop)));
    }

    if (uv_listen((uv_stream_t*) &out->pipe, 1, listenCallback)) {
        Except_raise(eh, -1 "uv_listen() failed [%s]", uv_err_name(uv_last_error(ctx->loop)));
    }

    return out;
}

struct Pipe* Pipe_connect(struct EventBase* eb,
                          const char* name,
                          struct Except* eh,
                          struct Allocator* alloc)
{
    struct Pipe* out = newPipe(eb, name, eh, alloc);
    uv_connect_t* req = Allocator_malloc(alloc, sizeof(uv_connect_t));
    req->data = out;
    uv_pipe_connect(req, &out->pipe, out->pub.name, connected);
    return out;
}
