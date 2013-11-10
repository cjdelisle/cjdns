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
#ifndef ReplayProtector_H
#define ReplayProtector_H

#include "util/Bits.h"

#include <stdbool.h>

struct ReplayProtector
{
    /** internal bitfield. */
    uint64_t bitfield;

    /** Internal offset. */
    uint32_t baseOffset;

    /** Number of definite duplicate packets. */
    uint32_t duplicates;

    /** Number of lost packets. */
    uint32_t lostPackets;

    /**
     * Number of packets which could not be verified because they were out of range.
     * Growing lostPackets and receivedOutOfRange together indicate severe packet
     * reordering issues.
     * receivedOutOfRange growing along indicates duplicate packets.
     */
    uint32_t receivedOutOfRange;
};

static inline int RelayProtector_lostInShift(uint64_t bitfield, int shiftAmount)
{
    if (shiftAmount == 0) {
        return 0;
    }
    if (shiftAmount > 63) {
        return shiftAmount - Bits_popCountx64(bitfield);
    }
    return shiftAmount - Bits_popCountx64(bitfield << (64 - shiftAmount));
}

/**
 * Check a nonce and file it as being seen.
 * Don't call this until the packet has been authenticated or else forged packets will
 * make legit ones appear to be duplicates.
 *
 * @param nonce the number to check, this should be a counter nonce as numbers less than 32 minus
 *              the highest seen nonce will be dropped erroniously.
 * @param context the context
 * @return true if the packet is provably not a replay, otherwise false.
 */
static inline bool ReplayProtector_checkNonce(const uint32_t nonce, struct ReplayProtector* context)
{
    if (nonce < context->baseOffset) {
        context->receivedOutOfRange++;
        return false;
    }

    uint32_t offset = nonce - context->baseOffset;

    while (offset > 63) {

        #define ReplayProtector_DO_SHIFT(bits) \
            context->baseOffset += (bits);     \
            if ((bits) > 63) {                 \
                context->bitfield = 0;         \
            } else {                           \
                context->bitfield >>= (bits);  \
            }                                  \
            offset -= (bits);

        if ((context->bitfield & 0xffffffffu) == 0xffffffffu) {
            // happy path, low 32 bits are checked in, rotate and continue.
            ReplayProtector_DO_SHIFT(32);

        } else {
            // we are going to have to accept some losses, take offset - 47 to mitigate that
            // as much as possible.
            context->lostPackets += RelayProtector_lostInShift(context->bitfield, offset - 47);
            ReplayProtector_DO_SHIFT(offset - 47);
        }
    }

    if (context->bitfield & (((uint64_t)1) << offset)) {
        context->duplicates++;
        return false;
    }
    context->bitfield |= (((uint64_t)1) << offset);
    return true;
}

#endif
