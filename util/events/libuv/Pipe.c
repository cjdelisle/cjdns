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
#include "exception/Er.h"
#include "memory/Allocator.h"
#include "util/events/Pipe.h"
#include "util/events/libuv/Pipe_pvt.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/CString.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "benc/String.h"

#include <inttypes.h>
#include <libgen.h>
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>

struct Pipe_WriteRequest_pvt;

struct Pipe_pvt
{
    struct Pipe pub;

    uv_pipe_t peer;

    /** Job to close the handles when the allocator is freed */
    struct Allocator_OnFreeJob* closeHandlesOnFree;

    /** Job which blocks the freeing until the callback completes */
    struct Allocator_OnFreeJob* blockFreeInsideCallback;

    // true if we can pass file descriptors through this pipe
    bool ipc;

    /** 1 when the pipe becomes active. */
    int isActive;

    int queueLen;

    /** Used by blockFreeInsideCallback */
    int isInCallback;

    /** only non-null before the connection is setup. */
    struct Pipe_WriteRequest_pvt* bufferedRequest;

    struct Allocator* alloc;

    struct Log* log;

    struct EventBase* base;

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
    void *glock = Rffi_glock();
    struct Pipe_WriteRequest_pvt* req = Identity_check((struct Pipe_WriteRequest_pvt*) uvReq);
    if (error) {
        Log_info(req->pipe->log, "Failed to write to pipe [%s] [%s]",
                 req->pipe->pub.fullName, uv_strerror(error) );
    }
    req->pipe->queueLen -= Message_getLength(req->msg);
    Assert_ifParanoid(req->pipe->queueLen >= 0);
    Allocator_free(req->alloc);
    Rffi_gunlock(glock);
}

static void sendMessage2(struct Pipe_WriteRequest_pvt* req)
{
    struct Pipe_pvt* pipe = req->pipe;
    struct Message* m = req->msg;

    uv_buf_t buffers[] = {
        { .base = (char*)m->msgbytes, .len = Message_getLength(m) }
    };

    int ret = -1;
    int fd = Message_getAssociatedFd(m);
    if (pipe->ipc && fd > -1 && !Defined(win32)) {
        uv_stream_t* fake_handle = Allocator_calloc(req->alloc, sizeof(uv_stream_t), 1);
        #ifndef win32
            fake_handle->io_watcher.fd = fd;
        #endif
        fake_handle->type = UV_TCP;
        ret = uv_write2(
            &req->uvReq,
            (uv_stream_t*) &pipe->peer,
            buffers,
            1,
            fake_handle,
            sendMessageCallback);
        Log_debug(pipe->log, "Sending message with fd [%d]", fd);
    } else {
        Log_debug(pipe->log, "Sending message of length [%d]", Message_getLength(m));
        ret = uv_write(&req->uvReq, (uv_stream_t*) &pipe->peer, buffers, 1, sendMessageCallback);
    }
    if (ret) {
        Log_info(pipe->log, "Failed writing to pipe [%s] [%s]",
                 pipe->pub.fullName, uv_strerror(ret) );
        Allocator_free(req->alloc);
        return;
    }
    pipe->queueLen += Message_getLength(m);

    return;
}

static Iface_DEFUN sendMessage(struct Message* m, struct Iface* iface)
{
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*) iface);

    if (pipe->queueLen > 50000) {
        return Error(m, "OVERFLOW");
    }

    // This allocator will hold the message allocator in existance after it is freed.
    struct Allocator* reqAlloc = Allocator_child(pipe->alloc);
    Allocator_adopt(reqAlloc, Message_getAlloc(m));

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
            Log_debug(pipe->log, "Buffering a message");
            pipe->bufferedRequest = req;
        } else {
            Log_debug(pipe->log, "Appending to the buffered message");
            struct Message* m2 = Message_new(0,
                Message_getLength(m) + Message_getLength(pipe->bufferedRequest->msg), reqAlloc);
            Er_assert(Message_epush(m2, m->msgbytes, Message_getLength(m)));
            Er_assert(Message_epush(m2,
                pipe->bufferedRequest->msg->msgbytes,
                Message_getLength(pipe->bufferedRequest->msg)));
            req->msg = m2;
            Allocator_free(pipe->bufferedRequest->alloc);
            pipe->bufferedRequest = req;
        }
    }
    return NULL;
}

