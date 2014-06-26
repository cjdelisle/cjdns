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
#include "exception/Except.h"
#include "interface/Interface.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Hex.h"

struct UDPAddrInterface_pvt
{
    struct AddrInterface pub;
    struct Log* logger;

    /** Job to close the handle when the allocator is freed */
    struct Allocator_OnFreeJob* closeHandleOnFree;

    /** Job which blocks the freeing until the callback completes */
    struct Allocator_OnFreeJob* blockFreeInsideCallback;

    uv_udp_t uvHandle;
    int queueLen;

    /** true if we are inside of the callback, used by blockFreeInsideCallback */
    int inCallback;

    Identity
};

struct UDPAddrInterface_WriteRequest_pvt {
    uv_udp_send_t uvReq;
    int32_t length;
    struct UDPAddrInterface_pvt* udp;
    struct Message* msg;
    struct Allocator* alloc;
    Identity
};

struct UDPAddrInterface_pvt* ifaceForHandle(uv_udp_t* handle)
{
    char* hp = ((char*)handle) - offsetof(struct UDPAddrInterface_pvt, uvHandle);
    return Identity_check((struct UDPAddrInterface_pvt*) hp);
}

static void sendComplete(uv_udp_send_t* uvReq, int error)
{
    struct UDPAddrInterface_WriteRequest_pvt* req =
        Identity_check((struct UDPAddrInterface_WriteRequest_pvt*) uvReq);
    if (error) {
        Log_info(req->udp->logger, "DROP Failed to write to UDPAddrInterface [%s]",
                 uv_strerror(error) );
    }
    Assert_true(req->msg->length == req->length);
    req->udp->queueLen -= req->msg->length;
    Assert_true(req->udp->queueLen >= 0);
    Allocator_free(req->alloc);
}


static uint8_t sendMessage(struct Message* m, struct Interface* iface)
{
    struct UDPAddrInterface_pvt* context = Identity_check((struct UDPAddrInterface_pvt*) iface);

    if (context->queueLen > UDPAddrInterface_MAX_QUEUE) {
        Log_warn(context->logger, "DROP Maximum queue length reached");
        return Error_NONE;
        //return Error_UNDELIVERABLE;
    }

    // This allocator will hold the message allocator in existance after it is freed.
    struct Allocator* reqAlloc = Allocator_child(context->pub.generic.allocator);
    if (m->alloc) {
        Allocator_adopt(reqAlloc, m->alloc);
    } else {
        m = Message_clone(m, reqAlloc);
    }

    struct UDPAddrInterface_WriteRequest_pvt* req =
        Allocator_clone(reqAlloc, (&(struct UDPAddrInterface_WriteRequest_pvt) {
            .udp = context,
            .msg = m,
            .alloc = reqAlloc
        }));
    Identity_set(req);

    struct Sockaddr_storage ss;
    Message_pop(m, &ss, context->pub.addr->addrLen, NULL);
    Assert_true(ss.addr.addrLen == context->pub.addr->addrLen);

    req->length = m->length;

    uv_buf_t buffers[] = {
        { .base = (char*)m->bytes, .len = m->length }
    };

    int ret = 0;
    uv_udp_send(&req->uvReq, &context->uvHandle, buffers, 1,
                (const struct sockaddr*)ss.nativeAddr, (uv_udp_send_cb)&sendComplete);

    if (ret) {
        Log_info(context->logger, "DROP Failed writing to UDPAddrInterface [%s]",
                 uv_strerror(ret));
        Allocator_free(req->alloc);
        return Error_NONE;
        //return Error_UNDELIVERABLE;
    }
    context->queueLen += m->length;

    return Error_NONE;
}

#if UDPAddrInterface_PADDING_AMOUNT < 8
    #error
#endif
#define ALLOC(buff) (((struct Allocator**) &(buff[-(8 + (((uintptr_t)buff) % 8))]))[0])

