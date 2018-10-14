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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "benc/String.h"
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"

int EncodingScheme_getFormNum(const struct EncodingScheme* scheme, uint64_t routeLabel)
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

static const struct EncodingScheme ES_358 = {
    .count = 3,
    .forms = (struct EncodingScheme_Form[]) {
        { .bitCount = 3, .prefixLen = 1, .prefix = 1, },
        { .bitCount = 5, .prefixLen = 2, .prefix = 1<<1, },
        { .bitCount = 8, .prefixLen = 2, .prefix = 0, }
    }
};
const struct EncodingScheme* EncodingScheme_358()
{
    return &ES_358;
}

bool EncodingScheme_is358(const struct EncodingScheme* scheme)
{
    if (scheme->count != 3) { return false; }
    return !Bits_memcmp(ES_358.forms, scheme->forms, sizeof(struct EncodingScheme_Form) * 3);
}

int EncodingScheme_parseDirector(const struct EncodingScheme* scheme, uint64_t label)
{
    int formNum = EncodingScheme_getFormNum(scheme, label);
    if (formNum == EncodingScheme_getFormNum_INVALID) {
        return EncodingScheme_parseDirector_INVALID;
    }
    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    int dir = (label >> currentForm->prefixLen) & Bits_maxBits64(currentForm->bitCount);

    if (EncodingScheme_is358(scheme)) {
        // slot 0 must always be represented as a 1, so in 358, 0 and 1 are swapped.
        if (formNum > 0) {
            dir += (dir > 0);
        } else {
            dir += (dir == 0) - (dir == 1);
        }
    }
    return dir;
}

uint64_t EncodingScheme_serializeDirector(
    const struct EncodingScheme* scheme, int dir, int formNum)
{
    if (!EncodingScheme_is358(scheme)) {
        if (formNum < 0) {
            for (formNum = 0; formNum < scheme->count; formNum++) {
                if (!(dir >> scheme->forms[formNum].bitCount)) { break; }
            }
        }
    } else {
        if (formNum < 0) {
            for (formNum = 0; formNum < scheme->count; formNum++) {
                if (!((dir - (!!formNum)) >> scheme->forms[formNum].bitCount)) { break; }
            }
        }

        if (formNum) {
            if (dir == 1) { return ~0ull; }
            // slot 1 is only represented in form 0 so in all other forms, it is skipped.
            dir -= (dir > 0);
        } else {
            // slot 0 must always be represented as a 1, so 0 and 1 are swapped.
            dir += (dir == 0) - (dir == 1);
        }
    }

    if (formNum >= scheme->count) { return ~0ull; }

    struct EncodingScheme_Form* f = &scheme->forms[formNum];
    return (((uint64_t)dir) << f->prefixLen) | f->prefix;
}

uint64_t EncodingScheme_convertLabel(const struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo)
{
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum < 0) { return EncodingScheme_convertLabel_INVALID; }
    struct EncodingScheme_Form* inForm = &scheme->forms[formNum];

    int dir = EncodingScheme_parseDirector(scheme, routeLabel);
    if (dir < 0) { return EncodingScheme_convertLabel_INVALID; }
    uint64_t dirS = EncodingScheme_serializeDirector(scheme, dir, convertTo);
    if (dirS == ~0ull) { return EncodingScheme_convertLabel_INVALID; }
    int outFormNum = EncodingScheme_getFormNum(scheme, dirS);
    struct EncodingScheme_Form* outForm = &scheme->forms[outFormNum];

    routeLabel >>= (inForm->prefixLen + inForm->bitCount);
    int outFormBits = (outForm->prefixLen + outForm->bitCount);
    if (Bits_log2x64(routeLabel) + outFormBits > 59) {
        return EncodingScheme_convertLabel_INVALID;
    }
    return (routeLabel << outFormBits) | dirS;
}

/**
 * Decode a form from its binary representation.
 * Can only use a maximum of 41 bits.
 *
 * One or more of these binary representation are bitwise concatenated to
 * give an unsigned integer; which is encoded in **little endian** to give
 * the serialization of Encoding Scheme.
 *
 * Ten least significant bits of a form are:
 *
 *                     1
 *     0 1 2 3 4 5 6 7 0 1
 *    +-+-+-+-+-+-+-+-+-+-+
 *  0 | bitcount| preflen |
 *    +-+-+-+-+-+-+-+-+-+-+
 *
 * Previous 'preflen' bits are the prefix
 *
 * @param out the output which will be populated with the encoding form data.
 * @param data the binary data in host order.
 * @return the number of bits of data which were consumed by the decoding.
 *         If the content is definitely not an encoding form, 0 is returned.
 */
static inline int decodeForm(struct EncodingScheme_Form* out, uint64_t d)
{
    out->prefixLen = d & Bits_maxBits64(5);
    d >>= 5;
    int bitCount = d & Bits_maxBits64(5);
    if (bitCount < 1) {
        return 0;
    }
    out->bitCount = bitCount;
    d >>= 5;
    out->prefix = d & Bits_maxBits64(out->prefixLen);
    return 5 + 5 + out->prefixLen;
}