/** Asynchronous allocator freeing. */
static void onClose(uv_handle_t* handle)
{
    void *glock = Rffi_glock();
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*)handle->data);
    handle->data = NULL;
    Log_debug(pipe->log, "Pipe closed");
    Assert_true(pipe->closeHandlesOnFree && !pipe->peer.data);
    Allocator_onFreeComplete((struct Allocator_OnFreeJob*) pipe->closeHandlesOnFree);
    Rffi_gunlock(glock);
}

#if Pipe_PADDING_AMOUNT < 8
    #error
#endif
#define ALLOC(buff) (((struct Message**) &(buff[-(8 + (((uintptr_t)buff) % 8))]))[0])

static void incoming(uv_stream_t* stream, ssize_t nread, const uv_buf_t* buf)
{
    void *glock = Rffi_glock();
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*) stream->data);

    // Grab out the msg which was placed there by allocate()
    struct Message* msg = buf->base ? ALLOC(buf->base) : NULL;
    pipe->isInCallback = 1;

    if (nread < 0) {
        if (pipe->pub.onClose) {
            pipe->pub.onClose(&pipe->pub, 0);
        }

    } else if (nread == 0) {
        // This is common.
        //Log_debug(pipe->log, "Pipe 0 length read [%s]", pipe->pub.fullName);

    } else {
        Assert_true(msg);
        Er_assert(Message_truncate(msg, nread));
        if (pipe->ipc) {
            #ifndef win32
                Message_setAssociatedFd(msg, stream->accepted_fd);
            #endif
        }
        Log_debug(pipe->log, "Pipe incoming message len [%d]", Message_getLength(msg));
        Iface_send(&pipe->pub.iface, msg);
    }

    if (msg) {
        Allocator_free(Message_getAlloc(msg));
    }

    pipe->isInCallback = 0;
    if (pipe->blockFreeInsideCallback) {
        Allocator_onFreeComplete((struct Allocator_OnFreeJob*) pipe->blockFreeInsideCallback);
    }
    Rffi_gunlock(glock);
}

static void incoming2(uv_pipe_t* stream, ssize_t nread, const uv_buf_t* buf, uv_handle_type _)
{
    incoming((uv_stream_t*)stream, nread, buf);
}

static void allocate(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    void *glock = Rffi_glock();
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*) handle->data);
    size = Pipe_BUFFER_CAP;

    struct Allocator* child = Allocator_child(pipe->alloc);
    struct Message* msg = Message_new(size, Pipe_PADDING_AMOUNT, child);

    ALLOC(msg->msgbytes) = msg;

    buf->base = msg->msgbytes;
    buf->len = size;
    Rffi_gunlock(glock);
}

static int startPipe(struct Pipe_pvt* pipe)
{
    if (pipe->ipc) {
        return uv_read2_start((uv_stream_t*)&pipe->peer, allocate, incoming2);
    } else {
        return uv_read_start((uv_stream_t*)&pipe->peer, allocate, incoming);
    }
}

static void connected(uv_connect_t* req, int status)
{
    void *glock = Rffi_glock();
    uv_stream_t* link = req->handle;
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*) link->data);
    Log_debug(pipe->log, "Pipe [%s] established connection", pipe->pub.fullName);

    int ret;
    if (status) {
        Log_info(pipe->log, "uv_pipe_connect() failed for pipe [%s] [%s]",
                 pipe->pub.fullName, uv_strerror(status) );
        uv_close((uv_handle_t*) &pipe->peer, NULL);

    } else if ((ret = startPipe(pipe))) {
        Log_info(pipe->log, "uv_read_start() failed for pipe [%s] [%s]",
                 pipe->pub.fullName, uv_strerror(ret));
        uv_close((uv_handle_t*) &pipe->peer, NULL);

    } else {
        pipe->isActive = 1;

        if (pipe->pub.onConnection) {
            pipe->pub.onConnection(&pipe->pub, status);
        }

        // If there's anything buffered then send it.
        if (pipe->bufferedRequest) {
            Log_debug(pipe->log, "Sending buffered message");
            sendMessage2(pipe->bufferedRequest);
            pipe->bufferedRequest = NULL;
        }
    }
    Rffi_gunlock(glock);
}

static int blockFreeInsideCallback(struct Allocator_OnFreeJob* job)
{
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*)job->userData);
    if (!pipe->isInCallback) {
        return 0;
    }
    pipe->blockFreeInsideCallback = job;
    return Allocator_ONFREE_ASYNC;
}

