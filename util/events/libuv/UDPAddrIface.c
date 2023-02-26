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
#include "exception/Except.h"
#include "interface/Iface.h"
#include "util/events/UDPAddrIface.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Hex.h"

struct UDPAddrIface_pvt
{
    struct UDPAddrIface pub;

    struct Allocator* userAlloc;
    struct Allocator* alloc;

    struct Log* logger;

    Iface_t iface;

    uv_udp_t uvHandle;
    int queueLen;

    /** true if we are inside of the callback, used by blockFreeInsideCallback */
    int inCallback;

    Identity
};

struct UDPAddrIface_WriteRequest_pvt {
    uv_udp_send_t uvReq;
    int32_t length;
    struct UDPAddrIface_pvt* udp;
    struct Message* msg;
    struct Allocator* alloc;
    Identity
};

static struct UDPAddrIface_pvt* ifaceForHandle(uv_udp_t* handle)
{
    char* hp = ((char*)handle) - offsetof(struct UDPAddrIface_pvt, uvHandle);
    return Identity_check((struct UDPAddrIface_pvt*) hp);
}

static void sendComplete(uv_udp_send_t* uvReq, int error)
{
    struct UDPAddrIface_WriteRequest_pvt* req =
        Identity_check((struct UDPAddrIface_WriteRequest_pvt*) uvReq);
    if (error) {
        Log_debug(req->udp->logger, "DROP Failed to write to UDPAddrIface [%s]",
                  uv_strerror(error) );
    }
    Assert_true(Message_getLength(req->msg) == req->length);
    req->udp->queueLen -= Message_getLength(req->msg);
    Assert_true(req->udp->queueLen >= 0);
    Allocator_free(req->alloc);
}


static Iface_DEFUN incomingFromIface(struct Message* m, struct Iface* iface)
{
    struct UDPAddrIface_pvt* context =
        Identity_containerOf(iface, struct UDPAddrIface_pvt, iface);

    Assert_true(Message_getLength(m) >= Sockaddr_OVERHEAD);
    if (((struct Sockaddr*)m->msgbytes)->flags & Sockaddr_flags_BCAST) {
        Log_debug(context->logger, "Attempted bcast, bcast unsupported");
        // bcast not supported.
        return Error(m, "UNHANDLED");
    }

    if (context->queueLen > UDPAddrIface_MAX_QUEUE) {
        Log_warn(context->logger, "DROP msg length [%d] to [%s] maximum queue length reached",
            Message_getLength(m), Sockaddr_print(context->pub.generic.addr, Message_getAlloc(m)));
        return Error(m, "OVERFLOW");
    }

    // This allocator will hold the message allocator in existance after it is freed.
    struct Allocator* reqAlloc = Allocator_child(context->alloc);
    Allocator_adopt(reqAlloc, Message_getAlloc(m));

    struct UDPAddrIface_WriteRequest_pvt* req =
        Allocator_clone(reqAlloc, (&(struct UDPAddrIface_WriteRequest_pvt) {
            .udp = context,
            .msg = m,
            .alloc = reqAlloc
        }));
    Identity_set(req);

    struct Sockaddr_storage ss;
    Er_assert(Message_epop(m, &ss, context->pub.generic.addr->addrLen));
    Assert_true(ss.addr.addrLen == context->pub.generic.addr->addrLen);

    req->length = Message_getLength(m);

    uv_buf_t buffers[] = {
        { .base = (char*)m->msgbytes, .len = Message_getLength(m) }
    };

    int ret = uv_udp_send(&req->uvReq, &context->uvHandle, buffers, 1,
                (const struct sockaddr*)ss.nativeAddr, (uv_udp_send_cb)&sendComplete);

    if (ret) {
        Log_info(context->logger, "DROP Failed writing to UDPAddrIface [%s]",
                 uv_strerror(ret));
        Allocator_free(req->alloc);
        return Error(m, "UNHANDLED");
    }
    context->queueLen += Message_getLength(m);

    return NULL;
}

static void onClosed(uv_handle_t* wasClosed)
{
    struct UDPAddrIface_pvt* context =
        Identity_check((struct UDPAddrIface_pvt*) wasClosed->data);
    Allocator_free(context->alloc);
}

#if UDPAddrIface_PADDING_AMOUNT < 8
    #error
#endif
#define ALLOC(buff) (((struct Message**) &(buff[-(8 + (((uintptr_t)buff) % 8))]))[0])

