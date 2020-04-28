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
#ifndef Message_H
#define Message_H

#include "exception/Er.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/UniqueName.h"

#include <stdint.h>

struct Message
{
    /** The length of the message. */
    int32_t length;

    /** The number of bytes of padding BEFORE where bytes begins. */
    int32_t padding;

    /** The content. */
    uint8_t* bytes;

    /** Amount of bytes of storage space available in the message. */
    int32_t capacity;

    /**
     * When sending/receiving a Message on a unix socket, a file descriptor to attach.
     * Caviat: In order to maintain backward compatibility with a Message which is
     * allocated using calloc, file descriptor 0 is referred to by -1
     */
    int associatedFd;

    #ifdef PARANOIA
        /** This is used inside of Iface.h to support Iface_next() */
        struct Iface* currentIface;
    #endif

    /** The allocator which allocated space for this message. */
    struct Allocator* alloc;
};

#define Message_STACK(name, messageLength, amountOfPadding) \
    uint8_t UniqueName_get()[messageLength + amountOfPadding]; \
    name = &(struct Message){                                  \
        .length = messageLength,                               \
        .bytes = UniqueName_last() + amountOfPadding,          \
        .padding = amountOfPadding,                            \
        .capacity = messageLength                              \
    }

static inline struct Message* Message_new(uint32_t messageLength,
                                          uint32_t amountOfPadding,
                                          struct Allocator* alloc)
{
    uint8_t* buff = Allocator_malloc(alloc, messageLength + amountOfPadding);
    struct Message* out = Allocator_calloc(alloc, sizeof(struct Message), 1);
    out->bytes = &buff[amountOfPadding];
    out->length = out->capacity = messageLength;
    out->padding = amountOfPadding;
    out->alloc = alloc;
    return out;
}

static inline void Message_setAssociatedFd(struct Message* msg, int fd)
{
    if (fd == -1) {
        msg->associatedFd = 0;
    } else if (fd == 0) {
        msg->associatedFd = -1;
    } else {
        msg->associatedFd = fd;
    }
}

static inline int Message_getAssociatedFd(struct Message* msg)
{
    if (msg->associatedFd == -1) {
        return 0;
    } else if (msg->associatedFd == 0) {
        return -1;
    } else {
        return msg->associatedFd;
    }
}

static inline struct Message* Message_clone(struct Message* toClone, struct Allocator* alloc)
{
    Assert_true(toClone->capacity >= toClone->length);
    int32_t len = toClone->capacity + toClone->padding;
    uint8_t* allocation = Allocator_malloc(alloc, len + 8);
    while (((uintptr_t)allocation % 8) != (((uintptr_t)toClone->bytes - toClone->padding) % 8)) {
        allocation++;
    }
    Bits_memcpy(allocation, toClone->bytes - toClone->padding, len);
    return Allocator_clone(alloc, (&(struct Message) {
        .length = toClone->length,
        .padding = toClone->padding,
        .bytes = allocation + toClone->padding,
        .capacity = toClone->capacity,
        .alloc = alloc
    }));
}

