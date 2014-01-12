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
#ifndef Address_H
#define Address_H

#include "crypto/AddressCalc.h"
#include "util/AddrTools.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/platform/libc/strlen.h"

#include <stdint.h>
#include <stdbool.h>

#define Address_KEY_SIZE 32
#define Address_NETWORK_ADDR_SIZE 8
#define Address_SEARCH_TARGET_SIZE 16
#define Address_SERIALIZED_SIZE 40

struct Address
{
    /** The protocol version of the node. */
    uint32_t protocolVersion;

    /** unused */
    uint32_t padding;

    union {
        struct {
            // tricksy: this is effectively a 64 bit rotate of the following bytes array
            uint32_t three_be;
            uint32_t four_be;
            uint32_t one_be;
            uint32_t two_be;
        } ints;

        struct {
            uint64_t two_be;
            uint64_t one_be;
        } longs;

        uint8_t bytes[Address_SEARCH_TARGET_SIZE];
    } ip6;

    uint8_t key[Address_KEY_SIZE];

    uint64_t path;
};
#define Address_SIZE (8 + Address_SEARCH_TARGET_SIZE + Address_KEY_SIZE + Address_NETWORK_ADDR_SIZE)
Assert_compileTime(sizeof(struct Address) == Address_SIZE);

struct Address_List
{
    int length;
    struct Address* elems;
};

static inline struct Address_List* Address_List_new(uint32_t length, struct Allocator* alloc)
{
    struct Address_List* out = Allocator_malloc(alloc, sizeof(struct Address_List));
    out->length = length;
    out->elems = Allocator_calloc(alloc, Address_SIZE, length);
    return out;
}

static inline uint32_t Address_getPrefix(struct Address* addr)
{
    if (addr->ip6.ints.one_be == 0
        && addr->ip6.ints.two_be == 0
        && addr->ip6.ints.three_be == 0
        && addr->ip6.ints.four_be == 0)
    {
        AddressCalc_addressForPublicKey(addr->ip6.bytes, addr->key);
    }
    return Endian_bigEndianToHost32(addr->ip6.ints.one_be);
}

static inline uint32_t Address_prefixForSearchTarget(const uint8_t searchTarget[16])
{
    uint32_t prefix_be;
    Bits_memcpyConst(&prefix_be, &searchTarget[8], 4);
    return Endian_bigEndianToHost32(prefix_be);
}

static inline void Address_serialize(uint8_t output[Address_SERIALIZED_SIZE],
                                     const struct Address* addr)
{
    Bits_memcpyConst(output, addr->key, Address_SERIALIZED_SIZE);
    if (!Endian_isBigEndian()) {
        uint64_t path_be = Endian_hostToBigEndian64(addr->path);
        Bits_memcpyConst(output + Address_KEY_SIZE, &path_be, Address_NETWORK_ADDR_SIZE);
    }
}

static inline void Address_parse(struct Address* addr,
                                 const uint8_t input[Address_SERIALIZED_SIZE])
{
    Bits_memset(addr->ip6.bytes, 0, 16);
    Bits_memcpyConst(addr->key, input, Address_SERIALIZED_SIZE);
    addr->path = Endian_bigEndianToHost64(addr->path);
}

static inline bool Address_isSame(const struct Address* addr,
                                  const struct Address* addr2)
{
    return Bits_memcmp(addr->key, addr2->key, Address_SERIALIZED_SIZE) == 0;
}

static inline bool Address_isSameIp(const struct Address* addr,
                                    const struct Address* addr2)
{
    return Bits_memcmp(addr->ip6.bytes, addr2->ip6.bytes, 16) == 0;
}

static inline bool Address_equalsSearchTarget(
    struct Address* addr,
    const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE])
{
    Address_getPrefix(addr);
    return Bits_memcmp(addr->ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);
}

static inline void Address_forKey(struct Address* out, const uint8_t key[Address_KEY_SIZE])
{
    Bits_memcpyConst(out->key, key, Address_KEY_SIZE);
    AddressCalc_addressForPublicKey(out->ip6.bytes, key);
}

static inline void Address_printIp(uint8_t output[40], struct Address* addr)
{
    Address_getPrefix(addr);
    AddrTools_printIp(output, addr->ip6.bytes);
}

static inline void Address_print(uint8_t output[60], struct Address* addr)
{
    Address_printIp(output, addr);
    output[39] = '@';
    AddrTools_printPath(output + 40, addr->path);
}

static inline int Address_xorcmp(uint32_t target,
                                 uint32_t negativeIfCloser,
                                 uint32_t positiveIfCloser)
{
    if (negativeIfCloser == positiveIfCloser) {
        return 0;
    }
    uint32_t ref = Endian_bigEndianToHost32(target);
    return ((Endian_bigEndianToHost32(negativeIfCloser) ^ ref)
               < (Endian_bigEndianToHost32(positiveIfCloser) ^ ref)) ? -1 : 1;
}

/**
 * Return which node is closer to the target.
 *
 * @param target the address to test distance against.
 * @param negativeIfCloser one address to check distance.
 * @param positiveIfCloser another address to check distance.
 * @return -1 if negativeIfCloser is closer to target, 1 if positiveIfCloser is closer
 *         0 if they are both the same distance.
 */
static inline int Address_closest(struct Address* target,
                                  struct Address* negativeIfCloser,
                                  struct Address* positiveIfCloser)
{
    Address_getPrefix(target);
    Address_getPrefix(negativeIfCloser);
    Address_getPrefix(positiveIfCloser);

    int ret = 0;

    #define Address_COMPARE(part) \
        if ((ret = Address_xorcmp(target->ip6.ints.part,               \
                                  negativeIfCloser->ip6.ints.part,     \
                                  positiveIfCloser->ip6.ints.part)))   \
        {                                                              \
            return ret;                                                \
        }

    Address_COMPARE(one_be)
    Address_COMPARE(two_be)
    Address_COMPARE(three_be)
    Address_COMPARE(four_be)

    return 0;

    #undef Address_COMPARE
}

#endif
