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
#ifndef RouterModule_pvt_H
#define RouterModule_pvt_H

/**
 * Internal structures which are needed for testing but should not be exposed to the outside world.
 */

struct RouterModule_Ping;

/** The context for this module. */
struct RouterModule
{
    /** This node's address, doesn't contain networkAddress component. */
    struct Address address;

    /** An AverageRoller for calculating the global mean response time. */
    void* gmrtRoller;

    /** The storage for the searches which are in progress. */
    struct SearchStore* searchStore;

    /** The storage for the nodes. */
    struct NodeStore* nodeStore;

    /** The sum of reach of all nodes. */
    uint64_t totalReach;

    /** The registry which is needed so that we can send messages. */
    struct DHTModuleRegistry* registry;

    /** The libevent event base for handling timeouts. */
    struct EventBase* eventBase;

    struct Janitor* janitor;

    struct Log* logger;

    struct Allocator* allocator;

    /** An array of timers for in-flight pings, allocated with pingAllocator. */
    struct RouterModule_Ping* pings[RouterModule_MAX_CONCURRENT_PINGS];

    /** A memory allocator which only exists when there are pings in flight. */
    struct Allocator* pingAllocator;

    struct Admin* admin;

    uint8_t gitVersionBytes[20];
    String gitVersion;

    struct Random* rand;
};

#endif
