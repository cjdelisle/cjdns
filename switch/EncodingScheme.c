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
#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include "util/Hex.h"

/** Greatest possible number using x bits, all are set. */
#define MAX_BITS(x) ((((uint64_t)1)<<(x))-1)

int EncodingScheme_getFormNum(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    if (scheme->count == 1) {
        return 0;
    }

    for (int i = 0; i < scheme->count; i++) {
        struct EncodingScheme_Form* form = &scheme->forms[i];
        Assert_true(form->prefixLen > 0 && form->prefixLen < 32);
        Assert_true(form->bitCount > 0 && form->bitCount < 32);
        if (0 == ((form->prefix ^ (uint32_t)routeLabel) << (32 - form->prefixLen))) {
            return i;
        }
    }
    return EncodingScheme_getFormNum_INVALID;
}

uint64_t EncodingScheme_convertLabel(struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo)
{
    if (scheme->count == 1) {
        // fixed width encoding, this is easy
        switch (convertTo) {
            case 0:
            case EncodingScheme_convertLabel_convertTo_CANNONICAL: return routeLabel;
            default: return EncodingScheme_convertLabel_INVALID;
        }
    }

    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == EncodingScheme_getFormNum_INVALID) {
        return EncodingScheme_convertLabel_INVALID;
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    routeLabel >>= currentForm->prefixLen;
    uint64_t director = routeLabel & MAX_BITS(currentForm->bitCount);
    routeLabel >>= currentForm->bitCount;

    int minBits = Bits_log2x64(director) + 1;
    if (convertTo == EncodingScheme_convertLabel_convertTo_CANNONICAL) {
        for (int i = 0; i < scheme->count; i++) {
            struct EncodingScheme_Form* form = &scheme->forms[i];
            if (form->bitCount >= minBits) {
                convertTo = i;
                break;
            }
        }
    }

    if (convertTo < 0 || convertTo >= scheme->count) {
        // convertTo value is insane
        return EncodingScheme_convertLabel_INVALID;
    }

    struct EncodingScheme_Form* nextForm = &scheme->forms[convertTo];

    if (minBits > nextForm->bitCount) {
        // won't fit in requested form
        return EncodingScheme_convertLabel_INVALID;
    }
    if (EncodingScheme_formSize(nextForm) > EncodingScheme_formSize(currentForm)
        && (Bits_log2x64(routeLabel) + nextForm->bitCount + nextForm->prefixLen) > 59)
    {
        // All labels need 3 high zero bits
        return EncodingScheme_convertLabel_INVALID;
    }

    routeLabel <<= nextForm->bitCount;
    routeLabel |= director;
    routeLabel <<= nextForm->prefixLen;
    routeLabel |= nextForm->prefix;
    return routeLabel;
}


/**
 * Decode a form from it's binary representation.
 * Can only use a maximum of 41 bits.
 *
 * @param out the output which will be populated with the encoding form data.
 * @param data the binary data in host order.
 * @return the number of bits of data which were consumed by the decoding.
 *         If the content is definitely not an encoding form, 0 is returned.
 */
static inline int decodeForm(struct EncodingScheme_Form* out, uint64_t d)
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

