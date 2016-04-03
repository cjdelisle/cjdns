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
#include "util/events/libuv/UvWrapper.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/events/FileNo.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/log/Log.h"
#include "util/Identity.h"

#include <inttypes.h>
#include <stdio.h>

// This structure used to be public and accessable.
struct FileNo
{
    /** The name of the file eg: "/tmp/cjdns_fileno_foo" */
    const char* const pipePath;

    void* userData;

    enum FileNo_Type type;

    struct EventBase* const base;

    struct Allocator* alloc;

    struct Log* logger;
};

struct FileNoContext
{
    struct FileNo_Promise pub;
    struct FileNo* fileno;

    Identity
};

struct FileNo_pvt
{
    struct FileNo pub;

    uv_pipe_t server;
    uv_pipe_t peer;

    /** Job to close the handles when the allocator is freed */
    struct Allocator_OnFreeJob* closeHandlesOnFree;

    int isActive;

    Identity
};

static void onClose(uv_handle_t* handle)
{
    struct FileNo_pvt* fileno = Identity_check((struct FileNo_pvt*)handle->data);
    handle->data = NULL;
    if (fileno->closeHandlesOnFree && !fileno->server.data && !fileno->peer.data) {
        Allocator_onFreeComplete((struct Allocator_OnFreeJob*) fileno->closeHandlesOnFree);
    }
}

#if FileNo_PADDING_AMOUNT < 8
    #error
#endif
#define ALLOC(buff) (((struct Allocator**) &(buff[-(8 + (((uintptr_t)buff) % 8))]))[0])

static void incoming(uv_pipe_t* stream,
                     ssize_t nread,
                     const uv_buf_t* buf,
                     uv_handle_type pending)
{
    // Grab out the allocator which was placed there by allocate()
    struct Allocator* alloc = buf->base ? ALLOC(buf->base) : NULL;

    Assert_true(pending == UV_UNKNOWN_HANDLE);

    if (nread < 0) {
        uv_close((uv_handle_t*) stream, onClose);
#ifndef win32
    } else {
        struct FileNo_pvt* fileno = Identity_check((struct FileNo_pvt*) stream->data);
        if (fileno->peer.accepted_fd != -1) {
            struct FileNoContext* fctx = (struct FileNoContext*) fileno->pub.userData;
            if (fctx->pub.callback) {
                fctx->pub.callback(&fctx->pub, fileno->peer.accepted_fd);
            }
        }
#endif
    }

    if (alloc) {
        Allocator_free(alloc);
    }
}

static void allocate(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    struct FileNo_pvt* pipe = Identity_check((struct FileNo_pvt*) handle->data);
    size = FileNo_BUFFER_CAP;
    size_t fullSize = size + FileNo_PADDING_AMOUNT;

    struct Allocator* child = Allocator_child(pipe->pub.alloc);
    char* buff = Allocator_malloc(child, fullSize);
    buff += FileNo_PADDING_AMOUNT;

    ALLOC(buff) = child;

    buf->base = buff;
    buf->len = size;
}

static void connected(uv_connect_t* req, int status)
{
    uv_stream_t* link = req->handle;
    struct FileNo_pvt* fileno = Identity_check((struct FileNo_pvt*) link->data);
    Log_debug(fileno->pub.logger, "FileNo [%s] established connection", fileno->pub.pipePath);

    int ret;
    if (status) {
        Log_info(fileno->pub.logger, "uv_pipe_connect() failed for pipe [%s] [%s]",
                 fileno->pub.pipePath, uv_strerror(status) );
        uv_close((uv_handle_t*) &fileno->peer, onClose);

    } else if ((ret = uv_read2_start((uv_stream_t*)&fileno->peer, allocate, incoming))) {
        Log_info(fileno->pub.logger, "uv_read2_start() failed for pipe [%s] [%s]",
                 fileno->pub.pipePath, uv_strerror(ret));
        uv_close((uv_handle_t*) &fileno->peer, onClose);

    } else {
        fileno->isActive = 1;
    }
}

