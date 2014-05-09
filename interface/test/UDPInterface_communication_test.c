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
#include "interface/UDPInterface_pvt.h"
#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "interface/InterfaceController.h"
#include "io/FileWriter.h"
#include "io/Writer.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "util/log/WriterLog.h"
#include "util/events/Timeout.h"

#include <stdlib.h>

/*
 * Setup 2 UDPInterface's, test sending traffic between them.
 */

static int registerPeer(struct InterfaceController* ic,
                        uint8_t herPublicKey[32],
                        String* password,
                        bool requireAuth,
                        bool transient,
                        struct Interface* iface)
{
    return 0;
}

int receiveMessageACount = 0;
static uint8_t receiveMessageA(struct Message* msg, struct Interface* iface)
{
    // pong
    receiveMessageACount++;
    return iface->sendMessage(msg, iface);
}

static uint8_t receiveMessageB(struct Message* msg, struct Interface* iface)
{
    if (receiveMessageACount) {
        // Got the message, test successful.
        struct Allocator* alloc = iface->receiverContext;
        Allocator_free(alloc);
    }
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
    struct Log* logger = WriterLog_new(logWriter, alloc);

    // mock interface controller.
    struct InterfaceController ic = {
        .registerPeer = registerPeer
    };

    struct Sockaddr_storage addr;
    Assert_true(!Sockaddr_parse("127.0.0.1", &addr));

    struct UDPInterface* udpA = UDPInterface_new(base, &addr.addr, alloc, NULL, logger, &ic);
    struct UDPInterface* udpB = UDPInterface_new(base, &addr.addr, alloc, NULL, logger, &ic);

    struct Message* msg;
    Message_STACK(msg, 0, 128);

    Message_push(msg, "Hello World", 12, NULL);
    Message_push(msg, udpA->addr, udpA->addr->addrLen, NULL);

    struct Interface* ifA = &((struct UDPInterface_pvt*) udpA)->udpBase->generic;
    struct Interface* ifB = &((struct UDPInterface_pvt*) udpB)->udpBase->generic;

    ifA->receiveMessage = receiveMessageA;
    ifB->receiveMessage = receiveMessageB;
    ifB->receiverContext = alloc;

    struct Allocator* child = Allocator_child(alloc);
    msg = Message_clone(msg, child);
    ifB->sendMessage(msg, ifB);
    Allocator_free(child);

    Timeout_setTimeout(fail, NULL, 1000, base, alloc);

    EventBase_beginLoop(base);
    return 0;
}
