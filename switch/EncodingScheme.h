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
#ifndef EncodingScheme_H
#define EncodingScheme_H

#include "benc/String.h"
#include "benc/List.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("switch/EncodingScheme.c");

#include <stdint.h>

struct EncodingScheme_Form
{
    // these only require 5 bits each but use 16 to avoid compiler injected padding
    // which can screw up comparison by memcmp()
    uint16_t bitCount;
    uint16_t prefixLen;
    uint32_t prefix;
};

struct EncodingScheme
{
    struct EncodingScheme_Form* forms;
    int count;
};

/**
 * Get the number of the encoding form used to encode the first director in the given label.
 * If there is no matching prefix for any of the forms then EncodingScheme_getFormNum_INVALID is
 * returned.
 */
#define EncodingScheme_getFormNum_INVALID -1
int EncodingScheme_getFormNum(struct EncodingScheme* scheme, uint64_t routeLabel);

/**
 * Convert the first director in a label from it's current form to the form given by convertTo.
 * convertTo must be between zero and scheme->count unless you are asking for the cannonical form
 * IE: the smallest possible represenation in which case convertTo should be:
 * EncodingScheme_convertLabel_convertTo_CANNONICAL
 *
 * The return value will be the converted label unless conversion is not possible because the
 * format of the label is invalid, the format of the label does not match the scheme, or the
 * label cannot be converted without overrunning the acceptable label size in which case
 * EncodingScheme_convertLabel_INVALID will be returned.
 */
#define EncodingScheme_convertLabel_convertTo_CANNONICAL (-5000)
#define EncodingScheme_convertLabel_INVALID (~((uint64_t)0))
uint64_t EncodingScheme_convertLabel(struct EncodingScheme* scheme,
                                     uint64_t routeLabel,
                                     int convertTo);

/**
 * Check that the given encoding scheme is conformant to protocol.
 * This will not check some things which are impossible to serialize (will never come from a
 * working deserializer) such as unrepresentable bit-widths but it will check that a representable
 * encoding scheme is indeed valid according to the protocol.
 */
bool EncodingScheme_isSane(struct EncodingScheme* scheme);

String* EncodingScheme_serialize(struct EncodingScheme* list,
                                 struct Allocator* alloc);

struct EncodingScheme* EncodingScheme_deserialize(String* data,
                                                  struct Allocator* alloc);

struct EncodingScheme* EncodingScheme_defineFixedWidthScheme(int bitCount, struct Allocator* alloc);


struct EncodingScheme* EncodingScheme_defineDynWidthScheme(struct EncodingScheme_Form* forms,
                                                           int formCount,
                                                           struct Allocator* alloc);

#define EncodingScheme_clone(scheme, alloc) \
    EncodingScheme_defineDynWidthScheme((scheme)->forms, (scheme)->count, (alloc))

static inline int EncodingScheme_formSize(const struct EncodingScheme_Form* form)
{
    return form->bitCount + form->prefixLen;
}

int EncodingScheme_compare(struct EncodingScheme* a, struct EncodingScheme* b);

#define EncodingScheme_equals(a,b) (!EncodingScheme_compare(a,b))

struct EncodingScheme* EncodingScheme_fromList(List* scheme, struct Allocator* alloc);
List* EncodingScheme_asList(struct EncodingScheme* list, struct Allocator* alloc);

/**
 * Return true if the route is to the switch's router interface.
 */
int EncodingScheme_isSelfRoute(struct EncodingScheme* scheme, uint64_t routeLabel);

/**
 * @return non-zero if the route label is one hop.
 */
int EncodingScheme_isOneHop(struct EncodingScheme* scheme, uint64_t routeLabel);

#endif
