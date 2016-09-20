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
#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "switch/EncodingScheme.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"

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

static bool is358(struct EncodingScheme* scheme)
{
    struct EncodingScheme_Form v358[3] = {
        { .bitCount = 3, .prefixLen = 1, .prefix = 1, },
        { .bitCount = 5, .prefixLen = 2, .prefix = 1<<1, },
        { .bitCount = 8, .prefixLen = 2, .prefix = 0, }
    };
    if (scheme->count != 3) { return false; }
    for (int i = 0; i < 3; i++) {
        if (Bits_memcmp(&v358[i], &scheme->forms[i], sizeof(struct EncodingScheme_Form))) {
            return false;
        }
    }
    return true;
}

uint64_t EncodingScheme_convertLabel(struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo)
{
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == EncodingScheme_getFormNum_INVALID) {
        return EncodingScheme_convertLabel_INVALID;
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    if (scheme->count == 1
        || (routeLabel & Bits_maxBits64(currentForm->prefixLen + currentForm->bitCount)) == 1)
    {
        // fixed width encoding or it's a self label, this is easy
        switch (convertTo) {
            case 0:
            case EncodingScheme_convertLabel_convertTo_CANNONICAL: return routeLabel;
            default: return EncodingScheme_convertLabel_INVALID;
        }
    }

    routeLabel >>= currentForm->prefixLen;
    uint64_t director = routeLabel & Bits_maxBits64(currentForm->bitCount);
    routeLabel >>= currentForm->bitCount;

    // ACKTUNG: Magic afoot!
    // Conversions are necessary for two reasons.
    // #1 ensure 0001 always references interface 1, the self interface.
    // #2 reuse interface the binary encoding for interface 1 in other EncodingForms
    //    because interface 1 cannot be expressed as anything other than 0001
    if (!is358(scheme)) {
        // don't pull this bug-workaround crap for sane encodings schemes.
    } else if ((currentForm->prefix & Bits_maxBits64(currentForm->prefixLen)) == 1) {
        // Swap 0 and 1 if the prefix is 1, this makes 0001 alias to 1
        // because 0 can never show up in the wild, we reuse it for 1.
        Assert_true(director != 0);
        if (director == 1) { director--; }
    } else if (director) {
        // Reuse the number 1 for 2 and 2 for 3 etc. to gain an extra slot in all other encodings.
        director++;
    }

    if (convertTo == EncodingScheme_convertLabel_convertTo_CANNONICAL) {
        // Take into account the fact that if the destination form does not have a 1 prefix,
        // an extra number will be available.
        int minBitsA = Bits_log2x64(director) + 1;
        int minBitsB = Bits_log2x64(director-1) + 1;
        for (int i = 0; i < scheme->count; i++) {
            struct EncodingScheme_Form* form = &scheme->forms[i];
            int minBits = ((form->prefix & Bits_maxBits64(form->prefixLen)) == 1)
                ? minBitsA : minBitsB;
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

    if (!is358(scheme)) {
        // don't pull this bug-workaround crap for sane encodings schemes.
    } else  if ((nextForm->prefix & Bits_maxBits64(nextForm->prefixLen)) == 1) {
        // Swap 1 and 0 back if necessary.
        if (director == 0) { director++; }
    } else if (director) {
        // Or move the numbers down by one.
        director--;
    }

    if ((Bits_log2x64(director) + 1) > nextForm->bitCount) {
        // won't fit in requested form
        return EncodingScheme_convertLabel_INVALID;
    }
    if (Bits_log2x64(routeLabel) + EncodingScheme_formSize(nextForm) > 59) {
        return EncodingScheme_convertLabel_INVALID;
    }

    routeLabel <<= nextForm->bitCount;
    routeLabel |= director;
    routeLabel <<= nextForm->prefixLen;
    routeLabel |= nextForm->prefix;

    if ((routeLabel & Bits_maxBits64(nextForm->prefixLen + nextForm->bitCount)) == 1) {
        // looks like a self-route
        return EncodingScheme_convertLabel_INVALID;
    }

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

List* EncodingScheme_asList(struct EncodingScheme* list, struct Allocator* alloc)
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
        list->forms[i].prefix = Endian_bigEndianToHost32(prefix_be);
    }
    if (!EncodingScheme_isSane(list)) {
        return NULL;
    }
    return list;
}

String* EncodingScheme_serialize(struct EncodingScheme* list,
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


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(struct EncodingScheme_Form* forms,
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

int EncodingScheme_compare(struct EncodingScheme* a, struct EncodingScheme* b)
{
    if (a->count == b->count) {
        return Bits_memcmp(a->forms, b->forms, sizeof(struct EncodingScheme_Form) * a->count);
    }
    return a->count > b->count ? 1 : -1;
}

/**
 * Return true if the route is to the switch's router interface.
 */
int EncodingScheme_isSelfRoute(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    int formNum = EncodingScheme_getFormNum(scheme, routeLabel);
    if (formNum == EncodingScheme_getFormNum_INVALID) {
        return 0;
    }

    struct EncodingScheme_Form* currentForm = &scheme->forms[formNum];

    return (routeLabel & Bits_maxBits64(currentForm->prefixLen + currentForm->bitCount)) == 1;
}

int EncodingScheme_isOneHop(struct EncodingScheme* scheme, uint64_t routeLabel)
{
    int fn = EncodingScheme_getFormNum(scheme, routeLabel);
    if (fn == EncodingScheme_getFormNum_INVALID) { return 0; }
    struct EncodingScheme_Form* form = &scheme->forms[fn];
    return (Bits_log2x64(routeLabel) == form->prefixLen + form->bitCount);
}
