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
//#include "util/platform/libc/string.h"

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

    /** Location of the pointer to this search in the array. */
    uint16_t searchIndex;

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
struct SearchStore* SearchStore_new(struct Allocator* allocator,
                                    struct AverageRoller* gmrtRoller,
                                    struct EventBase* eventBase,
                                    struct Random* rand,
                                    struct Log* logger)
{
    return Allocator_clone(allocator, (&(struct SearchStore) {
        .allocator = allocator,
        .gmrtRoller = gmrtRoller,
        .eventBase = eventBase,
        .logger = logger,
        .rand = rand
    }));
}

static void freeSearch(void* vsearch)
{
    struct SearchStore_Search_pvt* search = vsearch;
    search->pub.store->searches[search->searchIndex] = NULL;
}

/** See: SearchStore.h */
struct SearchStore_Search* SearchStore_newSearch(uint8_t searchTarget[16],
                                                 struct SearchStore* store,
                                                 struct Allocator* alloc)
{
    uint32_t i;
    for (i = 0; i < SearchStore_MAX_SEARCHES && store->searches[i] != NULL; i++) ;
    if (i == SearchStore_MAX_SEARCHES) {
        // too many searches
        return NULL;
    }

    struct SearchStore_Search_pvt* search =
        Allocator_clone(alloc, (&(struct SearchStore_Search_pvt) {
            .pub = {
                .callbackContext = NULL,
                .store = store,
                .alloc = alloc
            },
            .searchIndex = i
        }));
    Bits_memcpyConst(search->searchTarget, searchTarget, Address_SEARCH_TARGET_SIZE);
    store->searches[i] = &search->pub;

    Allocator_onFree(alloc, freeSearch, search);

    return &search->pub;
}

/** Node index for finding a node from a search. */
struct SearchStore_NodeIndex
{
    /** Index of the search. */
    uint16_t search;

    /** Index of the node in the search. */
    uint16_t node;

    /** A random number which is used to check the response against the search node. */
    uint32_t magic;
};

/** See: SearchStore.h */
struct SearchStore_Node* SearchStore_getNode(String* txid, struct SearchStore* store)
{
    struct SearchStore_NodeIndex index;
    if (txid->len != sizeof(struct SearchStore_NodeIndex)) {
        // wacky txid
        return NULL;
    }
    Bits_memcpyConst(&index, txid->bytes, sizeof(struct SearchStore_NodeIndex));
    if (index.search >= SearchStore_MAX_SEARCHES || index.node >= SearchStore_SEARCH_NODES) {
        // out of bounds.
        return NULL;
    }
    struct SearchStore_Search_pvt* search =
        (struct SearchStore_Search_pvt*) store->searches[index.search];
    if (search == NULL) {
        return NULL;
    }
    struct SearchStore_Node* n = &search->nodesAsked[index.node];
    if (!String_equals(txid, n->txid)) {
       // http://www.youtube.com/watch?v=x9pJqvs96nA
       return NULL;
    }
    return n;
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

    Bits_memcpyConst(&pvtSearch->searchStack[index], addr, Address_SIZE);

    return 0;
}

/** See: SearchStore.h */
uint32_t SearchStore_replyReceived(struct SearchStore_Node* node)
{
    struct SearchStore* store = node->search->store;
    uint32_t delay = Time_currentTimeMilliseconds(store->eventBase) - node->timeOfRequest;
    AverageRoller_update(store->gmrtRoller, delay);
    Log_debug(store->logger,
               "Received response in %u milliseconds, gmrt now %u\n",
               delay,
               AverageRoller_getAverage(store->gmrtRoller));
    return delay;
}

/** See: SearchStore.h */
struct SearchStore_Node* SearchStore_getNextNode(struct SearchStore_Search* search)
{
    struct SearchStore_Search_pvt* pvtSearch = (struct SearchStore_Search_pvt*) search;
    if (!pvtSearch->searchStackSize) {
        return NULL;
    }
    struct SearchStore_NodeIndex ni = {
        .search = pvtSearch->searchIndex,
        .node = pvtSearch->nodesAskedIndex,
        .magic = Random_uint32(search->store->rand)
    };

    struct SearchStore_Node* nn = &pvtSearch->nodesAsked[pvtSearch->nodesAskedIndex];
    Bits_memcpyConst(&nn->address,
                     &pvtSearch->searchStack[--pvtSearch->searchStackSize],
                     sizeof(struct Address));
    nn->txid = String_newBinary((char*)&ni, sizeof(struct SearchStore_NodeIndex), search->alloc);
    nn->timeOfRequest = Time_currentTimeMilliseconds(search->store->eventBase);
    nn->search = search;

    pvtSearch->nodesAskedIndex = (pvtSearch->nodesAskedIndex + 1) % SearchStore_SEARCH_NODES;
    return nn;
}
