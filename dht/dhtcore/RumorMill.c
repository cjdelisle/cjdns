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
#include "dht/dhtcore/RumorMill.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/Assert.h"
#include "util/Bits.h"

/**
 * The rumor mill is for new nodes which have been discovered by search and getPeers requests
 * but we have never actually communicated with them so we are not sure if they exist.
 *
 * More importantly, we *cannot* link them into the nodeStore tree because we are not sure of
 * their encoding scheme.
 */
struct RumorMill_pvt
{
    struct RumorMill pub;

    struct Address* selfAddr;

    struct Address* addresses;
    int count;
    int capacity;

    Identity
};

static inline bool hasNode(struct RumorMill* mill, struct Address* addr)
{
    struct RumorMill_pvt* rm = Identity_check((struct RumorMill_pvt*) mill);
    for (int i = 0; i < rm->pub.count; i++) {
        if (rm->addresses[i].path == addr->path ||
            !Bits_memcmp(&rm->addresses[i], addr->ip6.bytes, Address_SEARCH_TARGET_SIZE)) {
            return true;
            }
    }
    return false;
}

static int getBadness(struct Address* badAddr, struct Address* selfAddr)
{
    uint64_t xor = Endian_bigEndianToHost64(badAddr->ip6.longs.one_be ^ selfAddr->ip6.longs.one_be);
    return Bits_log2x64(xor) + Bits_log2x64(badAddr->path);
}

static struct Address* getWorst(struct RumorMill_pvt* rm)
{
    struct Address* worst = NULL;
    int howBadIsTheWorst = 0;
    for (int i = 0; i < rm->pub.count; i++) {
        int howBad = getBadness(&rm->addresses[i], rm->selfAddr);
        if (howBad > howBadIsTheWorst) {
            howBadIsTheWorst = howBad;
            worst = &rm->addresses[i];
        }
    }
    Assert_ifParanoid(worst);
    return worst;
}

static struct Address* getBest(struct RumorMill_pvt* rm)
{
    if (rm->pub.count == 0) { return NULL; }
    struct Address* best = NULL;
    int howBadIsTheBest = -1;
    for (int i = 0; i < rm->pub.count; i++) {
        int howBad = getBadness(&rm->addresses[i], rm->selfAddr);
        if (howBad < howBadIsTheBest || !best) {
            howBadIsTheBest = howBad;
            best = &rm->addresses[i];
        }
    }
    Assert_true(best);
    return best;
}

void RumorMill_addNode(struct RumorMill* mill, struct Address* addr)
{
    if (hasNode(mill, addr)) { return; } // Avoid duplicates
    struct RumorMill_pvt* rm = Identity_check((struct RumorMill_pvt*) mill);
    if (!Bits_memcmp(addr->key, rm->selfAddr->key, 32)) { return; }
    Address_getPrefix(addr);
    struct Address* replace;
    if (rm->pub.count < rm->capacity) {
        replace = &rm->addresses[rm->pub.count++];
    } else {
        replace = getWorst(rm);
    }
    Bits_memcpyConst(replace, addr, sizeof(struct Address));
}

bool RumorMill_getNode(struct RumorMill* mill, struct Address* output)
{
    struct RumorMill_pvt* rm = Identity_check((struct RumorMill_pvt*) mill);
    if (!rm->pub.count) { return false; }
    struct Address temp;
    struct Address* best = getBest(rm);
    Bits_memcpyConst(&temp, &rm->addresses[--rm->pub.count], sizeof(struct Address));
    Bits_memcpyConst(output, best, sizeof(struct Address));
    Bits_memcpyConst(best, &temp, sizeof(struct Address));
    return true;
}

struct RumorMill* RumorMill_new(struct Allocator* allocator, struct Address* selfAddr, int capacity)
{
    struct Allocator* alloc = Allocator_child(allocator);
    Address_getPrefix(selfAddr);

    struct RumorMill_pvt* rm = Allocator_calloc(alloc, sizeof(struct RumorMill_pvt), 1);
    rm->addresses = Allocator_calloc(alloc, sizeof(struct Address), capacity);
    rm->capacity = capacity;
    rm->selfAddr = Allocator_clone(alloc, selfAddr);
    Identity_set(rm);

    return &rm->pub;
}
