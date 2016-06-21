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

#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("dht/Address.c");

#include <stdint.h>
#include <stdbool.h>

#define Address_KEY_SIZE 32
#define Address_NETWORK_ADDR_SIZE 8
#define Address_SEARCH_TARGET_SIZE 16
#define Address_SERIALIZED_SIZE 40

// Don't undefine this yet, new code will depend on it so that it can be tracked down and changed.
#define Address_ROT64

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

struct Address_List* Address_List_new(uint32_t length, struct Allocator* alloc);

uint32_t Address_getPrefix(struct Address* addr);

uint32_t Address_prefixForIp6(uint8_t ip6[16]);

uint32_t Address_prefixForSearchTarget(const uint8_t searchTarget[16]);

void Address_serialize(uint8_t output[Address_SERIALIZED_SIZE], const struct Address* addr);

void Address_parse(struct Address* addr, const uint8_t input[Address_SERIALIZED_SIZE]);

bool Address_isSame(const struct Address* addr,
                    const struct Address* addr2);

bool Address_isSameIp(const struct Address* addr,
                      const struct Address* addr2);

bool Address_equalsSearchTarget(struct Address* addr,
                                const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE]);

void Address_forKey(struct Address* out, const uint8_t key[Address_KEY_SIZE]);

void Address_printIp(uint8_t output[40], struct Address* addr);

void Address_printShortIp(uint8_t output[40], struct Address* addr);

void Address_print(uint8_t output[60], struct Address* addr);

String* Address_toString(struct Address* addr, struct Allocator* alloc);

struct Address* Address_fromString(String* str, struct Allocator* alloc);

struct Address* Address_clone(struct Address* orig, struct Allocator* alloc);

int Address_xorcmp(uint32_t target,
                   uint32_t negativeIfCloser,
                   uint32_t positiveIfCloser);

/**
 * Return which node is closer to the target.
 *
 * @param target the address to test distance against.
 * @param negativeIfCloser one address to check distance.
 * @param positiveIfCloser another address to check distance.
 * @return -1 if negativeIfCloser is closer to target, 1 if positiveIfCloser is closer
 *         0 if they are both the same distance.
 */
int Address_closest(struct Address* target,
                    struct Address* negativeIfCloser,
                    struct Address* positiveIfCloser);

#endif
