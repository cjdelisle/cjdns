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
    struct AverageRoller* gmrtRoller;

    /** The storage for the nodes. */
    struct NodeStore* nodeStore;

    /** The sum of reach of all nodes. */
    uint64_t totalReach;

    /** The registry which is needed so that we can send messages. */
    struct DHTModuleRegistry* registry;

    /** The libevent event base for handling timeouts. */
    struct EventBase* eventBase;

    struct Log* logger;

    struct Allocator* allocator;

    struct Pinger* pinger;

    struct Admin* admin;

    struct Random* rand;

    uint32_t pingsInFlight;


    /**
     * Used by handleIncoming() to pass a message to onResponse()
     * while the execution goes through pinger.
     */
    struct DHTMessage* currentMessage;

    Identity
};

#endif
