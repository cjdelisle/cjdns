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
#include "admin/testframework/AdminTestFramework.h"
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "exception/Jmp.h"
#include "interface/addressable/UDPAddrInterface.h"
#include "interface/tuntap/TUNInterface.h"
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/events/Timeout.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"
#include "util/platform/netdev/NetDev.h"
#include "test/RootTest.h"

#include <unistd.h>
#include <stdlib.h>

#ifdef win32
    #include <windows.h>
    #define sleep(x) Sleep(1000*x)
#endif

static const uint8_t testAddrA[] = {0xfd,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
static const uint8_t testAddrB[] = {0xfd,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};

/*
 * Setup a UDPInterface and a TUNInterface, test sending traffic between them.
 */

static int receivedMessageTUNCount = 0;
static uint8_t receiveMessageTUN(struct Message* msg, struct Interface* iface)
{
    receivedMessageTUNCount++;
    uint16_t ethertype = TUNMessageType_pop(msg, NULL);
    if (ethertype != Ethernet_TYPE_IP6) {
        printf("Spurious packet with ethertype [%04x]\n", Endian_bigEndianToHost16(ethertype));
        return 0;
    }

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;

    if (msg->length != Headers_IP6Header_SIZE + Headers_UDPHeader_SIZE + 12) {
        int type = (msg->length >= Headers_IP6Header_SIZE) ? header->nextHeader : -1;
        printf("Message of unexpected length [%u] ip6->nextHeader: [%d]\n", msg->length, type);
        return 0;
    }

    Assert_true(!Bits_memcmp(header->destinationAddr, testAddrB, 16));
    Assert_true(!Bits_memcmp(header->sourceAddr, testAddrA, 16));

    Bits_memcpyConst(header->destinationAddr, testAddrA, 16);
    Bits_memcpyConst(header->sourceAddr, testAddrB, 16);

    TUNMessageType_push(msg, ethertype, NULL);

    return iface->sendMessage(msg, iface);
}

static uint8_t receiveMessageUDP(struct Message* msg, struct Interface* iface)
{
    if (!receivedMessageTUNCount) {
        return 0;
    }
    // Got the message, test successful.
    struct Allocator* alloc = iface->receiverContext;
    Allocator_free(alloc);
    return 0;
}

static void fail(void* ignored)
{
    Assert_true(!"timeout");
}

static struct AddrInterface* setupUDP(struct EventBase* base,
                                      struct Sockaddr* bindAddr,
                                      struct Allocator* allocator,
                                      struct Log* logger)
{
    struct Jmp jmp;
    Jmp_try(jmp) {
        return UDPAddrInterface_new(base, bindAddr, allocator, &jmp.handler, logger);
    } Jmp_catch {
        sleep(1);
        return NULL;
    }
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(testAddrA, Sockaddr_AF_INET6, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    struct Interface* tun = TUNInterface_new(NULL, assignedIfName, base, logger, NULL, alloc);
    NetDev_addAddress(assignedIfName, addrA, 126, logger, NULL);

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("[fd00::1]", &addr));

    #ifdef freebsd
        // tun is not setup synchronously in bsd but it lets you bind to the tun's
        // address anyway.
        sleep(1);
    #endif

    // Mac OSX and BSD do not set up their TUN devices synchronously.
    // We'll just keep on trying until this works.
    struct AddrInterface* udp = NULL;
    for (int i = 0; i < 20; i++) {
        if ((udp = setupUDP(base, &addr.addr, alloc, logger))) {
            break;
        }
    }
    Assert_true(udp);

    struct Sockaddr* dest = Sockaddr_clone(udp->addr, alloc);
    uint8_t* addrBytes;
    Assert_true(16 == Sockaddr_getAddress(dest, &addrBytes));
    Bits_memcpy(addrBytes, testAddrB, 16);

    struct Message* msg;
    Message_STACK(msg, 0, 64);
    Message_push(msg, "Hello World", 12, NULL);
    Message_push(msg, dest, dest->addrLen, NULL);

    udp->generic.receiveMessage = receiveMessageUDP;
    udp->generic.receiverContext = alloc;
    tun->receiveMessage = receiveMessageTUN;

    udp->generic.sendMessage(msg, &udp->generic);

    Timeout_setTimeout(fail, NULL, 10000, base, alloc);

    EventBase_beginLoop(base);
    return 0;
}
