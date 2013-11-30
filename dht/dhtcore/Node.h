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
#ifndef Node_H
#define Node_H

#include "dht/Address.h"

#include <stdint.h>
#include <stdbool.h>

/** A network address for reaching a peer, in the format which is sent over the wire. */
struct Node
{
    /**
     * The reach of the node (how big/fast/close it is).
     * Since reach is a fraction, the reach number represents a percentage where 0xFFFFFFFF = 100%
     */
    uint32_t reach;

    /** The version of the node, must be synchronized with NodeHeader */
    uint32_t version;

    /** The address of the node. */
    struct Address address;

    /**
     * If we lookup a node and the current time is later than this, ping it.
     * In ms, as per Time_currentTimeMilliseconds.
     */
    uint64_t timeOfNextPing;

    /**
     * Used to count the number of consecutive missed pings when testing reach.
     * Not allowing 1 or 2 before penalizing was causing us to switch paths too often,
     * leading to latency spikes.
     */
    uint8_t missedPings;
};

#endif
