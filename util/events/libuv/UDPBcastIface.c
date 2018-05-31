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
#include "util/events/UDPBcastIface.h"
#include "memory/Allocator.h"
#include "util/events/libuv/EventBase_pvt.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Identity.h"
#include "wire/Message.h"
#include "wire/Error.h"
#include "util/Hex.h"
#include "benc/String.h"

struct UDPBcastIface_pvt
{
    struct UDPBcastIface pub;

    struct Allocator* allocator;

    struct Log* logger;

    /** Job to close the handle when the allocator is freed */
    struct Allocator_OnFreeJob* closeHandleOnFree;

    /** Job which blocks the freeing until the callback completes */
    struct Allocator_OnFreeJob* blockFreeInsideCallback;

    uv_udp_t uvHandle;
    int queueLen;
    bool bcast;
    List* devices;

    /** true if we are inside of the callback, used by blockFreeInsideCallback */
    int inCallback;

    Identity
};

struct UDPBcastIface_WriteRequest_pvt {
    uv_udp_send_t uvReq;
    int32_t length;
    struct UDPBcastIface_pvt* udp;
    struct Message* msg;
    struct Allocator* alloc;
    Identity
};

static struct UDPBcastIface_pvt* ifaceForHandle(uv_udp_t* handle)
{
    char* hp = ((char*)handle) - offsetof(struct UDPBcastIface_pvt, uvHandle);
    return Identity_check((struct UDPBcastIface_pvt*) hp);
}

static void sendComplete(uv_udp_send_t* uvReq, int error)
{
    struct UDPBcastIface_WriteRequest_pvt* req =
        Identity_check((struct UDPBcastIface_WriteRequest_pvt*) uvReq);
    if (error) {
        Log_debug(req->udp->logger, "DROP Failed to write to UDPBcastIface [%s]",
                  uv_strerror(error) );
    }
    Assert_true(req->msg->length == req->length);
    req->udp->queueLen -= req->msg->length;
    Assert_true(req->udp->queueLen >= 0);
    Allocator_free(req->alloc);
}

static void sendPacket(struct Message* m, struct sockaddr* addr, struct Iface* iface)
{
    struct Message* msg;
    struct UDPBcastIface_pvt* context = Identity_check((struct UDPBcastIface_pvt*) iface);

    // This allocator will hold the message allocator in existance after it is freed.
    struct Allocator* reqAlloc = Allocator_child(context->allocator);
    msg = Message_clone(m, reqAlloc);

    struct UDPBcastIface_WriteRequest_pvt* req =
        Allocator_clone(reqAlloc, (&(struct UDPBcastIface_WriteRequest_pvt) {
            .udp = context,
            .msg = msg,
            .alloc = reqAlloc
        }));
    Identity_set(req);

    req->length = msg->length;

    uv_buf_t buffers[] = {
        { .base = (char*)msg->bytes, .len = msg->length }
    };

    int ret = uv_udp_send(&req->uvReq, &context->uvHandle, buffers, 1,
                          addr, (uv_udp_send_cb)&sendComplete);

    if (ret) {
        Log_info(context->logger, "DROP Failed writing to UDPBcastIface [%s]",
                 uv_strerror(ret));
        Allocator_free(req->alloc);
        return;
    }
    context->queueLen += msg->length;
}

static Iface_DEFUN incomingFromIface(struct Message* m, struct Iface* iface)
{
    struct UDPBcastIface_pvt* context = Identity_check((struct UDPBcastIface_pvt*) iface);

    Assert_true(m->length >= Sockaddr_OVERHEAD);

    if ((((struct Sockaddr*)m->bytes)->flags & Sockaddr_flags_BCAST) && !context->bcast) {
        Log_debug(context->logger, "Attempted bcast with bcast disabled");
        return NULL;
    }

    if (context->queueLen > UDPBcastIface_MAX_QUEUE) {
        Log_warn(context->logger, "DROP Maximum queue length reached");
        return NULL;
    }

    struct Sockaddr* sa = (struct Sockaddr*) m->bytes;
    struct Sockaddr_storage ss;
    Message_pop(m, &ss, sa->addrLen, NULL);

    struct UDPBcastIface_Header hdr = {
        .version = UDPBcastIface_CURRENT_VERSION,
        .zero = 0,
        .length_be = Endian_hostToBigEndian16(m->length + UDPBcastIface_Header_SIZE),
        .bcast = 0,
        .reversed = 0,
        .fb00_be= Endian_hostToBigEndian16(0xfb00),
    };

    if (sa->flags & Sockaddr_flags_BCAST) {
        // We will send the message to bcast addr of all selected interfaces
        uv_interface_address_t* interfaces;
        int i, j, count;
        int res = uv_interface_addresses(&interfaces, &count);
        if (res) {
            Log_warn(context->logger, "DROP message for none interface available");
            return NULL;
        }

        struct Allocator* tmpAlloc = Allocator_child(context->allocator);
        int32_t bcastCount = List_size(context->devices);

        hdr.bcast = 1;
        Message_push(m, &hdr, UDPBcastIface_Header_SIZE, NULL);

        for (i = 0; i < count; i++) {
            if (interfaces[i].is_internal) { continue; }
            if (interfaces[i].address.address4.sin_family != AF_INET) { continue; }

            for (j = 0; j < bcastCount; j++) {
                String* device = List_getString(context->devices, j);
                if (!CString_strcmp(interfaces[i].name, device->bytes)) { break; }
            }

            if (j == bcastCount) { continue; }

            // calculate the broadcast address
            struct sockaddr_in bcast4 = {
                .sin_family = AF_INET,
                .sin_port = htons(Sockaddr_getPort(context->pub.generic.addr)),
                .sin_addr = { .s_addr =
                    (interfaces[i].address.address4.sin_addr.s_addr &
                     interfaces[i].netmask.netmask4.sin_addr.s_addr) |
                        ~interfaces[i].netmask.netmask4.sin_addr.s_addr}
            };

            sendPacket(m, (struct sockaddr*)&bcast4, iface);
        }
        uv_free_interface_addresses(interfaces, count);
        Allocator_free(tmpAlloc);
    } else {
        Message_push(m, &hdr, UDPBcastIface_Header_SIZE, NULL);
        sendPacket(m, (struct sockaddr*)ss.nativeAddr, iface);
    }

    return NULL;
}

