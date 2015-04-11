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
#ifndef LabelSplicer_H
#define LabelSplicer_H

#include "util/Bits.h"

#include <stdint.h>
#include <stdbool.h>

/**
 * Convert a full path (represention we can use) to a representaiton which a node
 * along that path can use.
 *
 * @param fullPath the full route to the destination
 * @param midPath a path to a node which falls somewhere within fullPath
 * @return a version of fullPath which is sutible for use by node at midPath
 */
static inline uint64_t LabelSplicer_unsplice(uint64_t fullPath, uint64_t midPath)
{
    return fullPath >> Bits_log2x64(midPath);
}

/**
 * Splice a label and a label fragment together.
 *
 */
static inline uint64_t LabelSplicer_splice(uint64_t goHere, uint64_t viaHere)
{
    uint64_t log2ViaHere = Bits_log2x64(viaHere);

    if (Bits_log2x64(goHere) + log2ViaHere > 59) {
        // Too big, can't splice.
        return UINT64_MAX;
    }

    return ((goHere ^ 1) << log2ViaHere) ^ viaHere;
}

/**
 * Determine if the route to one node passes through another node.
 * Given:
 * 1. alice->bob->charlie->fred->bob
 * 2. alice->bob
 * 1 routes through 2.
 *
 * @param destination the node to route to.
 * @param midPath the node which might be in the middle of the route.
 * @return true if midPath is in the middle of the route to destination.
 */
static inline bool LabelSplicer_routesThrough(uint64_t destination, uint64_t midPath)
{
    if (midPath > destination) {
        return false;
    } if (midPath < 2) {
        return true;
    }
    uint64_t mask = UINT64_MAX >> (64 - Bits_log2x64(midPath));
    return (destination & mask) == (midPath & mask);
}

#endif
