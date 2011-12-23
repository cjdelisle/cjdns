#ifndef MESSAGE_H
#define MESSAGE_H

#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

#include "memory/MemAllocator.h"

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
                                            struct MemAllocator* allocator)
{
    uint8_t* allocation = allocator->malloc(toClone->length + toClone->padding, allocator);
    memcpy(allocation, toClone->bytes - toClone->padding, toClone->length + toClone->padding);
    return allocator->clone(sizeof(struct Message), allocator, &(struct Message) {
        .length = toClone->length,
        .padding = toClone->padding,
        .bytes = allocation + toClone->padding
    });
}

static inline void Message_copyOver(struct Message* output,
                                    struct Message* input,
                                    struct MemAllocator* allocator)
{
    size_t inTotalLength = input->length + input->padding;
    size_t outTotalLength = output->length + output->padding;
    uint8_t* allocation = output->bytes - output->padding;
    if (inTotalLength > outTotalLength) {
        allocation = allocator->realloc(allocation, inTotalLength, allocator);
    }
    memcpy(allocation, input->bytes - input->padding, inTotalLength);
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
    assert(toShift->padding >= amount);
    assert((amount >= 0) ? (UINT16_MAX - toShift->length >= amount) : (toShift->length >= -amount));
assert(toShift->length < 60000);
    toShift->length += amount;
    toShift->bytes -= amount;
    toShift->padding -= amount;
assert(toShift->length < 60000);
    return true;
}

#endif
