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
#ifndef LinkState_H
#define LinkState_H

#include "util/VarInt.h"
#include "wire/Message.h"
#include "wire/Announce.h"

#include <stdint.h>
#include <stdbool.h>

#define LinkState_SLOTS 18

struct LinkState {
    uint16_t lagSlots[LinkState_SLOTS];
    uint16_t dropSlots[LinkState_SLOTS];
    uint32_t kbRecvSlots[LinkState_SLOTS];
    uint32_t samples;
    uint16_t nodeId;
    uint16_t _pad;
};

static inline int LinkState_encode(
    Message_t* msg,
    struct LinkState* ls,
    uint32_t lastSamples)
{
    // Only encode the message if there is at least 255 bytes of headspace
    // We can then encode as many as possible and finally pop one which spills over the
    // size limit and then encode it again in the next message.
    if (Message_getPadding(msg) < 255) { return 1; }

    struct VarInt_Iter iter = {
        .ptr = Message_bytes(msg),
        .end = Message_bytes(msg),
        .start = &Message_bytes(msg)[-Message_getPadding(msg)]
    };

    // Take the newest X entries where X = MIN(ls->samples - lastSamples, LinkState_SLOTS)
    uint32_t i = ls->samples - 1;
    int count = 0;
    int err = 0;
    for (; i >= lastSamples && count < LinkState_SLOTS; i--, count++) {
        int idx = i % LinkState_SLOTS;
        err |= VarInt_push(&iter, ls->kbRecvSlots[idx]);
        err |= VarInt_push(&iter, ls->dropSlots[idx]);
        err |= VarInt_push(&iter, ls->lagSlots[idx]);
    }
    if (err) { return 1; }
    if (!count) { return 0; }

    // index of the first slot which should be updated when parsing
    Assert_true(!VarInt_push(&iter, (i + 1) % LinkState_SLOTS));
    Assert_true(!VarInt_push(&iter, ls->nodeId));

    int beginLength = Message_getLength(msg);
    Err_assert(Message_eshift(msg, (Message_bytes(msg) - iter.ptr)));
    Assert_true(Message_bytes(msg) == iter.ptr);

    int padCount = 0;
    while ((uintptr_t)(&Message_bytes(msg)[-3]) & 7) {
        Err_assert(Message_epush8(msg, 0));
        padCount++;
    }
    Err_assert(Message_epush8(msg, padCount));

    Err_assert(Message_epush8(msg, Announce_Type_LINK_STATE));
    int finalLength = Message_getLength(msg) - beginLength;
    Err_assert(Message_epush8(msg, finalLength + 1));

    Assert_true(!(((uintptr_t)Message_bytes(msg)) & 7));
    return 0;
}

static inline int LinkState_mkDecoder(Message_t* msg, struct VarInt_Iter* it)
{
    if (!Message_getLength(msg)) { return 1; }
    uint8_t len = Message_bytes(msg)[0];
    if (Message_getLength(msg) < len) { return 1; }
    if (len < 3) { return 1; }
    it->ptr = &Message_bytes(msg)[1];
    it->start = it->ptr;
    it->end = &Message_bytes(msg)[len];
    // Ok to pop this using VarInt because it's supposed to be 3, which is less than 253
    uint64_t type = 0;
    if (VarInt_pop(it, &type)) { return 1; }
    if (type != Announce_Type_LINK_STATE) { return 1; }
    // Should be < 8
    uint64_t padCount = 0;
    if (VarInt_pop(it, &padCount)) { return 1; }
    for (unsigned int i = 0; i < padCount; i++) {
        if (VarInt_pop(it, NULL)) { return 1; }
    }
    return 0;
}

static inline int LinkState_getNodeId(const struct VarInt_Iter* iter, uint32_t* nodeId)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    uint64_t id;
    if (VarInt_pop(&it, &id)) { return 1; }
    *nodeId = id;
    return 0;
}

#define LinkState_POP(it) (__extension__ ({ \
    uint64_t x; \
    if (VarInt_pop((it), &x)) { return 1; } \
    x; \
}))

static inline int LinkState_decode(const struct VarInt_Iter* iter, struct LinkState* ls)
{
    struct VarInt_Iter it;
    VarInt_clone(&it, iter);
    ls->nodeId = LinkState_POP(&it);
    uint32_t i = LinkState_POP(&it);
    uint32_t count = 0;
    if (i >= LinkState_SLOTS) { return 1; }
    for (;;) {
        if (it.ptr == it.end) { break; }
        ls->lagSlots[i] = LinkState_POP(&it);
        ls->dropSlots[i] = LinkState_POP(&it);
        ls->kbRecvSlots[i] = LinkState_POP(&it);
        i = (i + 1) % LinkState_SLOTS;
        count++;
    }
    ls->samples += count;
    return 0;
}

#endif