static void incoming(uv_udp_t* handle,
                     ssize_t nread,
                     const uv_buf_t* buf,
                     const struct sockaddr* addr,
                     unsigned flags)
{
    struct UDPAddrIface_pvt* context = ifaceForHandle(handle);

    context->inCallback = 1;

    // Grab out the allocator which was placed there by allocate()
    struct Message* msg = buf->base ? ALLOC(buf->base) : NULL;

    // if nread < 0, we used to log uv_last_error, which doesn't exist anymore.
    if (nread == 0) {
        // Happens constantly
        //Log_debug(context->logger, "0 length read");

    } else {
        Er_assert(Message_truncate(msg, nread));
        Er_assert(Message_epush(msg, addr, context->pub.generic.addr->addrLen - Sockaddr_OVERHEAD));

        // make sure the sockaddr doesn't have crap in it which will
        // prevent it from being used as a lookup key
        Sockaddr_normalizeNative((struct sockaddr*) msg->msgbytes);

        Er_assert(Message_epush(msg, context->pub.generic.addr, Sockaddr_OVERHEAD));

        /*uint8_t buff[256] = {0};
        Assert_true(Hex_encode(buff, 255, m->bytes, context->pub.generic.addr->addrLen));
        Log_debug(context->logger, "Message from [%s]", buff);*/

        Iface_send(context->pub.generic.iface, msg);
    }

    if (msg) {
        Allocator_free(Message_getAlloc(msg));
    }

    context->inCallback = 0;
    if (context->userAlloc == NULL) {
        uv_close((uv_handle_t*)&context->uvHandle, onClosed);
    }
}

static void allocate(uv_handle_t* handle, size_t size, uv_buf_t* buf)
{
    struct UDPAddrIface_pvt* context = ifaceForHandle((uv_udp_t*)handle);

    size = UDPAddrIface_BUFFER_CAP;

    struct Allocator* child = Allocator_child(context->alloc);
    struct Message* msg = Message_new(
        UDPAddrIface_BUFFER_CAP,
        UDPAddrIface_PADDING_AMOUNT + context->pub.generic.addr->addrLen,
        child
    );

    ALLOC(msg->msgbytes) = msg;

    buf->base = msg->msgbytes;
    buf->len = size;
}

static int onFree(struct Allocator_OnFreeJob* job)
{
    struct UDPAddrIface_pvt* context =
        Identity_check((struct UDPAddrIface_pvt*) job->userData);
    context->userAlloc = NULL;
    if (!context->inCallback) {
        uv_close((uv_handle_t*)&context->uvHandle, onClosed);
    }
    return 0;
}

int UDPAddrIface_setDSCP(struct UDPAddrIface* iface, uint8_t dscp)
{
    int res = 0;
    /* For win32 setsockopt is unable to mark the TOS field in IP header, do not support it now */
    #ifndef win32
        struct UDPAddrIface_pvt* context = Identity_check((struct UDPAddrIface_pvt*) iface);
        /* 6-bit DSCP, 2-bit ENC(useless for UDP) */
        int tos = dscp << 2;
        if (Sockaddr_getFamily(context->pub.generic.addr) == Sockaddr_AF_INET) {
            res = setsockopt(context->uvHandle.io_watcher.fd, IPPROTO_IP, IP_TOS,
                           &tos, sizeof(tos));
        } else if (Sockaddr_getFamily(context->pub.generic.addr) == Sockaddr_AF_INET6) {
            res = setsockopt(context->uvHandle.io_watcher.fd, IPPROTO_IPV6, IPV6_TCLASS,
                           &tos, sizeof(tos));
        }
    #endif
    return res;
}

int UDPAddrIface_getFd(struct UDPAddrIface* iface)
{
    int out = -1;
    #ifndef win32
        struct UDPAddrIface_pvt* context = Identity_check((struct UDPAddrIface_pvt*) iface);
        out = context->uvHandle.io_watcher.fd;
    #endif
    return out;
}

int UDPAddrIface_setBroadcast(struct UDPAddrIface* iface, bool enable)
{
    struct UDPAddrIface_pvt* context = Identity_check((struct UDPAddrIface_pvt*) iface);
    return uv_udp_set_broadcast(&context->uvHandle, enable ? 1 : 0);
}

Er_DEFUN(struct UDPAddrIface* UDPAddrIface_new(struct EventBase* eventBase,
                                      struct Sockaddr* addr,
                                      struct Allocator* userAlloc,
                                      struct Log* logger))
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    struct Allocator* alloc = Allocator_child(base->alloc);

    struct UDPAddrIface_pvt* context =
        Allocator_clone(alloc, (&(struct UDPAddrIface_pvt) {
            .logger = logger,
            .userAlloc = userAlloc,
            .alloc = alloc,
        }));
    context->pub.generic.alloc = alloc;
    context->pub.generic.iface = &context->iface;
    context->iface.send = incomingFromIface;
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
        Er_raise(alloc, "call to uv_udp_bind() failed [%s]", uv_strerror(ret));
    }

    ret = uv_udp_recv_start(&context->uvHandle, allocate, incoming);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Er_raise(alloc, "uv_udp_recv_start() failed [%s]", err);
    }

    int nameLen = sizeof(struct Sockaddr_storage);
    Bits_memset(&ss, 0, sizeof(struct Sockaddr_storage));
    ret = uv_udp_getsockname(&context->uvHandle, (void*)ss.nativeAddr, &nameLen);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Er_raise(alloc, "uv_udp_getsockname() failed [%s]", err);
    }
    ss.addr.addrLen = nameLen + 8;

    context->pub.generic.addr = Sockaddr_clone(&ss.addr, alloc);
    Log_debug(logger, "Bound to address [%s]", Sockaddr_print(context->pub.generic.addr, alloc));

    Allocator_onFree(userAlloc, onFree, context);

    Er_ret(&context->pub);
}
