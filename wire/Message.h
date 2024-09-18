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

#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Linker.h"
Linker_require("wire/Message.c")

#include <stdint.h>

typedef struct Message
{
    /** The length of the message. */
    int32_t _length;

    /** The number of bytes of padding BEFORE where bytes begins. */
    int32_t _padding;

    /** The content. */
    uint8_t* _msgbytes;

    /** Amount of bytes of storage space available in the message. */
    int32_t _capacity;

    // Amount of associated data
    int32_t _adLen;

    // Pointer to associated data
    uint8_t* _ad;

    /**
     * When sending/receiving a Message on a unix socket, a file descriptor to attach.
     * Caviat: In order to maintain backward compatibility with a Message which is
     * allocated using calloc, file descriptor 0 is referred to by -1
     */
    int _associatedFd;

    #ifdef PARANOIA
        /** This is used inside of Iface.h to support Iface_next() */
        struct Iface* currentIface;
    #endif

    /** The allocator which allocated space for this message. */
    struct Allocator* _alloc;
} Message_t;

static inline uint8_t* Message_bytes(struct Message* msg)
{
    return msg->_msgbytes;
}

static inline struct Allocator* Message_getAlloc(struct Message* msg)
{
    return msg->_alloc;
}

static inline int32_t Message_getLength(struct Message* msg)
{
    return msg->_length;
}

static inline Err_DEFUN Message_truncate(struct Message* msg, int32_t newLen)
{
    if (newLen > msg->_length) {
        Err_raise(msg->_alloc, "Message_truncate(%d) message length is %d", newLen, msg->_length);
    }
    msg->_length = newLen;
    return NULL;
}

static inline int32_t Message_getPadding(struct Message* msg)
{
    return msg->_padding;
}

static inline int32_t Message_getCapacity(struct Message* msg)
{
    return msg->_capacity;
}

struct Message* Message_new(uint32_t messageLength,
                                          uint32_t amountOfPadding,
                                          struct Allocator* alloc);

struct Message* Message_new_fromRust(uint32_t messageLength,
                                          uint32_t amountOfPadding,
                                          struct Allocator* alloc);

void Message_setAssociatedFd(struct Message* msg, int fd);

int Message_getAssociatedFd(struct Message* msg);

struct Message* Message_clone(struct Message* toClone, struct Allocator* alloc);

static inline Err_DEFUN Message_peakBytes(uint8_t** out, struct Message* msg, int32_t len)
{
    if (len > msg->_length) {
        Err_raise(msg->_alloc, "peakBytes(%d) too big, message length is %d", len, msg->_length);
    }
    *out = msg->_msgbytes;
    return NULL;
}

/**
 * Pretend to shift the content forward by amount.
 * Really it shifts the bytes value backward.
 */
static inline Err_DEFUN Message_eshift(struct Message* toShift, int32_t amount)
{
    if (amount > 0 && toShift->_padding < amount) {
        Err_raise(toShift->_alloc, "buffer overflow adding %d to length %d",
            amount, toShift->_length);
    } else if (toShift->_length < (-amount)) {
        Err_raise(toShift->_alloc, "buffer underflow");
    }

    toShift->_length += amount;
    toShift->_capacity += amount;
    toShift->_msgbytes -= amount;
    toShift->_padding -= amount;

    return NULL;
}

static inline Err_DEFUN Message_epushAd(struct Message* restrict msg,
                                            const void* restrict object,
                                            size_t size)
{
    if (msg->_padding < (int)size) {
        Err_raise(msg->_alloc, "not enough padding to push ad");
    }
    if (object) {
        Bits_memcpy(msg->_ad, object, size);
    } else {
        Bits_memset(msg->_ad, 0x00, size);
    }
    msg->_adLen += size;
    msg->_padding -= size;
    msg->_ad = &msg->_ad[size];
    return NULL;
}

static inline Err_DEFUN Message_epopAd(struct Message* restrict msg,
                                           void* restrict object,
                                           size_t size)
{
    if (msg->_adLen < (int)size) {
        Err_raise(msg->_alloc, "underflow, cannot pop ad");
    }
    msg->_adLen -= size;
    msg->_padding += size;
    msg->_ad = &msg->_ad[-((int)size)];
    if (object) {
        Bits_memcpy(object, msg->_ad, size);
    }
    return NULL;
}

static inline void Message_reset(struct Message* toShift)
{
    Assert_true(toShift->_length <= toShift->_capacity);
    Err_assert(Message_epopAd(toShift, NULL, toShift->_adLen));
    toShift->_length = toShift->_capacity;
    Err_assert(Message_eshift(toShift, -toShift->_length));
}

static inline Err_DEFUN Message_epush(struct Message* restrict msg,
                                          const void* restrict object,
                                          size_t size)
{
    Err(Message_eshift(msg, (int)size));
    if (object) {
        Bits_memcpy(msg->_msgbytes, object, size);
    } else {
        Bits_memset(msg->_msgbytes, 0x00, size);
    }
    return NULL;
}

static inline Err_DEFUN Message_epop(struct Message* restrict msg,
                                         void* restrict object,
                                         size_t size)
{
    Err(Message_eshift(msg, -(int)size));
    if (object) {
        Bits_memcpy(object, &msg->_msgbytes[-((int)size)], size);
    }
    return NULL;
}

#define Message_pushH(size) \
    static inline Err_DEFUN Message_epush ## size ## h                                \
        (struct Message* msg, uint ## size ## _t dat)                                    \
    {                                                                                     \
        Err(Message_epush(msg, &dat, (size)/8));                                           \
        return NULL; \
    }
#define Message_popH(size) \
    static inline Err_DEFUN Message_epop ## size ## h(uint ## size ## _t* out, struct Message* msg)     \
    {                                                                                             \
        Err(Message_epop(msg, out, (size)/8));                                                    \
        return NULL; \
    }

#define Message_popBE(size) \
    static inline Err_DEFUN Message_epop ## size ## be(uint ## size ## _t* out, struct Message* msg) \
    {                                                                                             \
        Err(Message_epop ## size ## h(out, msg));                              \
        *out = Endian_bigEndianToHost ## size (*out);                           \
        return NULL;                                                                             \
    }

#define Message_popLE(size) \
    static inline Err_DEFUN Message_epop ## size ## le(uint ## size ## _t* out, struct Message* msg) \
    {                                                                                             \
        Err(Message_epop ## size ## h(out, msg));                              \
        *out = Endian_littleEndianToHost ## size (*out);                        \
        return NULL;                                                                             \
    }

#define Message_pushPop(size) \
    Message_pushH(size) Message_popH(size) Message_popBE(size) Message_popLE(size)

Message_pushH(8)
Message_popH(8)

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
