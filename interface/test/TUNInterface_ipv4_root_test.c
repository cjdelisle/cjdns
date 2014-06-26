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
#include "interface/addressable/UDPAddrInterface.h"
#include "interface/tuntap/TUNInterface.h"
#include "interface/tuntap/TUNMessageType.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "interface/InterfaceController.h"
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

#include <stdlib.h>

// On loan from the DoD, thanks guys.
static const uint8_t testAddrA[4] = {11, 0, 0, 1};
static const uint8_t testAddrB[4] = {11, 0, 0, 2};

/*
 * Setup a UDPAddrInterface and a TUNInterface, test sending traffic between them.
 */


static int receivedMessageTUNCount = 0;
static uint8_t receiveMessageTUN(struct Message* msg, struct Interface* iface)
{
    receivedMessageTUNCount++;
    uint16_t ethertype = TUNMessageType_pop(msg, NULL);
    if (ethertype != Ethernet_TYPE_IP4) {
        printf("Spurious packet with ethertype [%u]\n", Endian_bigEndianToHost16(ethertype));
        return 0;
    }

    struct Headers_IP4Header* header = (struct Headers_IP4Header*) msg->bytes;

    Assert_true(msg->length == Headers_IP4Header_SIZE + Headers_UDPHeader_SIZE + 12);

    Assert_true(!Bits_memcmp(header->destAddr, testAddrB, 4));
    Assert_true(!Bits_memcmp(header->sourceAddr, testAddrA, 4));

    Bits_memcpyConst(header->destAddr, testAddrA, 4);
    Bits_memcpyConst(header->sourceAddr, testAddrB, 4);

    TUNMessageType_push(msg, ethertype, NULL);

    return iface->sendMessage(msg, iface);
}

static uint8_t receiveMessageUDP(struct Message* msg, struct Interface* iface)
{
    if (!receivedMessageTUNCount) {
        return 0;
    }

    // Got the message, test successful, tear everything down by freeing the root alloc.
    struct Allocator* alloc = iface->receiverContext;
    Allocator_free(alloc);

    return 0;
}

static void fail(void* ignored)
{
    Assert_true(!"timeout");
}

int main(int argc, char** argv)
{
    // TODO(cjd): fix TUNConfigurator_addIp4Address() for Illumos, Darwin, BSD.
    #if defined(sunos) || defined(darwin) || defined(freebsd)
        return 0;
    #endif

    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(testAddrA, Sockaddr_AF_INET, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    struct Interface* tun = TUNInterface_new(NULL, assignedIfName, base, logger, NULL, alloc);
    NetDev_addAddress(assignedIfName, addrA, 30, logger, NULL);

    struct Sockaddr_storage ss;
    Assert_true(!Sockaddr_parse("0.0.0.0", &ss));
    struct AddrInterface* udp = UDPAddrInterface_new(base, &ss.addr, alloc, NULL, logger);

    struct Sockaddr* dest = Sockaddr_clone(udp->addr, alloc);
    uint8_t* addr;
    Assert_true(4 == Sockaddr_getAddress(dest, &addr));
    Bits_memcpy(addr, testAddrB, 4);

    struct Message* msg;
    Message_STACK(msg, 0, 64);
    Message_push(msg, "Hello World", 12, NULL);
    Message_push(msg, dest, dest->addrLen, NULL);

    udp->generic.receiveMessage = receiveMessageUDP;
    udp->generic.receiverContext = alloc;
    tun->receiveMessage = receiveMessageTUN;

    udp->generic.sendMessage(msg, &udp->generic);

    Timeout_setTimeout(fail, NULL, 1000, base, alloc);

    EventBase_beginLoop(base);
    return 0;
}
