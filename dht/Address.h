#ifndef ADDRESS_H
#define ADDRESS_H

#include "crypto/AddressCalc.h"
#include "util/Assert.h"
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

    uint8_t networkAddress[Address_NETWORK_ADDR_SIZE];
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
    sprintf((char*)output, "%.2x%.2x.%.2x%.2x.%.2x%.2x.%.2x%.2x",
            input->networkAddress[0],
            input->networkAddress[1],
            input->networkAddress[2],
            input->networkAddress[3],
            input->networkAddress[4],
            input->networkAddress[5],
            input->networkAddress[6],
            input->networkAddress[7]);
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

#endif
