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
#ifndef PeerLink_H
#define PeerLink_H

#include "wire/Message.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("net/PeerLink.c");

#include <stdbool.h>

/**
 * The PeerLink adds a PeerHeader to outgoing messages and removes it from incoming messages while
 * checking clock skew to detect latency and react by sending congestion notifications to the peer.
 * In response to congestion notifications, this module will detect an optimal flow rate and buffer
 * packets to avoid sending faster than this rate. In the event that the buffer is over-filled,
 * packets will be dropped or replaced depending on highest penalty.
 */
struct PeerLink
{
    int queueLength;
    int linkMTU;
    bool peerHeaderEnabled;
};

struct PeerLink_Kbps
{
    uint32_t sendKbps;
    uint32_t recvKbps;
};

/**
 * Attempt to get a message from the peerlink to send, if it is time to send one.
 * If there are no messages in the queue or the link is already at capacity, NULL will be returned.
 */
struct Message* PeerLink_poll(struct PeerLink* pl);

/**
 * Enqueue a message to be sent.
 * @return the number of messages which are ready to be encrypted and written to the device.
 *         Call PeerLink_poll() to get these messages for actual sending.
 */
int PeerLink_send(struct Message* msg, struct PeerLink* pl);

/**
 * Receive (check the PeerHeader on) a message which has come in from the wire.
 * PeerHeader_SIZE bytes will be popped from the message if peerHeaders are enabled for this
 * peerLink.
 */
void PeerLink_recv(struct Message* msg, struct PeerLink* pl);

void PeerLink_kbps(struct PeerLink* peerLink, struct PeerLink_Kbps* output);


struct PeerLink* PeerLink_new(struct EventBase* base, struct Allocator* alloc);

#endif