static int closeHandlesOnFree(struct Allocator_OnFreeJob* job)
{
    struct Pipe_pvt* pipe = Identity_check((struct Pipe_pvt*)job->userData);
    pipe->closeHandlesOnFree = job;
    Assert_true(pipe->peer.data);
    uv_close((uv_handle_t*) &pipe->peer, onClose);
    EventBase_wakeup(pipe->base);
    return Allocator_ONFREE_ASYNC;
}

static Er_DEFUN(struct Pipe_pvt* newPipeAny(struct EventBase* eb,
                                  const char* fullPath,
                                  bool ipc,
                                  struct Log* log,
                                  struct Allocator* userAlloc))
{
    struct EventBase_pvt* ctx = EventBase_privatize(eb);
    struct Allocator* alloc = Allocator_child(userAlloc);

    struct Pipe_pvt* out = Allocator_clone(alloc, (&(struct Pipe_pvt) {
        .pub = {
            .iface = {
                .send = sendMessage
            },
            .fullName = (fullPath) ? CString_strdup(fullPath, alloc) : NULL,
        },
        .alloc = alloc,
        .log = log,
        .ipc = ipc,
        .base = eb,
    }));

    int ret = uv_pipe_init(ctx->loop, &out->peer, ipc);
    if (ret) {
        Er_raise(alloc, "uv_pipe_init() failed [%s]", uv_strerror(ret));
    }

    Allocator_onFree(alloc, closeHandlesOnFree, out);
    Allocator_onFree(alloc, blockFreeInsideCallback, out);

    out->peer.data = out;
    Identity_set(out);

    Er_ret(out);
}

Er_DEFUN(struct Pipe* Pipe_forFd(int fd,
                        bool ipc,
                        struct EventBase* eb,
                        struct Log* log,
                        struct Allocator* userAlloc))
{
    char buff[32] = {0};
    snprintf(buff, 31, "forFd(%d)", fd);

    struct Pipe_pvt* out = Er(newPipeAny(eb, buff, ipc, log, userAlloc));

    int ret = uv_pipe_open(&out->peer, fd);
    if (ret) {
        Er_raise(out->alloc, "uv_pipe_open(inFd) failed [%s]", uv_strerror(ret));
    }

    uv_connect_t req = { .handle = (uv_stream_t*) &out->peer };
    connected(&req, 0);

    Er_ret(&out->pub);
}

Er_DEFUN(struct Pipe* Pipe_named(const char* fullPath,
                        struct EventBase* eb,
                        struct Log* log,
                        struct Allocator* userAlloc))
{
    struct Pipe_pvt* out = Er(newPipeAny(eb, fullPath, true, log, userAlloc));

    uv_connect_t* req = Allocator_malloc(out->alloc, sizeof(uv_connect_t));
    req->data = out;
    uv_pipe_connect(req, &out->peer, out->pub.fullName, connected);

    int err = 0;

    // We get the error back synchronously but windows doesn't support that
    // TODO(cjd): Find a better way
    #ifndef win32
        err = (&out->peer)->delayed_error;
    #endif

    if (err != 0) {
        Er_raise(out->alloc, "uv_pipe_connect() failed [%s] for pipe [%s]",
                     uv_strerror(err), out->pub.fullName);
    }

    Er_ret(&out->pub);
}

Er_DEFUN(struct Pipe* Pipe_serverAccept(uv_pipe_t* server,
                               const char* pipeName,
                               struct EventBase* eb,
                               struct Log* log,
                               struct Allocator* userAlloc))
{
    struct Pipe_pvt* out = Er(newPipeAny(eb, NULL, true, log, userAlloc));
    int ret = uv_accept((uv_stream_t*) server, (uv_stream_t*) &out->peer);
    if (ret) {
        uv_close((uv_handle_t*) &out->peer, NULL);
        Er_raise(out->alloc, "uv_accept() failed: pipe [%s] [%s]",
            pipeName, uv_strerror(ret));
    } else {
        uv_connect_t req = { .handle = (uv_stream_t*) &out->peer };
        connected(&req, 0);
    }
    Er_ret(&out->pub);
}

Er_DEFUN(bool Pipe_exists(const char* path, struct Allocator* errAlloc))
{
    struct stat st;
    if (stat(path, &st)) {
        if (errno == ENOENT) { Er_ret(false); }
        Er_raise(errAlloc, "Failed stat(%s) [%s]", path, strerror(errno));
    } else {
        int flag = 0;
        #ifdef win32
            flag = S_IFIFO;
        #elif defined(S_IFSOCK)
            flag = S_IFSOCK;
        #else
            #error "missing S_IFSOCK"
        #endif
        Er_ret(((int)(st.st_mode & S_IFMT)) == flag);
    }
}