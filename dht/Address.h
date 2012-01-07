#ifndef ADDRESS_H
#define ADDRESS_H

#include "crypto/AddressCalc.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Endian.h"

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
        uint64_t mask = (1 << Bits_log264(addrBNet)) - 1;
        return (addrANet & mask) == (addrBNet & mask);
    } else {
        uint64_t mask = (1 << Bits_log264(addrANet)) - 1;
        return -((addrANet & mask) == (addrBNet & mask));
    }
}

#endif
