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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto/random/Random.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "subnode/LinkState.h"
#include "util/Assert.h"
#include "wire/Message.h"
#include "util/Hex.h"

#define CYCLES 10

static uint64_t randVarInt(struct Random* rand)
{
    uint64_t val = Random_uint64(rand);
    if ((val & 0xff) < 192) {
        return (val >> 8) & 0xff;
    }
    if ((val & 0xff) < 224) {
        return (val >> 8) & 0xffff;
    }
    if ((val & 0xff) < 240) {
        return (val >> 8) & 0xffffffff;
    }
    return Random_uint64(rand);
}

static void randomLs(struct Random* rand, struct LinkState* ls, bool total)
{
    if (total) {
        ls->samples = Random_uint32(rand);
        ls->nodeId = Random_uint16(rand);
    }
    for (int i = 0; i < LinkState_SLOTS; i++) {
        ls->lagSlots[i] = Random_uint16(rand); //randVarInt(rand);
        ls->dropSlots[i] = Random_uint16(rand); // randVarInt(rand);
        ls->kbRecvSlots[i] = Random_uint32(rand); //randVarInt(rand);
    }
}

static void randomLsUpdate(struct Random* rand, struct LinkState* ls)
{
    uint8_t toUpdate = Random_uint8(rand) % 20;
    if (!toUpdate) { return; }
    //printf(">>%d\n", toUpdate);
    int samplesNext = ls->samples;
    if (toUpdate >= LinkState_SLOTS) {
        samplesNext += LinkState_SLOTS;
        randomLs(rand, ls, false);
    } else {
        samplesNext += toUpdate;
        int i = ls->samples % LinkState_SLOTS;
        do {
            ls->lagSlots[i] = randVarInt(rand);
            ls->dropSlots[i] = randVarInt(rand);
            ls->kbRecvSlots[i] = randVarInt(rand);
            i = (i + 1) % LinkState_SLOTS;
        } while (i != samplesNext % LinkState_SLOTS);
    }
    Assert_true(samplesNext - ls->samples <= LinkState_SLOTS);
    ls->samples = samplesNext;
}

static void applyStateUpdates(struct LinkState* local, Message_t* msg)
{
    if (!Message_getLength(msg)) { return; }
    uint8_t length = Message_bytes(msg)[0];
    struct VarInt_Iter it;
    Assert_true(!LinkState_mkDecoder(msg, &it));
    uint32_t id = 0;
    Assert_true(!LinkState_getNodeId(&it, &id));
    Assert_true(id == local->nodeId);
    Err_assert(Message_eshift(msg, -length));
    applyStateUpdates(local, msg);
    Assert_true(!LinkState_decode(&it, local));
}

static void assertSame(struct LinkState* beforeState,
                       struct LinkState* updated,
                       Message_t* update)
{
    struct LinkState local;
    Bits_memcpy(&local, beforeState, sizeof(struct LinkState));
    //printf("%02x %02x\n", local.nodeId, local.samples);
    applyStateUpdates(&local, update);

    #if 0
    printf("%02x %02x\n", updated->nodeId, updated->samples);
    printf("%02x %02x\n", local.nodeId, local.samples);
    for (int i = 0; i < LinkState_SLOTS; i++) {
        printf("%02x %02x %02x\n",
            updated->lagSlots[i], updated->dropSlots[i], updated->kbRecvSlots[i]);
        printf("%02x %02x %02x\n\n",
            local.lagSlots[i], local.dropSlots[i], local.kbRecvSlots[i]);
    }
    printf("\n");
    #endif

    Assert_true(!Bits_memcmp(&local, updated, sizeof(struct LinkState)));
}

static void testStatic(Allocator_t* alloc)
{
    uint8_t* hex =
        "\x20\x03\x06\x00\x00\x00\x00\x00\x00"
        "\x04"  "\x10"
        "\x13\x00\x01"
        "\x12\x00\x02"
        "\x13\x00\x02"
        "\x13\x00\x00"
        "\x14\x00\x03"
        "\x12\x00\x01"
        "\x13\x00\x01";
    struct LinkState ls = { .nodeId = 0 };
    struct VarInt_Iter it = { .start = 0 };
    Message_t* msg = Message_new(0, 512, alloc);
    Err_assert(Message_epush(msg, hex, 32));
    Assert_true(!LinkState_mkDecoder(msg, &it));
    Assert_true(!LinkState_decode(&it, &ls));
    // printf("%u %u\n", ls.nodeId, ls.samples);
    // for (int i = 0; i < LinkState_SLOTS; i++) {
    //     printf("%u %u %u\n", ls.lagSlots[i], ls.dropSlots[i], ls.kbRecvSlots[i]);
    // }

    struct LinkState gold = {
        .nodeId = 4,
        .samples = 7,
        .lagSlots = { 19, 19, 20, 18, 19, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 19, 18 },
        .dropSlots = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
        .kbRecvSlots = { 2, 0, 3, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2 },
    };
    Assert_true(!Bits_memcmp(&ls, &gold, sizeof(struct LinkState)));
}

int main(int argc, char* argv[])
{
    struct Allocator* mainAlloc = Allocator_new(1<<18);
    testStatic(mainAlloc);
    struct Random* rand = NULL;
    Err_assert(Random_new(&rand, mainAlloc, NULL));

    int cycles = CYCLES;
    Message_t* msg = Message_new(0, 2048, mainAlloc);
    for (int cycle = 0; cycle < cycles; cycle++) {

        uint16_t nodeId = Random_uint16(rand);
        struct LinkState ls0 = { .nodeId = nodeId };
        struct LinkState ls = { .nodeId = nodeId, .samples = LinkState_SLOTS };
        randomLs(rand, &ls, false);

        Assert_true(!Message_getLength(msg));
        Assert_true(!LinkState_encode(msg, &ls, 0));
        assertSame(&ls0, &ls, msg);

        for (int cc = 0; cc < 100; cc++) {
            Bits_memcpy(&ls0, &ls, sizeof(struct LinkState));
            randomLsUpdate(rand, &ls);
            Assert_true(!LinkState_encode(msg, &ls, ls0.samples));
            //printf("L>%d\n", Message_getLength(msg));
            //for (int i = 0; i < Message_getLength(msg); i++) { printf("%02x", msg->bytes[i]); }
            //printf("\n");
            assertSame(&ls0, &ls, msg);
        }

        //printf("Test %d\n", Message_getLength(msg));
        //for (int i = 0; i < Message_getLength(msg); i++) { printf("%02x", msg->bytes[i]); }
        //printf("\n");
    }
    Allocator_free(mainAlloc);
    return 0;
}