static void listenCallback(uv_stream_t* server, int status)
{
    struct FileNo_pvt* fileno = Identity_check((struct FileNo_pvt*) server->data);
    if (fileno->isActive) {
        // first connection wins.
        return;
    }
    if (status == -1) {
        Log_info(fileno->pub.logger, "failed to accept pipe connection [%s] [%s]",
                 fileno->pub.pipePath, uv_strerror(status) );
        return;
    }

    Log_info(fileno->pub.logger, "Try accept pipe connection [%s]",
            fileno->pub.pipePath);

    int ret = uv_accept(server, (uv_stream_t*) &fileno->peer);
    if (ret) {
        Log_warn(fileno->pub.logger, "uv_accept() failed: pipe [%s] [%s]",
                 fileno->pub.pipePath, uv_strerror(ret) );
        uv_close((uv_handle_t*) &fileno->peer, onClose);
    } else {
        uv_connect_t req = { .handle = (uv_stream_t*) &fileno->peer };
        connected(&req, 0);
    }
}

static int closeHandlesOnFree(struct Allocator_OnFreeJob* job)
{
    struct FileNo_pvt* fileno = Identity_check((struct FileNo_pvt*)job->userData);
    fileno->closeHandlesOnFree = job;
    int skip = 2;
    if (fileno->server.data) {
        uv_close((uv_handle_t*) &fileno->server, NULL);
        skip--;
    }
    if (fileno->peer.data) {
        uv_close((uv_handle_t*) &fileno->peer, NULL);
        skip--;
    }
    if (skip == 2) {
        return 0;
    }
    return Allocator_ONFREE_ASYNC;
}

static struct FileNo* FileNo_new(const char* path,
                                 struct EventBase* eb,
                                 struct Except* eh,
                                 struct Log* logger,
                                 struct Allocator* userAlloc)
{
    struct EventBase_pvt* ctx = EventBase_privatize(eb);
    struct Allocator* alloc = Allocator_child(userAlloc);
    String* pathStr = String_new(path, alloc);

    struct FileNo_pvt* out = Allocator_clone(alloc, (&(struct FileNo_pvt) {
        .pub = {
            .pipePath = pathStr->bytes,
            .base = eb,
            .logger = logger,
            .alloc = alloc
        }
    }));

    int ret = uv_pipe_init(ctx->loop, &out->peer, 1);
    if (ret) {
        Except_throw(eh, "uv_pipe_init() failed [%s]", uv_strerror(ret));
    }

    ret = uv_pipe_init(ctx->loop, &out->server, 1);
    if (ret) {
        Except_throw(eh, "uv_pipe_init() failed [%s]", uv_strerror(ret));
    }

    Allocator_onFree(alloc, closeHandlesOnFree, out);

    out->peer.data = out;
    out->server.data = out;
    Identity_set(out);

    uv_fs_t req;
    uv_fs_unlink(out->peer.loop, &req, out->pub.pipePath, NULL);

    ret = uv_pipe_bind(&out->server, out->pub.pipePath);
    if (!ret) {
        ret = uv_listen((uv_stream_t*) &out->server, 1, listenCallback);
        if (ret) {
            Except_throw(eh, "uv_listen() failed [%s] for pipe [%s]",
                         uv_strerror(ret), out->pub.pipePath);
        }

        return &out->pub;
    }

    Except_throw(eh, "uv_pipe_bind() failed [%s] for pipe [%s]",
                 uv_strerror(ret), out->pub.pipePath);

    return &out->pub;
}

static struct FileNo_Promise* FileNo_newFile(const char* path,
                                             struct EventBase* eb,
                                             struct Except* eh,
                                             struct Log* logger,
                                             struct Allocator* alloc)
{
    struct FileNo* fn = FileNo_new(path, eb, eh, logger, alloc);
    struct FileNoContext* fctx = Allocator_clone(fn->alloc, (&(struct FileNoContext) {
                .pub = {
                    .alloc = fn->alloc
                },
                .fileno = fn
            }));
    Identity_set(fctx);

    fn->userData = fctx;

    return &fctx->pub;
}

struct FileNo_Promise* FileNo_import(const char* path,
                                     struct EventBase* eb,
                                     struct Except* eh,
                                     struct Log* logger,
                                     struct Allocator* alloc)
{
    return FileNo_newFile(path, eb, eh, logger, alloc);
}
