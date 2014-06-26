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
#include "crypto/Key.h"
#include "io/FileWriter.h"
#include "memory/MallocAllocator.h"
#include "memory/Allocator.h"
#include "util/Base32.h"
#include "util/Checksum.h"
#include "util/log/WriterLog.h"
#include "test/TestFramework.h"
#include "net/Ducttape_pvt.h"
#include "wire/Headers.h"
#include "wire/Ethernet.h"
#include "interface/tuntap/TUNMessageType.h"

#include <stdio.h>

#define TUNC 3
#define TUNB 2
#define TUNA 1
static uint8_t incomingTunC(struct Message* msg, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(msg, NULL) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    printf("Message from TUN in node C [%s] [%d]\n", msg->bytes, msg->length);
    *((int*)iface->senderContext) = TUNC;
    return 0;
}

static uint8_t incomingTunB(struct Message* msg, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(msg, NULL) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    printf("Message from TUN in node B [%s]\n", msg->bytes);
    *((int*)iface->senderContext) = TUNB;
    return 0;
}

static uint8_t incomingTunA(struct Message* msg, struct Interface* iface)
{
    Assert_true(TUNMessageType_pop(msg, NULL) == Ethernet_TYPE_IP6);
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    uint8_t buff[1024];
    Hex_encode(buff, 1024, msg->bytes, msg->length);
    printf("Message from TUN in node A [%s] [%d] [%s]\n", msg->bytes, msg->length, buff);
    *((int*)iface->senderContext) = TUNA;
    return 0;
}

struct ThreeNodes
{
    struct Interface tunIfC;
    struct TestFramework* nodeC;

    struct Interface tunIfB;
    struct TestFramework* nodeB;

    struct Interface tunIfA;
    struct TestFramework* nodeA;
    int messageFrom;
};

static struct ThreeNodes* setUp(struct Allocator* alloc)
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
        TestFramework_setUp("\xea\x8d\x34\x04\xa9\x7c\xe4\xf9\xca\x7e\x24\xe6\xf1\x85\xb9\x3f"
                            "\x01\x37\xb7\xa1\xf5\x2c\xce\xc0\x2c\xae\x03\xf1\x83\x38\x13\x24",
                            alloc, logger);
    // This address was found by brute force for one which falls between A and C without being
    // closer in either direction (XOR is bidirectional address space distance)
    // ipv6: fc2e:3273:644e:426f:283d:e3c7:c87c:41c1

    struct TestFramework* c =
        TestFramework_setUp("\xd8\x54\x3e\x70\xb9\xae\x7c\x41\xbc\x18\xa4\x9a\x9c\xee\xca\x9c"
                            "\xdc\x45\x01\x96\x6b\xbd\x7e\x76\xcf\x3a\x9f\xbc\x12\xed\x8b\xb4",
                            alloc, logger);
    //"publicKey": "vz21tg07061s8v9mckrvgtfds7j2u5lst8cwl6nqhp81njrh5wg0.k",
    //"ipv6": "fc1f:5b96:e1c5:625d:afde:2523:a7fa:383a",

    Log_debug(a->logger, "Linking A and B");
    TestFramework_linkNodes(b, a);
    Log_debug(a->logger, "Linking A and B complete");
    Log_debug(a->logger, "Linking B and C");
    TestFramework_linkNodes(c, b);
    Log_debug(a->logger, "Linking B and C complete");

    struct ThreeNodes* out = Allocator_malloc(alloc, sizeof(struct ThreeNodes));

    Bits_memcpyConst(out, (&(struct ThreeNodes) {
        .tunIfC = {
            .allocator = alloc,
            .sendMessage = incomingTunC,
            .senderContext = &out->messageFrom
        },
        .nodeC = c,
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
    }), sizeof(struct ThreeNodes));

    Ducttape_setUserInterface(c->ducttape, &out->tunIfC);
    Ducttape_setUserInterface(b->ducttape, &out->tunIfB);
    Ducttape_setUserInterface(a->ducttape, &out->tunIfA);

    Log_debug(a->logger, "Setup complete.");

    return out;
}

static void sendMessage(struct ThreeNodes* tn,
                        char* message,
                        struct TestFramework* from,
                        struct TestFramework* to)
{
    struct Message* msg;
    Message_STACK(msg, 64, 512);

    Bits_memcpy(msg->bytes, message, CString_strlen(message) + 1);
    msg->length = CString_strlen(message) + 1;

    TestFramework_craftIPHeader(msg, from->ip, to->ip);

    msg = Message_clone(msg, from->alloc);

    struct Interface* fromIf;

    if (from == tn->nodeA) {
        fromIf = &tn->tunIfA;
    } else if (from == tn->nodeB) {
        fromIf = &tn->tunIfB;
    } else if (from == tn->nodeC) {
        fromIf = &tn->tunIfC;
    } else {
        Assert_true(false);
    }

    TUNMessageType_push(msg, Ethernet_TYPE_IP6, NULL);
    fromIf->receiveMessage(msg, fromIf);

    if (to == tn->nodeA) {
        Assert_true(tn->messageFrom == TUNA);
    } else if (to == tn->nodeB) {
        Assert_true(tn->messageFrom == TUNB);
    } else if (to == tn->nodeC) {
        Assert_true(tn->messageFrom == TUNC);
    } else {
        Assert_true(false);
    }

    TestFramework_assertLastMessageUnaltered(tn->nodeA);
    TestFramework_assertLastMessageUnaltered(tn->nodeB);
    TestFramework_assertLastMessageUnaltered(tn->nodeC);

    tn->messageFrom = 0;
}

/** Check if nodes A and C can communicate via B without A knowing that C exists. */
int main()
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);

    struct ThreeNodes* tn = setUp(alloc);

    sendMessage(tn, "Hello World!", tn->nodeA, tn->nodeC);
    sendMessage(tn, "Hello cjdns!", tn->nodeC, tn->nodeA);
    sendMessage(tn, "send", tn->nodeA, tn->nodeC);
    sendMessage(tn, "a", tn->nodeC, tn->nodeA);
    sendMessage(tn, "few", tn->nodeA, tn->nodeC);
    sendMessage(tn, "packets", tn->nodeC, tn->nodeA);
    sendMessage(tn, "to", tn->nodeA, tn->nodeC);
    sendMessage(tn, "make", tn->nodeC, tn->nodeA);
    sendMessage(tn, "sure", tn->nodeA, tn->nodeC);
    sendMessage(tn, "the", tn->nodeC, tn->nodeA);
    sendMessage(tn, "cryptoauth", tn->nodeA, tn->nodeC);
    sendMessage(tn, "can", tn->nodeC, tn->nodeA);
    sendMessage(tn, "establish", tn->nodeA, tn->nodeC);

    Allocator_free(alloc);
    return 0;
}