static inline void Message_copyOver(struct Message* output,
                                    struct Message* input,
                                    struct Allocator* allocator)
{
    size_t inTotalLength = input->length + input->padding;
    size_t outTotalLength = output->length + output->padding;
    uint8_t* allocation = output->bytes - output->padding;
    if (inTotalLength > outTotalLength) {
        allocation = Allocator_realloc(allocator, allocation, inTotalLength);
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
static inline int Message_shift(struct Message* toShift, int32_t amount, struct Except* eh)
{
    if (amount > 0 && toShift->padding < amount) {
        Except_throw(eh, "buffer overflow adding %d to length %d", amount, toShift->length);
    } else if (toShift->length < (-amount)) {
        Except_throw(eh, "buffer underflow");
    }

    toShift->length += amount;
    toShift->capacity += amount;
    toShift->bytes -= amount;
    toShift->padding -= amount;

    return 1;
}

static inline Er_DEFUN(void Message_eshift(struct Message* toShift, int32_t amount))
{
    if (amount > 0 && toShift->padding < amount) {
        Er_raise(toShift->alloc, "buffer overflow adding %d to length %d",
            amount, toShift->length);
    } else if (toShift->length < (-amount)) {
        Er_raise(toShift->alloc, "buffer underflow");
    }

    toShift->length += amount;
    toShift->capacity += amount;
    toShift->bytes -= amount;
    toShift->padding -= amount;

    Er_ret();
}

static inline void Message_reset(struct Message* toShift)
{
    Assert_true(toShift->length <= toShift->capacity);
    toShift->length = toShift->capacity;
    Message_shift(toShift, -toShift->length, NULL);
}

static inline Er_DEFUN(void Message_epush(struct Message* restrict msg,
                                          const void* restrict object,
                                          size_t size))
{
    Er(Message_eshift(msg, (int)size));
    if (object) {
        Bits_memcpy(msg->bytes, object, size);
    } else {
        Bits_memset(msg->bytes, 0x00, size);
    }
    Er_ret();
}

static inline void Message_push(struct Message* restrict msg,
                                const void* restrict object,
                                size_t size,
                                struct Except* eh)
{
    Message_shift(msg, (int)size, eh);
    if (object) {
        Bits_memcpy(msg->bytes, object, size);
    } else {
        Bits_memset(msg->bytes, 0x00, size);
    }
}

static inline void Message_pop(struct Message* restrict msg,
                               void* restrict object,
                               size_t size,
                               struct Except* eh)
{
    Message_shift(msg, -((int)size), eh);
    if (object) {
        Bits_memcpy(object, &msg->bytes[-((int)size)], size);
    }
}

static inline Er_DEFUN(void Message_epop(struct Message* restrict msg,
                                         void* restrict object,
                                         size_t size))
{
    Er(Message_eshift(msg, -(int)size));
    if (object) {
        Bits_memcpy(object, &msg->bytes[-((int)size)], size);
    }
    Er_ret();
}

#define Message_pushH(size) \
    static inline void Message_push ## size ## h                                          \
        (struct Message* msg, uint ## size ## _t dat, struct Except* eh)                  \
    {                                                                                     \
        Message_push(msg, &dat, (size)/8, eh);                                            \
    } \
    static inline Er_DEFUN(void Message_epush ## size ## h                                \
        (struct Message* msg, uint ## size ## _t dat))                                    \
    {                                                                                     \
        Er(Message_epush(msg, &dat, (size)/8));                                           \
        Er_ret(); \
    }
#define Message_popH(size) \
    static inline uint ## size ## _t Message_pop ## size ## h \
        (struct Message* msg, struct Except* eh) \
    {                                                                                             \
        uint ## size ## _t out;                                                                   \
        Message_pop(msg, &out, (size)/8, eh);                                                     \
        return out;                                                                               \
    } \
    static inline Er_DEFUN(uint ## size ## _t Message_epop ## size ## h(struct Message* msg))     \
    {                                                                                             \
        uint ## size ## _t out;                                                                   \
        Er(Message_epop(msg, &out, (size)/8));                                                    \
        Er_ret(out); \
    }

#define Message_pushBE(size) \
    static inline void Message_push ## size \
            (struct Message* msg, uint ## size ## _t dat, struct Except* eh)                      \
    {                                                                                             \
        Message_push ## size ## h(msg, Endian_hostToBigEndian ## size (dat), eh);                 \
    }
#define Message_pushLE(size) \
    static inline void Message_push ## size ## le \
            (struct Message* msg, uint ## size ## _t dat, struct Except* eh)                      \
    {                                                                                             \
        Message_push ## size ## h(msg, Endian_hostToLittleEndian ## size (dat), eh);              \
    }
#define Message_popBE(size) \
    static inline uint ## size ## _t Message_pop ## size (struct Message* msg, struct Except* eh) \
    {                                                                                             \
        return Endian_bigEndianToHost ## size (Message_pop ## size ## h(msg, eh));                \
    } \
    static inline Er_DEFUN(uint ## size ## _t Message_epop ## size ## be(struct Message* msg)) \
    {                                                                                             \
        uint ## size ## _t out = Er(Message_epop ## size ## h(msg));                              \
        uint ## size ## _t out1 = Endian_bigEndianToHost ## size (out);                           \
        Er_ret(out1);                                                                             \
    }
#define Message_popLE(size) \
    static inline uint ## size ## _t Message_pop ## size ## le \
        (struct Message* msg, struct Except* eh) \
    {                                                                                             \
        return Endian_littleEndianToHost ## size (Message_pop ## size ## h(msg, eh));             \
    } \
    static inline Er_DEFUN(uint ## size ## _t Message_epop ## size ## le(struct Message* msg)) \
    {                                                                                             \
        uint ## size ## _t out = Er(Message_epop ## size ## h(msg));                              \
        uint ## size ## _t out1 = Endian_littleEndianToHost ## size (out);                        \
        Er_ret(out1);                                                                             \
    }

#define Message_pushPop(size) \
    Message_pushH(size) Message_popH(size) \
    Message_pushBE(size) Message_popBE(size) \
    Message_pushLE(size) Message_popLE(size)

Message_pushH(8)
Message_popH(8)
#define Message_push8 Message_push8h
#define Message_pop8 Message_pop8h

#define Message_epush8 Message_epush8h
#define Message_epop8 Message_epop8h

Message_pushPop(16)
Message_pushPop(32)
Message_pushPop(64)

#define Message_epush16le(msg, x) Message_epush16h(msg, Endian_hostToLittleEndian16(x))
#define Message_epush32le(msg, x) Message_epush16h(msg, Endian_hostToLittleEndian32(x))
#define Message_epush64le(msg, x) Message_epush16h(msg, Endian_hostToLittleEndian64(x))

#define Message_epush16be(msg, x) Message_epush16h(msg, Endian_hostToBigEndian16(x))
#define Message_epush32be(msg, x) Message_epush32h(msg, Endian_hostToBigEndian32(x))
#define Message_epush64be(msg, x) Message_epush64h(msg, Endian_hostToBigEndian64(x))

#endif
