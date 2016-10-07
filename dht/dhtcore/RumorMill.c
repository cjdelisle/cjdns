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
#include "util/Endian.h"
#include "util/Defined.h"

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

    int capacity;

    struct Log* log;

    Identity
};

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
        int howBad = getBadness(&rm->pub.addresses[i], rm->selfAddr);
        if (howBad > howBadIsTheWorst) {
            howBadIsTheWorst = howBad;
            worst = &rm->pub.addresses[i];
        }
    }
    Assert_true(worst);
    return worst;
}

static struct Address* getBest(struct RumorMill_pvt* rm)
{
    if (rm->pub.count == 0) { return NULL; }
    struct Address* best = NULL;
    int howBadIsTheBest = -1;
    for (int i = 0; i < rm->pub.count; i++) {
        int howBad = getBadness(&rm->pub.addresses[i], rm->selfAddr);
        if (howBad < howBadIsTheBest || !best) {
            howBadIsTheBest = howBad;
            best = &rm->pub.addresses[i];
        }
    }
    Assert_true(best);
    return best;
}

void RumorMill__addNode(struct RumorMill* mill, struct Address* addr, const char* file, int line)
{
    struct RumorMill_pvt* rm = Identity_check((struct RumorMill_pvt*) mill);

    Address_getPrefix(addr);
    Assert_true(addr->protocolVersion);

    for (int i = 0; i < rm->pub.count; i++) {
        if (rm->pub.addresses[i].path == addr->path &&
            !Bits_memcmp(rm->pub.addresses[i].key, addr->key, 32))
        {
            return;
        }
    }

    if (!Bits_memcmp(addr->key, rm->selfAddr->key, 32)) { return; }

    struct Address* replace;
    if (rm->pub.count < rm->capacity) {
        replace = &rm->pub.addresses[rm->pub.count++];
    } else {
        replace = getWorst(rm);
    }
    Bits_memcpy(replace, addr, sizeof(struct Address));

    if (Defined(Log_DEBUG)) {
        uint8_t addrStr[60];
        Address_print(addrStr, addr);
        Log_debug(rm->log, "[%s] addNode(%s) count[%d] from [%s:%d]",
                           rm->pub.name, addrStr, rm->pub.count, file, line);
    }
}

bool RumorMill_getNode(struct RumorMill* mill, struct Address* output)
{
    struct RumorMill_pvt* rm = Identity_check((struct RumorMill_pvt*) mill);
    if (!rm->pub.count) { return false; }
    struct Address* best = getBest(rm);
    if (output) {
        Bits_memcpy(output, best, sizeof(struct Address));
    }

    rm->pub.count--;
    if (&rm->pub.addresses[rm->pub.count] != best) {
        Bits_memcpy(best, &rm->pub.addresses[rm->pub.count], sizeof(struct Address));
    }
    return true;
}

struct RumorMill* RumorMill_new(struct Allocator* allocator,
                                struct Address* selfAddr,
                                int capacity,
                                struct Log* log,
                                const char* name)
{
    struct Allocator* alloc = Allocator_child(allocator);
    Address_getPrefix(selfAddr);

    struct RumorMill_pvt* rm = Allocator_calloc(alloc, sizeof(struct RumorMill_pvt), 1);
    rm->pub.addresses = Allocator_calloc(alloc, sizeof(struct Address), capacity);
    rm->capacity = capacity;
    rm->selfAddr = Allocator_clone(alloc, selfAddr);
    rm->log = log;
    rm->pub.name = name;
    Identity_set(rm);

    return &rm->pub;
}
