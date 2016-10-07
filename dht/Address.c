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
#include "dht/Address.h"
#include "crypto/AddressCalc.h"
#include "crypto/Key.h"
#include "util/AddrTools.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/Defined.h"

struct Address_List* Address_List_new(uint32_t length, struct Allocator* alloc)
{
    struct Address_List* out = Allocator_malloc(alloc, sizeof(struct Address_List));
    out->length = length;
    out->elems = Allocator_calloc(alloc, Address_SIZE, length);
    return out;
}

uint32_t Address_prefixForIp6(uint8_t ip6[16])
{
    uint32_t word = ((uint32_t*)ip6)[ Defined(Address_ROT64) ? 2 : 0 ];
    return Endian_bigEndianToHost32(word);
}

uint32_t Address_getPrefix(struct Address* addr)
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

uint32_t Address_prefixForSearchTarget(const uint8_t searchTarget[16])
{
    uint32_t prefix_be;
    Bits_memcpy(&prefix_be, &searchTarget[8], 4);
    return Endian_bigEndianToHost32(prefix_be);
}

void Address_serialize(uint8_t output[Address_SERIALIZED_SIZE], const struct Address* addr)
{
    Bits_memcpy(output, addr->key, Address_SERIALIZED_SIZE);
    if (!Endian_isBigEndian()) {
        uint64_t path_be = Endian_hostToBigEndian64(addr->path);
        Bits_memcpy(output + Address_KEY_SIZE, &path_be, Address_NETWORK_ADDR_SIZE);
    }
}

void Address_parse(struct Address* addr, const uint8_t input[Address_SERIALIZED_SIZE])
{
    Bits_memset(addr->ip6.bytes, 0, 16);
    Bits_memcpy(addr->key, input, Address_SERIALIZED_SIZE);
    addr->path = Endian_bigEndianToHost64(addr->path);
}

bool Address_isSame(const struct Address* addr,
                    const struct Address* addr2)
{
    return Bits_memcmp(addr->key, addr2->key, Address_SERIALIZED_SIZE) == 0;
}

bool Address_isSameIp(const struct Address* addr,
                      const struct Address* addr2)
{
    return Bits_memcmp(addr->ip6.bytes, addr2->ip6.bytes, 16) == 0;
}

bool Address_equalsSearchTarget(struct Address* addr,
                                const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE])
{
    Address_getPrefix(addr);
    return Bits_memcmp(addr->ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);
}

void Address_forKey(struct Address* out, const uint8_t key[Address_KEY_SIZE])
{
    Bits_memcpy(out->key, key, Address_KEY_SIZE);
    AddressCalc_addressForPublicKey(out->ip6.bytes, key);
}

void Address_printIp(uint8_t output[40], struct Address* addr)
{
    Address_getPrefix(addr);
    AddrTools_printIp(output, addr->ip6.bytes);
}

void Address_printShortIp(uint8_t output[40], struct Address* addr)
{
    Address_getPrefix(addr);
    AddrTools_printShortIp(output, addr->ip6.bytes);
}

void Address_print(uint8_t output[60], struct Address* addr)
{
    Address_printIp(output, addr);
    output[39] = '@';
    AddrTools_printPath(output + 40, addr->path);
}

int Address_xorcmp(uint32_t target,
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
int Address_closest(struct Address* target,
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

String* Address_toString(struct Address* addr, struct Allocator* alloc)
{
    struct Allocator* temp = Allocator_child(alloc);
    String* key = Key_stringify(addr->key, temp);
    String* path = String_newBinary(NULL, 19, temp);
    AddrTools_printPath(path->bytes, addr->path);
    String* out = String_printf(alloc, "v%u.%s.%s", addr->protocolVersion, path->bytes, key->bytes);
    Allocator_free(temp);
    return out;
}

struct Address* Address_fromString(String* str, struct Allocator* alloc)
{
    // v6.0000.0000.0000.0001.yw4hn81kh3f9n39ff3qhnhdl8ngd662utbpgtjp5q4b0yxqv4by0.k
    if (str->len < 77) { return NULL; }
    if (str->bytes[0] != 'v') { return NULL; }

    struct Address addr = { .protocolVersion = 0 };

    int i = 1;
    for (; str->bytes[i] && str->bytes[i] != '.'; i++) {
        Assert_true(i < 77);
        if (str->bytes[i] > '9' || str->bytes[i] < '0') { return NULL; }
        addr.protocolVersion *= 10;
        addr.protocolVersion += (str->bytes[i] - '0');
    }

    if (str->bytes[i] != '.') { return NULL; }
    i++;
    // 0000.0000.0000.0001.yw4hn81kh3f9n39ff3qhnhdl8ngd662utbpgtjp5q4b0yxqv4by0.k
    if (str->len - i != 74) { return NULL; }
    if (str->bytes[i+19] != '.') { return NULL; }
    str->bytes[i+19] = '\0';
    int ret = AddrTools_parsePath(&addr.path, &str->bytes[i]);
    str->bytes[i+19] = '.';
    if (ret) { return NULL; }
    String keyPart = { .len = str->len - i - 20,  .bytes = &str->bytes[i + 20] };
    Assert_true(keyPart.len == 54 && keyPart.bytes[53] == 'k');
    if (Key_parse(&keyPart, addr.key, addr.ip6.bytes)) { return NULL; }

    return Allocator_clone(alloc, &addr);
}

struct Address* Address_clone(struct Address* orig, struct Allocator* alloc)
{
    return Allocator_clone(alloc, orig);
}