static void incoming(uv_udp_t* handle,
                     ssize_t nread,
                     const uv_buf_t* buf,
                     const struct sockaddr* addr,
                     unsigned flags)
{
    struct UDPAddrInterface_pvt* context = ifaceForHandle(handle);

    context->inCallback = 1;

    // Grab out the allocator which was placed there by allocate()
    struct Allocator* alloc = buf->base ? ALLOC(buf->base) : NULL;

    // if nread < 0, we used to log uv_last_error, which doesn't exist anymore.
    if (nread == 0) {
        // Happens constantly
        //Log_debug(context->logger, "0 length read");

    } else if (context->pub.generic.receiveMessage) {
        struct Message* m = Allocator_malloc(alloc, sizeof(struct Message));
        m->length = nread;
        m->padding = UDPAddrInterface_PADDING_AMOUNT + context->pub.addr->addrLen;
        m->capacity = buf->len;
        m->bytes = (uint8_t*)buf->base;
        m->alloc = alloc;
        Message_push(m, addr, context->pub.addr->addrLen - 8, NULL);

        // make sure the sockaddr doesn't have crap in it which will
        // prevent it from being used as a lookup key
        Sockaddr_normalizeNative((struct sockaddr*) m->bytes);

        Message_push(m, &context->pub.addr->addrLen, 8, NULL);

        /*uint8_t buff[256] = {0};
        Assert_true(Hex_encode(buff, 255, m->bytes, context->pub.addr->addrLen));
        Log_debug(context->logger, "Message from [%s]", buff);*/

        Interface_receiveMessage(&context->pub.generic, m);
    }

    if (alloc) {
        Allocator_free(alloc);
    }

    context->inCallback = 0;
    if (context->blockFreeInsideCallback) {
        Allocator_onFreeComplete((struct Allocator_OnFreeJob*) context->blockFreeInsideCallback);
    }
}

static void allocate(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    struct UDPAddrInterface_pvt* context = ifaceForHandle((uv_udp_t*)handle);

    size = UDPAddrInterface_BUFFER_CAP;
    size_t fullSize = size + UDPAddrInterface_PADDING_AMOUNT + context->pub.addr->addrLen;

    struct Allocator* child = Allocator_child(context->pub.generic.allocator);
    char* buff = Allocator_malloc(child, fullSize);
    buff += UDPAddrInterface_PADDING_AMOUNT + context->pub.addr->addrLen;

    ALLOC(buff) = child;

    buf->base = buff;
    buf->len = size;
}

static void onClosed(uv_handle_t* wasClosed)
{
    struct UDPAddrInterface_pvt* context =
        Identity_check((struct UDPAddrInterface_pvt*) wasClosed->data);
    Allocator_onFreeComplete((struct Allocator_OnFreeJob*) context->closeHandleOnFree);
}

static int closeHandleOnFree(struct Allocator_OnFreeJob* job)
{
    struct UDPAddrInterface_pvt* context =
        Identity_check((struct UDPAddrInterface_pvt*) job->userData);
    context->closeHandleOnFree = job;
    uv_close((uv_handle_t*)&context->uvHandle, onClosed);
    return Allocator_ONFREE_ASYNC;
}

static int blockFreeInsideCallback(struct Allocator_OnFreeJob* job)
{
    struct UDPAddrInterface_pvt* context =
        Identity_check((struct UDPAddrInterface_pvt*) job->userData);
    if (!context->inCallback) {
        return 0;
    }
    context->blockFreeInsideCallback = job;
    return Allocator_ONFREE_ASYNC;
}

struct AddrInterface* UDPAddrInterface_new(struct EventBase* eventBase,
                                           struct Sockaddr* addr,
                                           struct Allocator* alloc,
                                           struct Except* exHandler,
                                           struct Log* logger)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);

    struct UDPAddrInterface_pvt* context =
        Allocator_clone(alloc, (&(struct UDPAddrInterface_pvt) {
            .pub = {
                .generic = {
                    .sendMessage = sendMessage,
                    .allocator = alloc
                },
            },
            .logger = logger
        }));
    Identity_set(context);

    if (addr) {
        Log_debug(logger, "Binding to address [%s]", Sockaddr_print(addr, alloc));
    }

    struct Sockaddr_storage ss;
    if (!addr) {
        Sockaddr_parse("0.0.0.0:0", &ss);
        addr = &ss.addr;
    }

    uv_udp_init(base->loop, &context->uvHandle);
    context->uvHandle.data = context;

    int ret;
    void* native = Sockaddr_asNative(addr);
    ret = uv_udp_bind(&context->uvHandle, (const struct sockaddr*)native, 0);

    if (ret) {
        Except_throw(exHandler, "call to uv_udp_bind() failed [%s]",
                     uv_strerror(ret));
    }

    ret = uv_udp_recv_start(&context->uvHandle, allocate, incoming);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Except_throw(exHandler, "uv_udp_recv_start() failed [%s]", err);
    }

    int nameLen = sizeof(struct Sockaddr_storage);
    Bits_memset(&ss, 0, sizeof(struct Sockaddr_storage));
    ret = uv_udp_getsockname(&context->uvHandle, (void*)ss.nativeAddr, &nameLen);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Except_throw(exHandler, "uv_udp_getsockname() failed [%s]", err);
    }
    ss.addr.addrLen = nameLen + 8;

    context->pub.addr = Sockaddr_clone(&ss.addr, alloc);
    Log_debug(logger, "Bound to address [%s]", Sockaddr_print(context->pub.addr, alloc));

    Allocator_onFree(alloc, closeHandleOnFree, context);
    Allocator_onFree(alloc, blockFreeInsideCallback, context);

    return &context->pub;
}
