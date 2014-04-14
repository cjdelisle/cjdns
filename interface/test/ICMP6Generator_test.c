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

#include "memory/Allocator.h"
#include "memory/MallocAllocator.h"
#include "crypto/random/Random.h"
#include "interface/ICMP6Generator_pvt.h"
#include "wire/Headers.h"
#include "util/Assert.h"
#include "util/Bits.h"

#include <stdio.h>

#define MIN(a, b)  (((a) < (b)) ? (a) : (b))

static struct Message* newMessage(struct Allocator* alloc, int messageSize)
{
    uint8_t* buff = Allocator_calloc(alloc, messageSize + 64, 1);
    return Allocator_clone(alloc, (&(struct Message) {
        .bytes = buff + 64,
        .length = messageSize,
        .padding = 64
    }));
}

static void mtuTest(struct Allocator* mainAlloc, struct Random* rand, int messageSize, uint32_t mtu)
{
    struct Allocator* alloc = Allocator_child(mainAlloc);
    struct Message* msg = newMessage(alloc, messageSize);

    uint8_t* sourceAddr = (uint8_t*) "sourceAddress123";
    uint8_t* destAddr = (uint8_t*) "destinationAddr1";

    // Fill it with predictable data.
    for (int i = 0; i < messageSize; i++) {
        msg->bytes[i] = i & 0xff;
    }

    ICMP6Generator_generate(msg, sourceAddr, destAddr, ICMP6Generator_Type_PACKET_TOO_BIG, mtu);


    Assert_true(msg->length <= 1280);

    struct Headers_IP6Header* header = (struct Headers_IP6Header*) msg->bytes;
    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);

    Assert_true(!Bits_memcmp(sourceAddr, header->sourceAddr, 16));
    Assert_true(!Bits_memcmp(destAddr, header->destinationAddr, 16));
    Assert_true(Headers_getIpVersion(header) == 6);
    Assert_true(header->flowLabelLow_be == 0);
    Assert_true(Endian_bigEndianToHost16(header->payloadLength_be) == msg->length);
    Assert_true(header->nextHeader == 58); // 58 -> icmp
    Assert_true(header->hopLimit == 64);

    struct Headers_ICMP6Header* icmp = (struct Headers_ICMP6Header*) msg->bytes;
    Message_shift(msg, -Headers_ICMP6Header_SIZE, NULL);

    Assert_true(icmp->type == 2); // packet too big.
    Assert_true(icmp->code == 0);
    Assert_true(icmp->additional == Endian_hostToBigEndian32(mtu));

    Assert_true(msg->length ==
        MIN(messageSize,
            ICMP6Generator_MIN_IPV6_MTU
            - Headers_IP6Header_SIZE
            - Headers_ICMP6Header_SIZE));

    uint32_t out = 0;
    for (int i = 0; i < msg->length; i++) {
        out |= msg->bytes[i] ^ (i & 0xff);
    }
    Assert_true(!out);

    Allocator_free(alloc);
}

static uint8_t messageFromGenerator(struct Message* msg, struct Interface* iface)
{
    uint64_t* reassemblyBuff = iface->receiverContext;
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;
    struct Headers_IP6Fragment* frag = (struct Headers_IP6Fragment*) (&ip6[1]);

    int index = Headers_IP6Fragment_getOffset(frag);

    Message_shift(msg, -Headers_IP6Header_SIZE, NULL);
    Assert_true(Endian_bigEndianToHost16(ip6->payloadLength_be) == msg->length);
    Message_shift(msg, -Headers_IP6Fragment_SIZE, NULL);
    Assert_true(msg->length > 0);

    Bits_memcpy(&reassemblyBuff[index], msg->bytes, msg->length);
    Message_shift(msg, (Headers_IP6Header_SIZE + Headers_IP6Fragment_SIZE), NULL);

    printf("Got message fragment with index [%d] length [%d] hasMoreFragments [%d]\n",
           index, msg->length, Headers_IP6Fragment_hasMoreFragments(frag));
    return 0;
}

static void fragTest(struct Allocator* mainAlloc,
                     struct Random* rand,
                     int messageSize,
                     uint32_t mtu)
{
    struct Allocator* alloc = Allocator_child(mainAlloc);
    struct Message* msg = newMessage(alloc, messageSize);
    for (int i = 0; i < msg->length; i++) {
        msg->bytes[i] = i & 0xff;
    }
    struct Headers_IP6Header* ip6 = (struct Headers_IP6Header*) msg->bytes;
    Headers_setIpVersion(ip6);
    ip6->payloadLength_be = Endian_hostToBigEndian16(messageSize - Headers_IP6Header_SIZE);
    struct Headers_IP6Fragment* frag = (struct Headers_IP6Fragment*) (&ip6[1]);
    ip6->nextHeader = 44;
    Bits_memset(frag, 0, sizeof(frag));

    const uint32_t headersSize = Headers_IP6Header_SIZE + Headers_IP6Fragment_SIZE;
    const uint32_t reassemblySize = ((messageSize + 7 - headersSize) / 8) * 8;
    uint8_t* reassemblyBuff = Allocator_calloc(alloc, reassemblySize, 1);
    printf("Allocating [%d] bytes for reassembly.\n", reassemblySize);
    struct ICMP6Generator* ig = ICMP6Generator_new(alloc);
    ((struct ICMP6Generator_pvt*)ig)->mtu = mtu;
    ig->internal.receiveMessage = messageFromGenerator;
    ig->internal.receiverContext = reassemblyBuff;

    ig->external.sendMessage(msg, &ig->external);

    for (int i = 0; i < (int)(messageSize - headersSize); i++) {
        Assert_true(reassemblyBuff[i] == ((i + headersSize) & 0xff));
    }

    Allocator_free(alloc);
}

int main()
{
    struct Allocator* alloc = MallocAllocator_new(20000);
    struct Random* rand = Random_new(alloc, NULL, NULL);
    mtuTest(alloc, rand, 2048, 1500);
    mtuTest(alloc, rand, 1500, 1492);
    mtuTest(alloc, rand, 1492, 1280);
    mtuTest(alloc, rand, 1280, 1024);
    mtuTest(alloc, rand, 1492, 1024);
    mtuTest(alloc, rand, 1024, 512);

    fragTest(alloc, rand, 2048, 1000);
    fragTest(alloc, rand, 1400, 1000);
    fragTest(alloc, rand, 1300, 500);
    fragTest(alloc, rand, 1500, 200);
    fragTest(alloc, rand, 1500, 100);

    Allocator_free(alloc);
    return 0;
}
