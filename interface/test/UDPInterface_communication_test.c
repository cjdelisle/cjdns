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
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "net/InterfaceController.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/platform/libc/string.h"
#include "util/Timeout.h"

#include <event2/event.h>

/*
 * Setup 2 UDPInterface's, test sending traffic between them.
 */

static void registerInterface(struct Interface* externalInterface,
                              struct InterfaceController* ic)
{
}

static uint8_t receiveMessageA(struct Message* msg, struct Interface* iface)
{
    // pong
    return iface->sendMessage(msg, iface);
}

static uint8_t receiveMessageB(struct Message* msg, struct Interface* iface)
{
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
        .registerInterface = registerInterface
    };

    struct UDPInterface* udpA = UDPInterface_new(base, "127.0.0.1", alloc, NULL, logger, &ic);
    struct UDPInterface* udpB = UDPInterface_new(base, "127.0.0.1", alloc, NULL, logger, &ic);

    uint8_t key[8] = {0};
    uint8_t length = sizeof(struct sockaddr_in);
    length = (length > 8) ? 8 : length;
    Bits_memcpy(key, &((struct UDPInterface_pvt*) udpA)->addr, length);

    struct Message* msg;
    Message_STACK(msg, 12, 64);
    Bits_memcpyConst((char*)msg->bytes, "Hello World", 12);
    Message_shift(msg, 8);
    Bits_memcpyConst((char*)msg->bytes, key, 8);

    udpA->generic.receiveMessage = receiveMessageA;
    udpB->generic.receiveMessage = receiveMessageB;

    udpB->generic.sendMessage(msg, &udpB->generic);

    Timeout_setTimeout(fail, NULL, 1000, base, alloc);

    EventBase_beginLoop(base);
}
