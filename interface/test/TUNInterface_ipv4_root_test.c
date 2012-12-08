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

#if defined(BSD) || defined(Illumos)
    #include <netinet/in.h>
    #include <sys/socket.h>
#endif

// On loan from the DoD, thanks guys.
const uint8_t testAddrA[4] = {11, 0, 0, 1};
const uint8_t testAddrB[4] = {11, 0, 0, 2};

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
    uint16_t ethertype = TUNInterface_popMessageType(msg);
    Assert_always(ethertype == Ethernet_TYPE_IP4);

    struct Headers_IP4Header* header = (struct Headers_IP4Header*) msg->bytes;

    Assert_always(msg->length == Headers_IP4Header_SIZE + Headers_UDPHeader_SIZE + 12);

    Assert_always(!Bits_memcmp(header->destAddr, testAddrB, 4));
    Assert_always(!Bits_memcmp(header->sourceAddr, testAddrA, 4));

    Bits_memcpyConst(header->destAddr, testAddrA, 4);
    Bits_memcpyConst(header->sourceAddr, testAddrB, 4);

    TUNInterface_pushMessageType(msg, ethertype);

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
    TUNConfigurator_addIp4Address(assignedInterfaceName, testAddrA, 30, logger, NULL);
    struct TUNInterface* tun = TUNInterface_new(tunPtr, base, alloc);

    struct UDPInterface* udp = UDPInterface_new(base, "11.0.0.1:5000", alloc, NULL, logger, &ic);

    struct sockaddr_in sin = { .sin_family = AF_INET };
    sin.sin_port = Endian_hostToBigEndian16(5000);
    Bits_memcpy(&sin.sin_addr, testAddrB, 4);

    struct Message* msg;
    Message_STACK(msg, 0, 64);
    Message_push(msg, "Hello World", 12);
    Message_push(msg, &sin, sizeof(struct sockaddr_in));

    udp->generic.receiveMessage = receiveMessageUDP;
    tun->iface.receiveMessage = receiveMessageTUN;

    udp->generic.sendMessage(msg, &udp->generic);

    Timeout_setTimeout(fail, NULL, 1000, base, alloc);

    EventBase_beginLoop(base);
}
