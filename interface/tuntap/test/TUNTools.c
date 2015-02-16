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
#include "interface/tuntap/test/TUNTools.h"
#include "util/events/UDPAddrIface.h"
#include "exception/Jmp.h"
#include "util/events/Timeout.h"

#ifdef win32
    #include <windows.h>
    #define sleep(x) Sleep(1000*x)
#else
    #include <unistd.h>
#endif

static struct AddrIface* setupUDP2(struct EventBase* base,
                                   struct Sockaddr* bindAddr,
                                   struct Allocator* allocator,
                                   struct Log* logger)
{
    struct Jmp jmp;
    Jmp_try(jmp) {
        struct UDPAddrIface* ua = UDPAddrIface_new(base, bindAddr, allocator, &jmp.handler, logger);
        return &ua->generic;
    } Jmp_catch {
        sleep(1);
        return NULL;
    }
}

struct AddrIface* TUNTools_setupUDP(struct EventBase* base,
                                    struct Sockaddr* bindAddr,
                                    struct Allocator* allocator,
                                    struct Log* logger)
{
    // Mac OSX and BSD do not set up their TUN devices synchronously.
    // We'll just keep on trying until this works.
    struct AddrInterface* udp = NULL;
    for (int i = 0; i < 20; i++) {
        if ((udp = setupUDP2(base, bindAddr, allocator, logger))) {
            break;
        }
    }
    Assert_true(udp);
    return udp;
}

struct TUNTools_pvt
{
    struct Iface iface;
    struct Sockaddr* dest;
    Identity
};

static void sendHello(void* vctx)
{
    struct TUNTools_pvt* ctx = Identity_check((struct TUNTools_pvt*) vctx);
    struct Message* msg;
    Message_STACK(msg, 0, 64);
    Message_push(msg, "Hello World", 12, NULL);
    Message_push(msg, ctx->dest, ctx->dest->addrLen, NULL);
    Iface_send(&ctx->iface, msg);
}

static Iface_DEFUN handleMessage(struct Iface* iface, struct Message* msg)
{
    Assert_failure("unexpected message");
    return NULL;
}

struct Timeout* TUNTools_sendHelloWorld(struct AddrIface* iface,
                                        struct Sockaddr* dest,
                                        struct EventBase* base,
                                        struct Allocator* alloc)
{
    struct TUNTools_pvt* ctx = Allocator_clone(alloc, (&(struct TUNTools_pvt) {
        .dest = dest
    }));
    ctx->iface.send = handleMessage;
    Iface_plumb(&ctx->iface, &iface->iface);
    
    Identity_set(ctx);
    return Timeout_setInterval(sendHello, ctx, 1000, base, alloc);
}
