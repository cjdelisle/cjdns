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
#include "interface/tuntap/NDPServer.h"
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
#include "interface/tuntap/test/TUNTools.h"
#include "interface/tuntap/TAPWrapper.h"

#include <unistd.h>
#include <stdlib.h>

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

    if (Bits_memcmp(header->destinationAddr, testAddrB, 16)) { return 0; }
    if (Bits_memcmp(header->sourceAddr, testAddrA, 16)) { return 0; }

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

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<20);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* log = WriterLog_new(logWriter, alloc);

    struct Sockaddr* addrA = Sockaddr_fromBytes(testAddrA, Sockaddr_AF_INET6, alloc);

    char assignedIfName[TUNInterface_IFNAMSIZ];
    struct Interface* tap = TUNInterface_new(NULL, assignedIfName, 1, base, log, NULL, alloc);
    struct TAPWrapper* tapWrapper = TAPWrapper_new(tap, log, alloc);

    // Now setup the NDP server so the tun will work correctly.
    struct NDPServer* ndp = NDPServer_new(&tapWrapper->generic, log, TAPWrapper_LOCAL_MAC, alloc);
    ndp->advertisePrefix[0] = 0xfd;
    ndp->prefixLen = 8;

    struct Interface* tun = &ndp->generic;

    NetDev_addAddress(assignedIfName, addrA, 126, log, NULL);

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("[::]", &addr));

    struct AddrInterface* udp = TUNTools_setupUDP(base, &addr.addr, alloc, log);

    struct Sockaddr* dest = Sockaddr_clone(udp->addr, alloc);
    uint8_t* addrBytes;
    Assert_true(16 == Sockaddr_getAddress(dest, &addrBytes));
    Bits_memcpy(addrBytes, testAddrB, 16);

    udp->generic.receiveMessage = receiveMessageUDP;
    udp->generic.receiverContext = alloc;
    tun->receiveMessage = receiveMessageTUN;

    TUNTools_sendHelloWorld(udp, dest, base, alloc);
    Timeout_setTimeout(fail, NULL, 10000000, base, alloc);

    EventBase_beginLoop(base);
    return 0;
}
