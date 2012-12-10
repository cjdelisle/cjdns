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
#define string_strncpy
#define string_strlen
#include "crypto/Key.h"
#include "memory/MallocAllocator.h"
#include "memory/CanaryAllocator.h"
#include "memory/Allocator.h"
#include "util/Base32.h"
#include "util/Checksum.h"
#include "util/platform/libc/string.h"
#include "test/TestFramework.h"
#include "net/Ducttape_pvt.h"
#include "util/log/WriterLog.h"
#include "io/FileWriter.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "interface/TUNMessageType.h"

#include <stdio.h>

#define TUNB 2
#define TUNA 1
uint8_t incomingTunB(struct Message* msg, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(msg) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE);
    printf("Message from TUN in node B [%s]\n", msg->bytes);
    *((int*)iface->senderContext) = TUNB;
    return 0;
}
uint8_t incomingTunA(struct Message* msg, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(msg) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE);
    printf("Message from TUN in node A [%s]\n", msg->bytes);
    *((int*)iface->senderContext) = TUNA;
    return 0;
}

struct TwoNodes
{
    struct Interface tunIfB;
    struct TestFramework* nodeB;

    struct Interface tunIfA;
    struct TestFramework* nodeA;
    int messageFrom;
};

static struct TwoNodes* setUp(struct Allocator* alloc)
{
    struct Writer* logwriter = FileWriter_new(stdout, alloc);
    struct Log* logger = WriterLog_new(logwriter, alloc);

    struct TestFramework* a =
        TestFramework_setUp("\xad\x7e\xa3\x26\xaa\x01\x94\x0a\x25\xbc\x9e\x01\x26\x22\xdb\x69"
                            "\x4f\xd9\xb4\x17\x7c\xf3\xf8\x91\x16\xf3\xcf\xe8\x5c\x80\xe1\x4a",
                            alloc, logger);
    //"publicKey": "kmzm4w0kj9bswd5qmx74nu7kusv5pj40vcsmp781j6xxgpd59z00.k",
    //"ipv6": "fc41:94b5:0925:7ba9:3959:11ab:a006:367a",


    struct TestFramework* b =
        TestFramework_setUp("\x50\xa0\x9c\xaf\xc3\xce\xee\xa9\x26\xce\x41\x7a\x14\xbc\x3d\x2f"
                            "\xb9\xc0\xc9\xde\x59\x77\x6f\x78\x0a\x4f\xc9\x37\x56\xe7\x19\xa4",
                            alloc, logger);
    //"publicKey": "9u9crb3buyk4dbvfs8f6z68n9s9q4tj3qnpnvnrzxcl994h54js0.k",
    //"ipv6": "fc20:1b0b:e296:df67:ee93:2fd9:e36d:12d9",

    TestFramework_linkNodes(b, a);

    struct TwoNodes* out = Allocator_malloc(alloc, sizeof(struct TwoNodes));

    Bits_memcpyConst(out, (&(struct TwoNodes) {
        .tunIfB = {
            .allocator = alloc,
            .sendMessage = incomingTunB,
            .senderContext = &out->messageFrom
        },
        .nodeB = b,
        .tunIfA = {
            .allocator = alloc,
            .sendMessage = incomingTunA,
            .senderContext = &out->messageFrom
        },
        .nodeA = a
    }), sizeof(struct TwoNodes));

    Ducttape_setUserInterface(b->ducttape, &out->tunIfB);
    Ducttape_setUserInterface(a->ducttape, &out->tunIfA);

    return out;
}

void sendMessage(struct TwoNodes* tn, char* message, bool bToA)
{
    struct Message* msg;
    Message_STACK(msg, 64, 512);

    Bits_memcpy(msg->bytes, message, strlen(message) + 1);
    msg->length = strlen(message) + 1;

    if (bToA) {
        TestFramework_craftIPHeader(msg, tn->nodeB->ip, tn->nodeA->ip);
        TUNMessageType_push(msg, Ethernet_TYPE_IP6);
        tn->tunIfB.receiveMessage(msg, &tn->tunIfB);
    } else {
        TestFramework_craftIPHeader(msg, tn->nodeA->ip, tn->nodeB->ip);
        TUNMessageType_push(msg, Ethernet_TYPE_IP6);
        tn->tunIfA.receiveMessage(msg, &tn->tunIfA);
    }

    Assert_always(tn->messageFrom == ((bToA) ? TUNA : TUNB));
    tn->messageFrom = 0;
}

int main()
{
    struct Allocator* alloc = CanaryAllocator_new(MallocAllocator_new(1<<22), NULL);
    struct TwoNodes* tn = setUp(alloc);

    sendMessage(tn, "Hello World!", true);
    sendMessage(tn, "Hello cjdns!", false);
    sendMessage(tn, "hi", true);
    sendMessage(tn, "this is a test", false);
    sendMessage(tn, "ok bye now", true);
    sendMessage(tn, "byebye", false);

    alloc->free(alloc);
}
