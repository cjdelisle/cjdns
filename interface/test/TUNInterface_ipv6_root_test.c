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
#define string_strcmp
#define string_strlen
#include "admin/testframework/AdminTestFramework.h"
#include "admin/Admin.h"
#include "admin/AdminClient.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "benc/Int.h"
#include "interface/UDPInterface_pvt.h"
#include "interface/TUNInterface.h"
#include "interface/TUNMessageType.h"
#include "interface/TUNConfigurator.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "interface/InterfaceController.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/platform/libc/string.h"
#include "util/Timeout.h"
#include "wire/Ethernet.h"
#include "wire/Headers.h"

#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

const uint8_t testAddrA[] = {0xfd,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1};
const uint8_t testAddrB[] = {0xfd,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2};

/*
 * Setup a UDPInterface and a TUNInterface, test sending traffic between them.
 */

static int registerPeer(struct InterfaceController* ic,
                        uint8_t herPublicKey[32],
                        String* password,
                        bool requireAuth,
                        struct Interface* iface)
{
    return 0;
}

static int receivedMessageTUNCount = 0;
static uint8_t receiveMessageTUN(struct Message* msg, struct Interface* iface)
{
    receivedMessageTUNCount++;
    uint16_t ethertype = TUNMessageType_pop(msg);
    if (ethertype != Ethernet_TYPE_IP6) {
        printf("Spurious packet with ethertype [%u]\n", Endian_bigEndianToHost16(ethertype));
        return 0;
    }

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;

    if (msg->length != Headers_IP6Header_SIZE + Headers_UDPHeader_SIZE + 12) {
        int type = (msg->length >= Headers_IP6Header_SIZE) ? header->nextHeader : -1;
        printf("Message of unexpected length [%u] ip6->nextHeader: [%d]\n", msg->length, type);
        return 0;
    }

    Assert_always(!Bits_memcmp(header->destinationAddr, testAddrB, 16));
    Assert_always(!Bits_memcmp(header->sourceAddr, testAddrA, 16));

    Bits_memcpyConst(header->destinationAddr, testAddrA, 16);
    Bits_memcpyConst(header->sourceAddr, testAddrB, 16);

    TUNMessageType_push(msg, ethertype);

    return iface->sendMessage(msg, iface);
}

static uint8_t receiveMessageUDP(struct Message* msg, struct Interface* iface)
{
    if (!receivedMessageTUNCount) {
        return 0;
    }
    // Got the message, test successful.
    exit(0);
    return 0;
}

static void fail(void* ignored)
{
    Assert_true(!"timeout");
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<20), NULL);
    struct EventBase* base = EventBase_new(alloc);
    struct Writer* logWriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logWriter, alloc);

    // mock interface controller.
    struct InterfaceController ic = {
        .registerPeer = registerPeer
    };

    char assignedInterfaceName[TUNConfigurator_IFNAMSIZ];
    void* tunPtr = TUNConfigurator_initTun(NULL, assignedInterfaceName, logger, NULL);
    TUNConfigurator_addIp6Address(assignedInterfaceName, testAddrA, 126, logger, NULL);
    struct TUNInterface* tun = TUNInterface_new(tunPtr, base, alloc, logger);

    #if defined(OSX) || defined(BSD)
        // Mac OSX and BSD do not set up their TUN devices synchronously.
        // There should be a proper way to do this but I'm too lazy to look for it atm.
        usleep(10 * 1000);
    #endif

    struct UDPInterface* udp = UDPInterface_new(base, "[fd00::1]", alloc, NULL, logger, &ic);

    struct sockaddr_in6 sin = { .sin6_family = AF_INET6 };
    sin.sin6_port = udp->boundPort_be;
    Bits_memcpy(&sin.sin6_addr, testAddrB, 16);

    struct Message* msg;
    Message_STACK(msg, 0, 64);
    Message_push(msg, "Hello World", 12);
    Message_push(msg, &sin, sizeof(struct sockaddr_in6));

    udp->generic.receiveMessage = receiveMessageUDP;
    tun->iface.receiveMessage = receiveMessageTUN;

    udp->generic.sendMessage(msg, &udp->generic);

    Timeout_setTimeout(fail, NULL, 1000, base, alloc);

    EventBase_beginLoop(base);
}