#if UDPBcastIface_PADDING_AMOUNT < 8
    #error
#endif
#define ALLOC(buff) (((struct Allocator**) &(buff[-(8 + (((uintptr_t)buff) % 8))]))[0])

static void incoming(uv_udp_t* handle,
                     ssize_t nread,
                     const uv_buf_t* buf,
                     const struct sockaddr* addr,
                     unsigned flags)
{
    struct UDPBcastIface_pvt* context = ifaceForHandle(handle);

    context->inCallback = 1;

    // Grab out the allocator which was placed there by allocate()
    struct Allocator* alloc = buf->base ? ALLOC(buf->base) : NULL;

    // if nread < 0, we used to log uv_last_error, which doesn't exist anymore.
    if (nread == 0) {
        // Happens constantly
        //Log_debug(context->logger, "0 length read");

    } else if (nread < UDPBcastIface_Header_SIZE) {
        Log_debug(context->logger, "Failed to receive udp bcast frame");
    } else {
        do {
            struct Message* m = Allocator_calloc(alloc, sizeof(struct Message), 1);
            m->length = nread;
            m->padding = UDPBcastIface_PADDING_AMOUNT + context->pub.generic.addr->addrLen;
            m->capacity = buf->len;
            m->bytes = (uint8_t*)buf->base;
            m->alloc = alloc;


            struct UDPBcastIface_Header hdr;
            Message_pop(m, &hdr, UDPBcastIface_Header_SIZE, NULL);

            if (hdr.version != UDPBcastIface_CURRENT_VERSION) {
                Log_debug(context->logger, "DROP unknown version");
                break;
            }

            uint16_t reportedLength = Endian_bigEndianToHost16(hdr.length_be);
            reportedLength -= UDPBcastIface_Header_SIZE;
            if (m->length != reportedLength) {
                if (m->length < reportedLength) {
                    Log_debug(context->logger, "DROP size field is larger than frame");
                    break;
                }
                m->length = reportedLength;
            }

            if (hdr.fb00_be != Endian_hostToBigEndian16(0xfb00)) {
                Log_debug(context->logger, "DROP bad magic");
                break;
            }

            if (!context->bcast) {
                Log_debug(context->logger, "Drop packet with bcast disabled");
                return;
            }

            struct Sockaddr laddr;
            Bits_memcpy(&laddr, context->pub.generic.addr, Sockaddr_OVERHEAD);
            if (hdr.bcast) {
                laddr.flags |= Sockaddr_flags_BCAST;
            }

            Message_push(m, addr, context->pub.generic.addr->addrLen - Sockaddr_OVERHEAD, NULL);

            // make sure the sockaddr doesn't have crap in it which will
            // prevent it from being used as a lookup key
            Sockaddr_normalizeNative((struct sockaddr*) m->bytes);

            Message_push(m, &laddr, Sockaddr_OVERHEAD, NULL);

            Iface_send(&context->pub.generic.iface, m);
        } while (0);
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
    struct UDPBcastIface_pvt* context = ifaceForHandle((uv_udp_t*)handle);

    size = UDPBcastIface_BUFFER_CAP;
    size_t fullSize = size + UDPBcastIface_PADDING_AMOUNT + context->pub.generic.addr->addrLen;

    struct Allocator* child = Allocator_child(context->allocator);
    char* buff = Allocator_malloc(child, fullSize);
    buff += UDPBcastIface_PADDING_AMOUNT + context->pub.generic.addr->addrLen;

    ALLOC(buff) = child;

    buf->base = buff;
    buf->len = size;
}

static void onClosed(uv_handle_t* wasClosed)
{
    struct UDPBcastIface_pvt* context =
        Identity_check((struct UDPBcastIface_pvt*) wasClosed->data);
    Allocator_onFreeComplete((struct Allocator_OnFreeJob*) context->closeHandleOnFree);
}

static int closeHandleOnFree(struct Allocator_OnFreeJob* job)
{
    struct UDPBcastIface_pvt* context =
        Identity_check((struct UDPBcastIface_pvt*) job->userData);
    context->closeHandleOnFree = job;
    uv_close((uv_handle_t*)&context->uvHandle, onClosed);
    return Allocator_ONFREE_ASYNC;
}

static int blockFreeInsideCallback(struct Allocator_OnFreeJob* job)
{
    struct UDPBcastIface_pvt* context =
        Identity_check((struct UDPBcastIface_pvt*) job->userData);
    if (!context->inCallback) {
        return 0;
    }
    context->blockFreeInsideCallback = job;
    return Allocator_ONFREE_ASYNC;
}

List* UDPBcastIface_listDevices(struct Allocator* alloc, struct Except* eh)
{
    List* out = List_new(alloc);
    uv_interface_address_t* interfaces;
    int i, count;
    int res = uv_interface_addresses(&interfaces, &count);
    if (res) {
        return out;
    }

    for (i = 0; i < count; i++) {
        if (interfaces[i].is_internal) { continue; }
        if (interfaces[i].address.address4.sin_family != AF_INET) { continue; }

        List_addString(out, String_new(interfaces[i].name, alloc), alloc);
    }
    uv_free_interface_addresses(interfaces, count);
    return out;
}


int UDPBcastIface_setBroadcast(struct UDPBcastIface* iface, bool enable)
{
    struct UDPBcastIface_pvt* context = Identity_check((struct UDPBcastIface_pvt*) iface);
    int res = uv_udp_set_broadcast(&context->uvHandle, enable ? 1 : 0);
    if (!res) {
        context->bcast = enable;
    }
    return res;
}

struct UDPBcastIface* UDPBcastIface_new(struct EventBase* eventBase,
                                        struct Sockaddr* addr,
                                        const List* devices,
                                        struct Allocator* alloc,
                                        struct Except* exHandler,
                                        struct Log* logger)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);

    struct UDPBcastIface_pvt* context =
        Allocator_clone(alloc, (&(struct UDPBcastIface_pvt) {
            .logger = logger,
            .bcast = true,
            .allocator = alloc
        }));
    context->pub.generic.alloc = alloc;
    context->pub.generic.iface.send = incomingFromIface;
    Identity_set(context);

    if (!addr) {
        Except_throw(exHandler, "Must assign the bcast address.");
    }
    Log_debug(logger, "Binding to address [%s]", Sockaddr_print(addr, alloc));

    if (!Sockaddr_getPort(addr)) {
        Except_throw(exHandler, "Must assign the bcast port.");
    }

    if (Sockaddr_getFamily(addr) != Sockaddr_AF_INET) {
        Except_throw(exHandler, "UDP broadcast only supported by ipv4.");
    }

    uv_udp_init(base->loop, &context->uvHandle);
    context->uvHandle.data = context;

    void* native = Sockaddr_asNative(addr);
    int ret = uv_udp_bind(&context->uvHandle, (const struct sockaddr*)native, 0);

    if (ret) {
        Except_throw(exHandler, "call to uv_udp_bind() failed [%s]",
                     uv_strerror(ret));
    }

    ret = uv_udp_set_broadcast(&context->uvHandle, 1);

    if (ret) {
        Except_throw(exHandler, "call to uv_udp_set_broadcast() failed [%s]",
                     uv_strerror(ret));
    }

    ret = uv_udp_recv_start(&context->uvHandle, allocate, incoming);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Except_throw(exHandler, "uv_udp_recv_start() failed [%s]", err);
    }

    int nameLen = sizeof(struct Sockaddr_storage);
    struct Sockaddr_storage ss;
    Bits_memset(&ss, 0, sizeof(struct Sockaddr_storage));
    ret = uv_udp_getsockname(&context->uvHandle, (void*)ss.nativeAddr, &nameLen);
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Except_throw(exHandler, "uv_udp_getsockname() failed [%s]", err);
    }
    ss.addr.addrLen = nameLen + 8;

    context->pub.generic.addr = Sockaddr_clone(&ss.addr, alloc);
    Log_debug(logger, "Bound to address [%s]", Sockaddr_print(context->pub.generic.addr, alloc));

    context->devices = List_new(alloc);

    if (devices) {
        int32_t count = List_size(devices);
        for (int32_t i = 0; i < count; i++) {
            String* device = List_getString(devices, i);
            List_addStringC(context->devices, device->bytes, alloc);
        }
    }

    Allocator_onFree(alloc, closeHandleOnFree, context);
    Allocator_onFree(alloc, blockFreeInsideCallback, context);

    return &context->pub;
}
