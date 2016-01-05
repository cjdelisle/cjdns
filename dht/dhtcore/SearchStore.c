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
#include "dht/Address.h"
#include "dht/dhtcore/SearchStore.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/AverageRoller.h"
#include "util/Endian.h"
#include "util/events/Time.h"

/*--------------------Structures--------------------*/

/** An outstanding search for a target. */
struct SearchStore_Search_pvt
{
    struct SearchStore_Search pub;

    /** Numbert of Address's in searchStack. */
    uint16_t searchStackSize;

    /** The index to insert the next node in nodesAsked, this will wrap. */
    uint16_t nodesAskedIndex;

    /** The ID of what we are looking for. */
    uint8_t searchTarget[16];

    /** The nodes to ask when performing the search. */
    struct Address searchStack[SearchStore_SEARCH_NODES];

    /** The nodes which have already been queried in this search. */
    struct SearchStore_Node nodesAsked[SearchStore_SEARCH_NODES];
};

/*--------------------Functions--------------------*/

/** See: SearchStore.h */
struct SearchStore* SearchStore_new(struct Allocator* allocator, struct Log* logger)
{
    return Allocator_clone(allocator, (&(struct SearchStore) {
        .allocator = allocator,
        .logger = logger
    }));
}

/** See: SearchStore.h */
struct SearchStore_Search* SearchStore_newSearch(uint8_t searchTarget[16],
                                                 struct SearchStore* store,
                                                 struct Allocator* alloc)
{
    struct SearchStore_Search_pvt* search =
        Allocator_clone(alloc, (&(struct SearchStore_Search_pvt) {
            .pub = {
                .callbackContext = NULL,
                .store = store,
                .alloc = alloc
            }
        }));
    Bits_memcpy(search->searchTarget, searchTarget, Address_SEARCH_TARGET_SIZE);

    return &search->pub;
}

/** See: SearchStore.h */
int SearchStore_addNodeToSearch(struct Address* addr, struct SearchStore_Search* search)
{
    struct SearchStore_Search_pvt* pvtSearch = (struct SearchStore_Search_pvt*) search;
    for (int i = 0; i < SearchStore_SEARCH_NODES; i++) {
        if (!Bits_memcmp(addr->key, pvtSearch->nodesAsked[i].address.key, 32)) {
            // Already bugged this node, skip.
            return -1;
        } else if (i == pvtSearch->nodesAskedIndex
            && Bits_isZero(pvtSearch->nodesAsked[i].address.key, 32))
        {
            // short circuit a common case where the nodesAskedIndex has not yet wrapped.
            break;
        }
    }

    const uint16_t index =
        (pvtSearch->searchStackSize < SearchStore_SEARCH_NODES)
            // add it to the end...
            ? pvtSearch->searchStackSize++

            // nodes are added worst-to-best so replace the last entry.
            : pvtSearch->searchStackSize - 1;

    Bits_memcpy(&pvtSearch->searchStack[index], addr, Address_SIZE);

    return 0;
}

/** See: SearchStore.h */
struct SearchStore_Node* SearchStore_getNextNode(struct SearchStore_Search* search)
{
    struct SearchStore_Search_pvt* pvtSearch = (struct SearchStore_Search_pvt*) search;
    if (!pvtSearch->searchStackSize) {
        return NULL;
    }

    struct SearchStore_Node* nn = &pvtSearch->nodesAsked[pvtSearch->nodesAskedIndex];
    Bits_memcpy(&nn->address,
                     &pvtSearch->searchStack[--pvtSearch->searchStackSize],
                     sizeof(struct Address));
    nn->search = search;

    pvtSearch->nodesAskedIndex = (pvtSearch->nodesAskedIndex + 1) % SearchStore_SEARCH_NODES;
    return nn;
}
