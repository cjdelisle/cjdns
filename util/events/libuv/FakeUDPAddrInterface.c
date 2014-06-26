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

#define Map_NAME OfInterfacesByAddr
#define Map_KEY_TYPE struct Sockaddr*
#define Map_VALUE_TYPE struct Interface*
#include "util/Map.h"

struct FakeUDPAddrInterface {
};

struct AddrInterface* FakeUDPAddrInterface_new(struct EventBase* eventBase,
                                               struct Sockaddr* addr,
                                               struct Allocator* alloc,
                                               struct Except* exHandler,
                                               struct Log* logger)
{
    struct EventBase_pvt* base = EventBase_privatize(eventBase);
    Assert_true(base->faker);

    struct FakeUDPAddrInterface_pvt* context =
        Allocator_clone(alloc, (&(struct FakeUDPAddrInterface_pvt) {
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

    if (Sockaddr_getPort(addr) && ) {
//
    }

    uv_udp_init(base->loop, &context->uvHandle);
    context->uvHandle.data = context;

    int ret;
    void* native = Sockaddr_asNative(addr);
    if (Sockaddr_getFamily(addr) == Sockaddr_AF_INET6) {
        ret = uv_udp_bind6(&context->uvHandle, *((struct sockaddr_in6*)native), 0);
    } else {
        ret = uv_udp_bind(&context->uvHandle, *((struct sockaddr_in*)native), 0);
    }

    if (ret) {
        Except_throw(exHandler, "call to uv_udp_bind() failed [%s]",
                     uv_strerror(ret);
    }

    ret = uv_udp_recv_start(&context->uvHandle, allocate, incoming)
    if (ret) {
        const char* err = uv_strerror(ret);
        uv_close((uv_handle_t*) &context->uvHandle, NULL);
        Except_throw(exHandler, "uv_udp_recv_start() failed [%s]", err);
    }

    int nameLen = sizeof(struct Sockaddr_storage);
    Bits_memset(&ss, 0, sizeof(struct Sockaddr_storage));
    ret = uv_udp_getsockname(&context->uvHandle, (void*)ss.nativeAddr, &nameLen)
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
