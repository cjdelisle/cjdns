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

#include <stdint.h>

#define LinkState_SLOTS 18

struct LinkState {
    uint16_t lagSlots[LinkState_SLOTS];
    uint16_t dropSlots[LinkState_SLOTS];
    uint32_t kbRecvSlots[LinkState_SLOTS];
    uint32_t samples;
};
/*
struct LinkState_AllPeers {

};

static int LinkState_encode(struct Message* msg, struct LinkState* ls, int lastSamples)
{
    struct VarInt_Iter iter = {
        .ptr = msg->bytes,
        .end = msg->bytes,
        .start = &msg->bytes[-msg->padding]
    };
    if (iter.end - iter.start > 255) { iter.start = &iter.end[-255]; }

    int startingPoint = ls->samples - lastSamples;

    int err = 0;
    for (int i = 0; i < LinkState_SLOTS; i++) { err |= VarInt_push(&iter, ls->lagSlots[i]); }
    for (int i = 0; i < LinkState_SLOTS; i++) { err |= VarInt_push(&iter, ls->dropSlots[i]); }
    for (int i = 0; i < LinkState_SLOTS; i++) { err |= VarInt_push(&iter, ls->kbRecvSlots[i]); }
    err |= VarInt_push(&iter, ls->samples);
    err |= VarInt_push(&iter, 0xfc);
    while ((((uintptr_t)iter.ptr) & 7) != 2) { err |= VarInt_push(&iter, 0); }
    if (iter.end - iter.ptr) { }
    VarInt_push(&iter, 0);
    Announce_Type_LINK_STATE
    err |= VarInt_push(&iter, 0xfc);
    if (!err) {
        Message_shift(msg, iter.end - iter.ptr )
    }

}
*/

#endif