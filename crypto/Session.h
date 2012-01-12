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
#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <stdbool.h>

struct Session
{
    /** The symmetrical key. */
    uint8_t sharedSecret[32];

    /** the next nonce to use when sending. */
    uint32_t nextNonce;

    /** true if this node initiated the session. */
    bool isInitiator : 1;

    /** If true, poly1305 authentication must be run on every packet. */
    bool authenticatePackets : 1;

    /** true if this is a session and not just empty space. */
    bool exists : 1;
};

#endif
