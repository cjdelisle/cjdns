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

#include <stdint.h>
#include <stdbool.h>

struct ReplayProtector
{
    uint32_t baseOffset;
    uint32_t bitfield;
};

/**
 * Check a nonce and file it as being seen.
 * Don't call this until the packet has been authenticated or else forged packets will
 * make legit ones appear to be duplicates.
 *
 * @param nonce the number to check, this should be a counter nonce as numbers less than 20 minus
 *              the highest seen nonce will be dropped erroniously.
 * @param context the context
 * @return true if the packet is provably not a replay, otherwise false.
 */
static inline bool ReplayProtector_checkNonce(const uint32_t nonce, struct ReplayProtector* context)
{
    if (nonce < context->baseOffset) {
        return false;
    }

    uint32_t offset = nonce - context->baseOffset;

    if (offset > 20) {
        context->baseOffset += offset - 20;
        context->bitfield = ((offset > 51) ? 0 : context->bitfield >> (offset - 20)) | (1 << 20);
        return true;
    }

    if (context->bitfield & (1 << offset)) {
        return false;
    }
    context->bitfield |= (1 << offset);
    return true;
}

#endif