static inline int encodeForm(struct EncodingScheme_Form* in, uint64_t* data, int bits)
{
    *data |= ((uint64_t)in->prefixLen & Bits_maxBits64(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->bitCount & Bits_maxBits64(5)) << bits;
    bits += 5;

    *data |= ((uint64_t)in->prefix & Bits_maxBits64(in->prefixLen)) << bits;

    return 5 + 5 + in->prefixLen;
}

bool EncodingScheme_isSane(const struct EncodingScheme* scheme)
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
        if (scheme->forms[0].prefixLen != 0 || scheme->forms[0].prefix != 0) {
            // prefixLen must be 0
            return false;
        }
        if (scheme->forms[0].bitCount == 0 || scheme->forms[0].bitCount > 31) {
            // bitcount must be non-zero and can't overflow the number
            return false;
        }
        return true;
    }

    // Variable width encoding.
    for (int i = 0; i < scheme->count; i++) {
        struct EncodingScheme_Form* form = &scheme->forms[i];

        if (form->prefixLen == 0 || form->prefixLen > 31) {
            // Prefix must exist in order to distinguish between forms
            return false;
        }
        if (form->bitCount == 0 || form->bitCount > 31) {
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
            if (j != i
                && (scheme->forms[j].prefix & Bits_maxBits64(form->prefixLen)) == form->prefix)
            {
                return false;
            }
        }
    }
    return true;
}

List* EncodingScheme_asList(const struct EncodingScheme* list, struct Allocator* alloc)
{
    Assert_ifParanoid(EncodingScheme_isSane(list));
    List* scheme = List_new(alloc);
    for (int i = (int)list->count - 1; i >= 0; i--) {
        Dict* form = Dict_new(alloc);
        Dict_putIntC(form, "prefixLen", list->forms[i].prefixLen, alloc);
        Dict_putIntC(form, "bitCount", list->forms[i].bitCount, alloc);
        if (list->forms[i].prefixLen == 0) {
            Dict_putStringCC(form, "prefix", "", alloc);
        } else {
            String* pfx = String_newBinary(NULL, 8, alloc);
            uint32_t prefix_be = Endian_hostToBigEndian32(list->forms[i].prefix);
            Hex_encode(pfx->bytes, 8, (uint8_t*)&prefix_be, 4);
            while (pfx->bytes[0] == '0' && pfx->len > 2) {
                pfx->bytes += 2;
                pfx->len -= 2;
            }
            Dict_putStringC(form, "prefix", pfx, alloc);
        }
        List_addDict(scheme, form, alloc);
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
        uint64_t* prefixLen = Dict_getIntC(form, "prefixLen");
        uint64_t* bitCount = Dict_getIntC(form, "bitCount");
        String* prefixStr = Dict_getStringC(form, "prefix");
        if (!prefixLen || !bitCount || !prefixStr || prefixStr->len != 8) {
            return NULL;
        }
        uint32_t prefix_be;
        if (Hex_decode((uint8_t*)&prefix_be, 4, prefixStr->bytes, 8) != 4) {
            return NULL;
        }
        list->forms[i].prefixLen = *prefixLen;
        list->forms[i].bitCount = *bitCount;
        list->forms[i].prefix = Endian_bigEndianToHost32(prefix_be);
    }
    if (!EncodingScheme_isSane(list)) {
        return NULL;
    }
    return list;
}

String* EncodingScheme_serialize(const struct EncodingScheme* list,
                                 struct Allocator* alloc)
{
    Assert_ifParanoid(EncodingScheme_isSane(list));

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

        Assert_true((next.prefix >> next.prefixLen) == 0);

        outCount += 1;
        forms = Allocator_realloc(alloc, forms, outCount * sizeof(struct EncodingScheme_Form));
        Bits_memcpy(&forms[outCount-1], &next, sizeof(struct EncodingScheme_Form));
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

    struct NumberCompress_FixedWidthScheme* out =
        Allocator_malloc(alloc, sizeof(struct NumberCompress_FixedWidthScheme));

    struct NumberCompress_FixedWidthScheme scheme = {
        .scheme = { .count = 1, .forms = &out->form },
        .form = { .bitCount = bitCount, .prefixLen = 0, .prefix = 0, },
    };
    Bits_memcpy(out, &scheme, sizeof(struct NumberCompress_FixedWidthScheme));

    Assert_true(EncodingScheme_isSane(&out->scheme));

    return &out->scheme;
}


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(const struct EncodingScheme_Form* forms,
                                                           int formCount,
                                                           struct Allocator* alloc)
{
    struct EncodingScheme_Form* formsCopy =
        Allocator_malloc(alloc, sizeof(struct EncodingScheme_Form) * formCount);
    Bits_memcpy(formsCopy, forms, sizeof(struct EncodingScheme_Form) * formCount);

    struct EncodingScheme* scheme = Allocator_clone(alloc, (&(struct EncodingScheme) {
        .count = formCount,
        .forms = formsCopy
    }));

    Assert_ifParanoid(EncodingScheme_isSane(scheme));
    return scheme;
}

int EncodingScheme_compare(const struct EncodingScheme* a, const struct EncodingScheme* b)
{
    if (a->count == b->count) {
        return Bits_memcmp(a->forms, b->forms, sizeof(struct EncodingScheme_Form) * a->count);
    }
    return a->count > b->count ? 1 : -1;
}

/**
 * Return true if the route is to the switch's router interface.
 */
int EncodingScheme_isSelfRoute(const struct EncodingScheme* scheme, uint64_t routeLabel)
{
    if (!EncodingScheme_is358(scheme)) { return routeLabel == 1; }
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == EncodingScheme_getFormNum_INVALID) {
        return 0;
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    return (routeLabel & Bits_maxBits64(currentForm->prefixLen + currentForm->bitCount)) == 1;
}

int EncodingScheme_isOneHop(const struct EncodingScheme* scheme, uint64_t routeLabel)
{
    int fn = EncodingScheme_getFormNum(scheme, routeLabel);
    if (fn == EncodingScheme_getFormNum_INVALID) { return 0; }
    struct EncodingScheme_Form* form = &scheme->forms[fn];
    return (Bits_log2x64(routeLabel) == form->prefixLen + form->bitCount);
}
