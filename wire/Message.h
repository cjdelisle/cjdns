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
#ifndef Message_H
#define Message_H

#include "util/Assert.h"
#include <stdbool.h>
#include <stdint.h>

#include "memory/Allocator.h"
#include "util/Bits.h"

struct Message
{
    /** The length of the message. */
    uint16_t length;

    /** The number of bytes of padding BEFORE where bytes begins. */
    uint16_t padding;

    /** The content. */
    uint8_t* bytes;
};

static inline struct Message* Message_clone(struct Message* toClone,
                                            struct Allocator* allocator)
{
    uint8_t* allocation = allocator->malloc(toClone->length + toClone->padding, allocator);
    Bits_memcpy(allocation, toClone->bytes - toClone->padding, toClone->length + toClone->padding);
    return allocator->clone(sizeof(struct Message), allocator, &(struct Message) {
        .length = toClone->length,
        .padding = toClone->padding,
        .bytes = allocation + toClone->padding
    });
}

static inline void Message_copyOver(struct Message* output,
                                    struct Message* input,
                                    struct Allocator* allocator)
{
    size_t inTotalLength = input->length + input->padding;
    size_t outTotalLength = output->length + output->padding;
    uint8_t* allocation = output->bytes - output->padding;
    if (inTotalLength > outTotalLength) {
        allocation = allocator->realloc(allocation, inTotalLength, allocator);
    }
    Bits_memcpy(allocation, input->bytes - input->padding, inTotalLength);
    output->bytes = allocation + input->padding;
    output->length = input->length;
    output->padding = input->padding;
}

/**
 * Pretend to shift the content forward by amount.
 * Really it shifts the bytes value backward.
 */
static inline bool Message_shift(struct Message* toShift, int32_t amount)
{
    Assert_true(toShift->padding >= amount);
    Assert_true((amount >= 0) ? (UINT16_MAX - toShift->length >= amount) : (toShift->length >= -amount));
Assert_true(toShift->length < 60000);
    toShift->length += amount;
    toShift->bytes -= amount;
    toShift->padding -= amount;
Assert_true(toShift->length < 60000);
    return true;
}

#endif
