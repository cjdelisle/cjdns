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
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"

#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

#define Address_KEY_SIZE 32
#define Address_NETWORK_ADDR_SIZE 8
#define Address_SEARCH_TARGET_SIZE 16
#define Address_SERIALIZED_SIZE 40

struct Address
{
    union {
        struct {
            uint32_t three;
            uint32_t four;
            uint32_t one;
            uint32_t two;
        } ints;

        uint8_t bytes[Address_SEARCH_TARGET_SIZE];
    } ip6;

    uint8_t key[Address_KEY_SIZE];

    uint64_t networkAddress_be;
};
#define Address_SIZE (Address_SEARCH_TARGET_SIZE + Address_KEY_SIZE + Address_NETWORK_ADDR_SIZE)
Assert_assertTrue(sizeof(struct Address) == Address_SIZE);

static inline uint32_t Address_getPrefix(struct Address* addr)
{
    if (addr->ip6.ints.one == 0
        && addr->ip6.ints.two == 0
        && addr->ip6.ints.three == 0
        && addr->ip6.ints.four == 0)
    {
        AddressCalc_addressForPublicKey(addr->ip6.bytes, addr->key);
    }
    return Endian_bigEndianToHost32(addr->ip6.ints.one);
}

static inline uint32_t Address_prefixForSearchTarget(const uint8_t searchTarget[16])
{
    uint32_t prefix_be;
    memcpy(&prefix_be, &searchTarget[8], 4);
    return Endian_bigEndianToHost32(prefix_be);
}

static inline void Address_serialize(uint8_t output[Address_SERIALIZED_SIZE],
                                     const struct Address* addr)
{
    memcpy(output, addr->key, Address_SERIALIZED_SIZE);
}

static inline void Address_parse(struct Address* addr,
                                 const uint8_t input[Address_SERIALIZED_SIZE])
{
    memset(addr->ip6.bytes, 0, 16);
    memcpy(addr->key, input, Address_SERIALIZED_SIZE);
}

static inline bool Address_isSame(const struct Address* addr,
                                  const struct Address* addr2)
{
    return memcmp(addr->key, addr2->key, Address_SERIALIZED_SIZE) == 0;
}

static inline bool Address_isSameIp(const struct Address* addr,
                                    const struct Address* addr2)
{
    return memcmp(addr->key, addr2->key, Address_KEY_SIZE) == 0;
}

static inline bool Address_equalsSearchTarget(struct Address* addr,
                                              const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE])
{
    Address_getPrefix(addr);
    return memcmp(addr->ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);
}

static inline void Address_forKey(struct Address* out, const uint8_t key[Address_KEY_SIZE])
{
    memcpy(out->key, key, Address_KEY_SIZE);
    AddressCalc_addressForPublicKey(out->ip6.bytes, key);
}

static inline void Address_printNetworkAddress(uint8_t output[20], struct Address* input)
{
    uint8_t addr[8];
    memcpy(addr, &input->networkAddress_be, 8);
    sprintf((char*)output, "%.2x%.2x.%.2x%.2x.%.2x%.2x.%.2x%.2x",
            addr[0],
            addr[1],
            addr[2],
            addr[3],
            addr[4],
            addr[5],
            addr[6],
            addr[7]);
}

static inline void Address_printIp(uint8_t output[40], struct Address* addr)
{
    Address_getPrefix(addr);
    sprintf((char*)output, "%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x:%.2x%.2x",
            addr->ip6.bytes[ 0],
            addr->ip6.bytes[ 1],
            addr->ip6.bytes[ 2],
            addr->ip6.bytes[ 3],
            addr->ip6.bytes[ 4],
            addr->ip6.bytes[ 5],
            addr->ip6.bytes[ 6],
            addr->ip6.bytes[ 7],
            addr->ip6.bytes[ 8],
            addr->ip6.bytes[ 9],
            addr->ip6.bytes[10],
            addr->ip6.bytes[11],
            addr->ip6.bytes[12],
            addr->ip6.bytes[13],
            addr->ip6.bytes[14],
            addr->ip6.bytes[15]);
}

static inline void Address_print(uint8_t output[60], struct Address* addr)
{
    Address_printIp(output, addr);
    output[39] = '@';
    Address_printNetworkAddress(output + 40, addr);
}

static inline int Address_parseNetworkAddress(uint64_t* out_be, uint8_t netAddr[20])
{
    if (netAddr[4] != '.' || netAddr[9] != '.' || netAddr[14] != '.') {
        return -1;
    }
    uint8_t hex[16];
    memcpy(hex, netAddr, 4);
    memcpy(hex + 4, netAddr + 5, 4);
    memcpy(hex + 8, netAddr + 10, 4);
    memcpy(hex + 12, netAddr + 15, 4);

    uint8_t numberBytes_be[8];
    if (Hex_decode(numberBytes_be, 8, hex, 16) != 8) {
        return -1;
    }
    memcpy(out_be, numberBytes_be, 8);
    return 0;
}

/**
 * Detect a redundant (looping) route.
 *
 * @param addrA
 * @param addrB
 * @return 1 if addrA is a redundant version of addrB, -1 if addrB is a redundant version of
 *         addrA, 0 if neither is a redundant version of the other.
 */
static inline int Address_checkRedundantRoute(struct Address* addrA, struct Address* addrB)
{
    if (addrA->networkAddress_be == addrB->networkAddress_be) {
        return 0;
    }
    uint64_t addrANet = Endian_bigEndianToHost64(addrA->networkAddress_be);
    uint64_t addrBNet = Endian_bigEndianToHost64(addrB->networkAddress_be);

    if (addrANet > addrBNet) {
        uint64_t mask = (1 << Bits_log2x64(addrBNet)) - 1;
        return (addrANet & mask) == (addrBNet & mask);
    } else {
        uint64_t mask = (1 << Bits_log2x64(addrANet)) - 1;
        return -((addrANet & mask) == (addrBNet & mask));
    }
}

/**
 * Determine if the route to one node passes through another node.
 *
 * @param destination the node to route to.
 * @param midPath the node which might be in the middle of the route.
 * @return true if midPath is in the middle of the route to destination.
 */
static inline bool Address_routesThrough(uint64_t destination, uint64_t midPath)
{
    if (midPath > destination) {
        return false;
    }
    uint64_t mask = (1 << Bits_log2x64(destination)) - 1;
    return (destination & mask) == (midPath & mask);
}

#endif
