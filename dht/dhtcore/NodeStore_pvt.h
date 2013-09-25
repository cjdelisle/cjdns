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
#ifndef NodeStore_pvt_H
#define NodeStore_pvt_H

#include "crypto/random/Random.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeIndex.h"
#include "util/log/Log.h"

/** A list of DHT nodes. */
struct NodeStore_pvt
{
    struct NodeStore pub;

    /** Source of random numbers. */
    struct Random* rand;

    /** array of pointers to allocated Nodes */
    struct Node* nodes;

    /** used / free heap of node pointers */
    struct Node** heap;

    /** an index of nodes accesssible by ip6 or path */
    struct NodeIndex* index;

    /** The maximum number of nodes which can be allocated. */
    int capacity;

    /** The sum of the logs base 2 of all node labels. */
    int32_t labelSum;

    /** The means for this node store to log. */
    struct Log* logger;

    Identity
};

#endif
