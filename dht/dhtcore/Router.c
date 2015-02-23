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

#include "dht/dhtcore/Router.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/SearchRunner.h"
#include "util/Identity.h"

struct Router_pvt
{
    struct Router pub;
    struct NodeStore* nodeStore;
    struct SearchRunner* searchRunner;
    struct RouterModule* routerModule;
    Identity
};

struct Node_Link* Router_linkForPath(struct Router* r, uint64_t path)
{
    struct Router_pvt* rr = Identity_check((struct Router_pvt*)r);
    return NodeStore_linkForPath(rr->nodeStore, path);
}

void Router_sendGetPeers(struct Router* r,
                         struct Address* addr,
                         uint64_t nearbyLabel,
                         uint32_t timeoutMilliseconds,
                         struct Allocator* alloc)
{
    struct Router_pvt* rr = Identity_check((struct Router_pvt*)r);
    RouterModule_getPeers(addr, nearbyLabel, timeoutMilliseconds, rr->routerModule, alloc);
}

struct Node_Two* Router_lookup(struct Router* r, uint8_t targetAddr[16])
{
    struct Router_pvt* rr = Identity_check((struct Router_pvt*)r);
    return NodeStore_getBest(rr->nodeStore, targetAddr);
}

void Router_brokenLink(struct Router* r, uint64_t path, uint64_t labelAtErrorHop)
{
    struct Router_pvt* rr = Identity_check((struct Router_pvt*)r);
    NodeStore_brokenLink(rr->nodeStore, path, labelAtErrorHop);
}

void Router_disconnectedPeer(struct Router* r, uint64_t path)
{
    struct Router_pvt* rr = Identity_check((struct Router_pvt*)r);
    NodeStore_disconnectedPeer(rr->nodeStore, path);
}

struct Router* Router_new(struct RouterModule* rm,
                          struct NodeStore* ns,
                          struct SearchRunner* sr,
                          struct Allocator* alloc)
{
    struct Router_pvt* rp = Allocator_clone(alloc, (&(struct Router_pvt) {
        .routerModule = rm,
        .nodeStore = ns,
        .searchRunner = sr
    }));
    Identity_set(rp);
    return &rp->pub;
}
