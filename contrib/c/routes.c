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
#include "util/log/FileWriterLog.h"
#include "util/log/Log.h"
#include "util/Bits.h"
#include "util/Endian.h"

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

struct Prefix
{
    uint32_t addr;
    int prefix;
};

struct PrefixSet
{
    struct Prefix* prefixes;
    int count;
};
/*
static uint32_t mkMask(int prefixBits)
{
    return ((uint32_t)~0) << (32 - prefixBits);
}

static bool isPrefixSuperset(struct Prefix* maybeSubset, struct Prefix* maybeSuperset)
{
    if (maybeSubset->prefix > maybeSuperset->prefix) {
        return false;
    }
    uint32_t mask = mkMask(maybeSuperset->prefix);
    if ((maybeSubset->addr & mask) == (maybeSuperset->addr & mask)) {
        return true;
    }
    return false;
}
*/
static struct PrefixSet* invertPrefix(struct Prefix* toInvert, struct Allocator* alloc)
{
    struct Prefix* pfxs = Allocator_calloc(alloc, sizeof(struct Prefix), toInvert->prefix);
    struct PrefixSet* out = Allocator_calloc(alloc, sizeof(struct PrefixSet), 1);
    out->prefixes = pfxs;
    out->count = toInvert->prefix;
    for (int i = 32 - toInvert->prefix, c = 0; i < 32; i++) {
        pfxs[c].addr = ( toInvert->addr & (~0 << i) ) ^ (1 << i);
        pfxs[c].prefix = 32 - i;
        Assert_true(c < out->count);
        c++;
    }
    return out;
}
/*
static struct PrefixSet* mergePfxSets(struct PrefixSet* setOne,
                                      struct PrefixSet* setTwo,
                                      struct Allocator* alloc)
{
    for (int i = 0; i < setOne->count && i < setTwo->count; i++) {

    }
}
*/

static struct Prefix* mkPfx(uint8_t addrBytes[4], int prefix, struct Allocator* alloc)
{
    uint32_t addr;
    Bits_memcpy(&addr, addrBytes, 4);
    addr = Endian_hostToBigEndian32(addr);
    addr = addr >> (32 - prefix) << (32 - prefix);
    struct Prefix* pfx = Allocator_calloc(alloc, sizeof(struct Prefix), 1);
    pfx->addr = addr;
    pfx->prefix = prefix;
    return pfx;
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<22);
    struct Prefix* gateway = mkPfx(((uint8_t[]){ 198, 167, 222, 70 }), 32, alloc);
    //struct Prefix* lo = mkPfx(((uint8_t[]){ 127, 0, 0, 1 }), 8, alloc);
    //struct Prefix* lan = mkPfx(((uint8_t[]){ 192, 168, 0, 1 }), 16, alloc);
    struct Prefix* pfx = gateway;
    struct Log* log = FileWriterLog_new(stderr, alloc);
    struct PrefixSet* ps = invertPrefix(pfx, alloc);

    /*struct Prefix* lastPfx = &pfxs[31];
    for (int i = 30; i > 0; i--) {
        if (isPrefixSuperset(&pfxs[i], lastPfx)) {
            pfxs[i].prefix = 0;
            pfxs[i].addr = 0;
        } else {
            lastPfx = &pfxs[i];
        }
    }*/

    Log_debug(log, "invert: %d.%d.%d.%d/%d",
        pfx->addr >> 24,
        pfx->addr << 8 >> 24,
        pfx->addr << 16 >> 24,
        pfx->addr << 24 >> 24,
        pfx->prefix
    );
    for (int i = 0; i < ps->count; i++) {
        printf("route add %d.%d.%d.%d/%d -iface utun0\n",
            ps->prefixes[i].addr >> 24,
            ps->prefixes[i].addr << 8 >> 24,
            ps->prefixes[i].addr << 16 >> 24,
            ps->prefixes[i].addr << 24 >> 24,
            ps->prefixes[i].prefix
        );
    }

    return 0;
}
