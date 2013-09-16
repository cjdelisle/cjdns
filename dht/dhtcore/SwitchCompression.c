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
#include "benc/String.h"
#include "memory/Allocator.h"
#include "dht/dhtcore/SwitchCompression.h"
#include "util/Bits.h"

/** Greatest possible number using x bits, all are set. */
#define MAX_BITS(x) ((1<<(x))-1)

/**
 * Decode a scheme from it's binary representation.
 * Can only use a maximum of 41 bits.
 *
 * @param out the output which will be populated with the encoding scheme data.
 * @param data the binary data in host order.
 * @return the number of bits of data which were consumed by the decoding.
 *         If the content is definitely not an encoding scheme, 0 is returned.
 */
static inline int decodeScheme(struct SwitchCompression_Scheme* out, uint64_t d)
{
    out->prefixLen = d & MAX_BITS(5);
    d >>= 5;
    int bitCount = d & MAX_BITS(5);
    if (bitCount < 1) {
        return 0;
    }
    out->bitCount = bitCount;
    d >>= 5;
    out->prefix = d & MAX_BITS(out->prefixLen);
    return 5 + 5 + out->prefixLen;
}

static inline int encodeScheme(struct SwitchCompression_Scheme* in, uint64_t* data, int bits)
{
    *data |= ((uint64_t)in->prefixLen & MAX_BITS(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->bitCount & MAX_BITS(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->prefix & MAX_BITS(in->prefixLen)) << bits;

    return 5 + 5 + in->prefixLen;
}

String* SwitchCompression_encodeSchemes(struct SwitchCompression_SchemeList* list,
                                        struct Allocator* alloc)
{
    if (list->count == 0) {
        return String_new("", alloc);
    }

    // Create the string as the largest that is possible for the list size.
    String* out = String_newBinary(NULL, list->count * 6, alloc);

    int bits = 0;
    int outIndex = 0;
    uint64_t block = 0;
    for (int listIndex = 0; listIndex < (int)list->count; listIndex++) {
        bits += encodeScheme(&list->elems[listIndex], &block, bits);
        while (bits > 8) {
            Assert_true(outIndex < (int)out->len);
            out->bytes[outIndex++] = (uint8_t) (block & 0xff);
            bits -= 8;
            block >>= 8;
        }
    }

    if (bits > 0) {
        out->bytes[outIndex++] = (uint8_t) (block & 0xff);
    }

    out->len = outIndex;

    return out;
}

struct SwitchCompression_SchemeList* SwitchCompression_decodeSchemes(String* data,
                                                                     struct Allocator* alloc)
{
    struct SwitchCompression_Scheme* out = NULL;
    int outCount = 0;

    uint64_t block = 0;
    int bits = 0;
    int dataIndex = 0;

    for (;;) {
        // load data into the block from the incoming data source
        while (bits < 56 && dataIndex < (int)data->len) {
            block |= (((uint64_t)data->bytes[dataIndex++] & 0xff) << bits);
            bits += 8;
        }
        struct SwitchCompression_Scheme next;
        int ret = decodeScheme(&next, block);
        bits -= ret;
        if (!ret || bits < 0) {
            if (block || dataIndex < (int)data->len || bits < 0) {
                // Invalid encoding
                return NULL;
            }
            break;
        }
        block >>= ret;

        outCount += 1;
        out = Allocator_realloc(alloc, out, outCount * sizeof(struct SwitchCompression_Scheme));
        Bits_memcpyConst(&out[outCount-1], &next, sizeof(struct SwitchCompression_Scheme));
    }

    return Allocator_clone(alloc, (&(struct SwitchCompression_SchemeList) {
        .elems = out,
        .count = outCount
    }));
}
