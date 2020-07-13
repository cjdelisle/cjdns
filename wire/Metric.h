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
#ifndef Metric_H
#define Metric_H

#include <stdint.h>

// This is INTERNAL to cjdns, it does not get used in the protocol at all,
// but it is an internal protocol which is used to coordinate the different
// modules.
//
// Different parts of the code become aware of the validity of different paths.
// The InterfaceController becomes aware of peers, the DHT and the subnode
// Pathfinder become aware of more distant nodes. And of course the SessionManager
// becomes aware of a node by virtue of communicating with it.
//
// Each of these components informs the SessionManager when it becomes aware
// of a path, if the SessionManager has a session for the node and the metric
// for the path is lower than the metric which the SessionManager currently
// has, it will update the path to that node.
//
// Because this is not inter-node protocol, you can change these numbers at any
// time.

// The snode says this is a path to the node
static const uint32_t Metric_SNODE_SAYS =      0xff100000;

// Node is a direct peer according to the SubnodePathfinder
static const uint32_t Metric_PF_PEER =         0xff200000;

// This is our discovered path to our snode
static const uint32_t Metric_SNODE =           0xff300000;

// Node is a direct peer according to the (dht) Pathfinder
static const uint32_t Metric_DHT_PEER =        0xff400000;

// We sent a ping to a node to complete the session setup, it replied.
// This is a path which we know works, but we don't know if it's any good.
static const uint32_t Metric_PING_REPLY =      0xff500000;
static const uint32_t Metric_DHT_INCOMING =    0xff510000;

// Anything that comes from the DHT Pathfinder is &'d with Metric_DHT_MASK
// and OR'd with Metric_DHT.
static const uint32_t Metric_DHT =             0xff600000;
static const uint32_t Metric_DHT_MASK =        0x000fffff;

// Incoming message, CryptoAuth has not yet checked that the key is real
static const uint32_t Metric_SM_INCOMING =     0xff700000;
// Outgoing message, some upper layer thinks this is the path
static const uint32_t Metric_SM_SEND =         0xff710000;

// We have no information about this path, but we don't know it to be dead
static const uint32_t Metric_NO_INFO =         0xfffffffe;

// This will cause the SM to kill off a path
static const uint32_t Metric_DEAD_LINK =       0xffffffff;



// This is known to be a peer by InterfaceController, OR'd with the latency
// This is NEVER used as a metric in the SessionManager, it is only how
// the InterfaceController reports a peer to the pathfinders.
static const uint32_t Metric_IC_PEER =         0xffff0000;
static const uint32_t Metric_IC_PEER_MASK =    0x0000ffff;

#endif