static inline int encodeForm(struct EncodingScheme_Form* in, uint64_t* data, int bits)
{
    *data |= ((uint64_t)in->prefixLen & MAX_BITS(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->bitCount & MAX_BITS(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->prefix & MAX_BITS(in->prefixLen)) << bits;

    return 5 + 5 + in->prefixLen;
}

bool EncodingScheme_isSane(struct EncodingScheme* scheme)
{
    // Check for obviously insane encoding.
    if (scheme->count == 0) {
        // No encoding schemes
        return false;
    }

    if (scheme->count > 31) {
        // impossible, each form must have a different bitCount and bitCount
        // can only be expressed in 5 bits limiting it to 31 bits max and a form
        // using zero bits is not allowed so there are only 31 max possibilities.
        return false;
    }

    if (scheme->count == 1) {
        // Fixed width encoding, prefix is not allowed and bitcount must be non-zero
        if (scheme->forms[0].prefixLen != 0) {
            // prefixLen must be 0
            return false;
        }
        if (scheme->forms[0].bitCount == 0) {
            // bitcount must be non-zero
            return false;
        }
        return true;
    }

    // Variable width encoding.
    for (int i = 0; i < scheme->count; i++) {
        struct EncodingScheme_Form* form = &scheme->forms[i];

        if (form->prefixLen == 0) {
            // Prefix must exist in order to distinguish between forms
            return false;
        }
        if (form->bitCount == 0) {
            // Bitcount must be non-zero
            return false;
        }
        if (EncodingScheme_formSize(form) > 59) {
            // cannot be represented in the usable space in a label
            return false;
        }
        if (i > 0 && form->bitCount <= scheme->forms[i-1].bitCount) {
            // Forms must be in ascending order.
            return false;
        }
        for (int j = 0; j < scheme->count; j++) {
            // Forms must be distinguishable by their prefixes.
            if (j != i && (scheme->forms[j].prefix & MAX_BITS(form->prefixLen)) == form->prefix)
            {
                return false;
            }
        }
    }
    return true;
}

List* EncodingScheme_asList(struct EncodingScheme* list, struct Allocator* alloc)
{
    Assert_true(EncodingScheme_isSane(list));
    String* prefixLen = String_new("prefixLen", alloc);
    String* bitCount = String_new("bitCount", alloc);
    String* prefix = String_new("prefix", alloc);
    List* scheme = NULL;
    for (int i = 0; i < (int)list->count; i++) {
        Dict* form = Dict_new(alloc);
        Dict_putInt(form, prefixLen, list->forms[i].prefixLen, alloc);
        Dict_putInt(form, bitCount, list->forms[i].bitCount, alloc);
        String* pfx = String_newBinary(NULL, 8, alloc);
        uint32_t prefix_be = Endian_hostToBigEndian32(list->forms[i].prefix);
        Hex_encode(pfx->bytes, 8, (uint8_t*)&prefix_be, 4);
        Dict_putString(form, prefix, pfx, alloc);
        scheme = List_addDict(scheme, form, alloc);
    }
    return scheme;
}

struct EncodingScheme* EncodingScheme_fromList(List* scheme, struct Allocator* alloc)
{
    struct EncodingScheme* list = Allocator_malloc(alloc, sizeof(struct EncodingScheme));
    list->count = List_size(scheme);
    list->forms = Allocator_malloc(alloc, sizeof(struct EncodingScheme_Form) * list->count);
    for (int i = 0; i < (int)list->count; i++) {
        Dict* form = List_getDict(scheme, i);
        uint64_t* prefixLen = Dict_getInt(form, String_CONST("prefixLen"));
        uint64_t* bitCount = Dict_getInt(form, String_CONST("bitCount"));
        String* prefixStr = Dict_getString(form, String_CONST("prefix"));
        if (!prefixLen || !bitCount || !prefixStr || prefixStr->len != 8) {
            return NULL;
        }
        uint32_t prefix_be;
        if (Hex_decode((uint8_t*)&prefix_be, 4, prefixStr->bytes, 8) != 4) {
            return NULL;
        }
        list->forms[i].prefixLen = *prefixLen;
        list->forms[i].bitCount = *bitCount;
        list->forms[i].prefix = Endian_bigEndianToHost32(prefix);
    }
    if (!EncodingScheme_isSane(list)) {
        return NULL;
    }
    return list;
}

String* EncodingScheme_serialize(struct EncodingScheme* list,
                                 struct Allocator* alloc)
{
    Assert_true(EncodingScheme_isSane(list));

    // Create the string as the largest that is possible for the list size.
    String* out = String_newBinary(NULL, list->count * 6, alloc);

    int bits = 0;
    int outIndex = 0;
    uint64_t block = 0;
    for (int listIndex = 0; listIndex < (int)list->count; listIndex++) {
        bits += encodeForm(&list->forms[listIndex], &block, bits);
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

struct EncodingScheme* EncodingScheme_deserialize(String* data,
                                                  struct Allocator* alloc)
{
    struct EncodingScheme_Form* forms = NULL;
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
        struct EncodingScheme_Form next;
        int ret = decodeForm(&next, block);
        bits -= ret;
        if (!ret || bits < 0) {
            if (block || dataIndex < (int)data->len || bits < 0) {
                // Invalid encoding
                return NULL;
            }
            break;
        }
        block >>= ret;

        #ifdef PARANOIA
            if (next.prefixLen == 0) {
                Assert_true(next.prefix == 0);
            } else {
                Assert_true(next.prefix >> next.prefixLen == 0);
            }
        #endif

        outCount += 1;
        forms = Allocator_realloc(alloc, forms, outCount * sizeof(struct EncodingScheme_Form));
        Bits_memcpyConst(&forms[outCount-1], &next, sizeof(struct EncodingScheme_Form));
    }

    struct EncodingScheme* out = Allocator_clone(alloc, (&(struct EncodingScheme) {
        .forms = forms,
        .count = outCount
    }));

    return EncodingScheme_isSane(out) ? out : NULL;
}

struct EncodingScheme* EncodingScheme_defineFixedWidthScheme(int bitCount, struct Allocator* alloc)
{
    struct NumberCompress_FixedWidthScheme
    {
        struct EncodingScheme scheme;
        struct EncodingScheme_Form form;
    };
    struct NumberCompress_FixedWidthScheme scheme = {
        .scheme = { .count = 1, .forms = &scheme.form },
        .form = { .bitCount = bitCount, .prefixLen = 0, .prefix = 0, },
    };

    struct NumberCompress_FixedWidthScheme* out =
        Allocator_malloc(alloc, sizeof(struct NumberCompress_FixedWidthScheme));

    Bits_memcpyConst(out, &scheme, sizeof(struct NumberCompress_FixedWidthScheme));

    return &out->scheme;
}


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(struct EncodingScheme_Form* forms,
                                                           int formCount,
                                                           struct Allocator* alloc)
{
    struct EncodingScheme_Form* formsCopy =
        Allocator_malloc(alloc, sizeof(struct EncodingScheme_Form) * formCount);
    Bits_memcpy(formsCopy, forms, sizeof(struct EncodingScheme_Form) * formCount);

    return Allocator_clone(alloc, (&(struct EncodingScheme) {
        .count = formCount,
        .forms = formsCopy
    }));
}

int EncodingScheme_compare(struct EncodingScheme* a, struct EncodingScheme* b)
{
    if (a->count == b->count) {
        return Bits_memcmp(a->forms, b->forms, sizeof(struct EncodingScheme_Form) * a->count);
    }
    return a->count > b->count ? 1 : -1;
}
