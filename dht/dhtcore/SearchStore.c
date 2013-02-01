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
#include "util/platform/libc/string.h"

#include "dht/Address.h"
#include "dht/dhtcore/SearchStore.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/AverageRoller.h"
#include "util/Endian.h"
#include "util/events/Time.h"

/*--------------------Structures--------------------*/

/**
 * Information about a node which has responded to a search.
 * This is used to update the node's reach after the search has yielded results.
 */
struct SearchNode;
struct SearchNode
{
    /** Number of milliseconds since the epoch when the search request was sent to this node. */
    uint64_t timeOfRequest;

    /** The node which introduced us to this node. */
    struct SearchNode* parent;

    /** The number of milliseconds between the original request and the reply to it. */
    uint32_t delayUntilReply;

    /** The address of this node. */
    struct Address address;
};

/** An outstanding search for a target. */
struct SearchStore_Search
{
    /** The location of this search in the array of searches. */
    uint16_t searchIndex;

    /** Number of nodes involved in this search. */
    uint16_t nodeCount;

    /** The last node to be inserted, SearchStore_getNextNode() will work backward from here. */
    uint16_t indexOfLastInsertedNode;

    /** The ID of what we are looking for. */
    uint8_t searchTarget[Address_SEARCH_TARGET_SIZE];

    /** Number of milliseconds since the epoch when the last request was sent for this search. */
    uint32_t timeOfLastRequest;

    // This will be padded 4 bytes on a 64bit machine

    /** Some external data which might be associated with this search. */
    void* externalContext;

    /** The SearchStore which created this search. */
    struct SearchStore* store;

    /** Allocator which allocated this Search. */
    struct Allocator* allocator;

    /** The nodes to ask when performing the search. */
    struct SearchNode nodes[SearchStore_SEARCH_NODES];
};

/** Node index for finding a node from a search. */
struct SearchNodeIndex
{
    /** Index of the search. */
    uint16_t search;

    /** Index of the node in the search. */
    uint16_t node;
};

/*--------------------Functions--------------------*/

/** See: SearchStore.h */
struct SearchStore* SearchStore_new(struct Allocator* allocator,
                                    struct AverageRoller* gmrtRoller,
                                    struct EventBase* eventBase,
                                    struct Log* logger)
{
    struct SearchStore* out = allocator->calloc(sizeof(struct SearchStore), 1, allocator);
    out->allocator = allocator;
    out->gmrtRoller = gmrtRoller;
    out->eventBase = eventBase;
    out->logger = logger;
    return out;
}

/** See: SearchStore.h */
struct SearchStore_Search* SearchStore_newSearch(
    const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
    struct SearchStore* store)
{
    uint32_t i;
    for (i = 0; i < SearchStore_MAX_SEARCHES && store->searches[i] != NULL; i++) ;
    if (i == SearchStore_MAX_SEARCHES) {
        // too many searches
        return NULL;
    }

    struct Allocator* allocator = Allocator_child(store->allocator);
    struct SearchStore_Search* search =
        allocator->calloc(sizeof(struct SearchStore_Search), 1, allocator);
    search->searchIndex = i;
    search->nodeCount = 0;
    Bits_memcpyConst(search->searchTarget, searchTarget, Address_SEARCH_TARGET_SIZE);
    search->timeOfLastRequest = 0;
    search->externalContext = NULL;
    search->store = store;
    search->allocator = allocator;
    store->searches[i] = search;

    return search;
}

/** See: SearchStore.h */
struct Allocator* SearchStore_getAllocator(const struct SearchStore_Search* search)
{
    return search->allocator;
}

/** See: SearchStore.h */
void SearchStore_setContext(void* vcontext, struct SearchStore_Search* search)
{
    search->externalContext = vcontext;
}

/** See: SearchStore.h */
void* SearchStore_getContext(struct SearchStore_Search* search)
{
    return search->externalContext;
}

/** See: SearchStore.h */
void SearchStore_freeSearch(struct SearchStore_Search* search)
{
    search->store->searches[search->searchIndex] = NULL;
    search->allocator->free(search->allocator);
}

/** See: SearchStore.h */
struct SearchStore_Search* SearchStore_getSearchForNode(const struct SearchStore_Node* node,
                                                        const struct SearchStore* store)
{
    return store->searches[node->searchIndex];
}

/**
 * Calculate the log base 2 of an integer.
 * Get the position of the most significant bit counting from the right.
 * This implementation is slow but can be calculated at compile time for constants.
 *
 * @param number the number to test
 * @return the log base 2 of that number
 */
static inline uint32_t intLog2_constant(const uint32_t number)
{
    #define BIT(K) (((number & (1 << K)) >> K) * K)

    return BIT( 0) + BIT( 1) + BIT( 2) + BIT( 3) + BIT( 4) + BIT( 5) + BIT( 6) + BIT( 7)
         + BIT( 8) + BIT( 9) + BIT(10) + BIT(11) + BIT(12) + BIT(13) + BIT(14) + BIT(15)
         + BIT(16) + BIT(17) + BIT(18) + BIT(19) + BIT(20) + BIT(21) + BIT(22) + BIT(23)
         + BIT(24) + BIT(25) + BIT(26) + BIT(27) + BIT(28) + BIT(29) + BIT(30) + BIT(31);

    #undef BIT
}

/**
 * Serialize a node index into a string which is sutable for storage as a transaction ID.
 *
 * @param index the search index and node index.
 *        These must not exceedSearchStore_MAX_SEARCHES and SearchStore_SEARCH_NODES respectively.
 * @param allocator the memory allocator to get memory for storing the output string.
 * @return a string which will deserialize to the same index if passed to searchNodeIndexForTid().
 */
static String* tidForSearchNodeIndex(const struct SearchNodeIndex* index,
                                     const struct Allocator* allocator)
{
    const uint32_t searchNodesBits = intLog2_constant(SearchStore_SEARCH_NODES);
    const uint32_t maxSearchBits = intLog2_constant(SearchStore_MAX_SEARCHES);
    const uint32_t totalBits = searchNodesBits + maxSearchBits;
    const uint32_t totalBytes = totalBits / 8 + ((totalBits % 8 > 0) ? 1 : 0);

    // Make sure the above computations are run at compile time.
    // Only works if using -O3 and obviously not platform independent.
    //Assert_true(__builtin_constant_p(totalBytes));

    uint32_t out = (index->node << maxSearchBits) | index->search;

    // If we are testing then try to confuse the decoder by
    // padding the unused edge with random bits.
    //out |= rand() << totalBits;

    if (Endian_isBigEndian()) {
        out <<= (32 - totalBits);
    }

    return String_newBinary((char*) &out, totalBytes, allocator);
}

/**
 * Deserialize a node index from a string which is sutable for storage as a transaction ID.
 *
 * @param tid the serialized string.
 * @return the search index and node index.
 */
static struct SearchNodeIndex searchNodeIndexForTid(const String* tid)
{
    const uint32_t maxNodesBits = intLog2_constant(SearchStore_SEARCH_NODES);
    const uint32_t maxSearchBits = intLog2_constant(SearchStore_MAX_SEARCHES);

    uint32_t number = 0;

    Bits_memcpy(&number, tid->bytes, (tid->len > 4) ? 4 : tid->len);

    if (Endian_isBigEndian()) {
        number >>= (32 - maxNodesBits - maxSearchBits);
    }

    return (struct SearchNodeIndex) {
        .search = number & (UINT32_MAX >> (32 - maxSearchBits)),

        .node = (number >> maxSearchBits) & (UINT32_MAX >> (32 - maxNodesBits))
    };
}

/** See: SearchStore.h */
String* SearchStore_tidForNode(const struct SearchStore_Node* node,
                               const struct Allocator* allocator)
{
    const struct SearchNodeIndex index = {
        .search = node->searchIndex,
        .node = node->nodeIndex
    };

    return tidForSearchNodeIndex(&index, allocator);
}

/** See: SearchStore.h */
struct SearchStore_Node* SearchStore_getNode(const String* tid,
                                             const struct SearchStore* store,
                                             const struct Allocator* allocator)
{
    struct SearchNodeIndex index = searchNodeIndexForTid(tid);
    struct SearchStore_Search* search = store->searches[index.search];
    if (search == NULL) {
        // No such search.
        return NULL;
    }
    if (search->nodeCount <= index.node) {
       // no such node.
       return NULL;
    }

    struct SearchStore_Node* out =
        allocator->malloc(sizeof(struct SearchStore_Node), allocator);

    struct SearchNode* node = &search->nodes[index.node];

    out->address = &node->address;
    out->searchIndex = index.search;
    out->nodeIndex = index.node;

    return out;
}

/** See: SearchStore.h */
int32_t SearchStore_addNodeToSearch(const struct SearchStore_Node* parent,
                                    struct Address* addr,
                                    const uint64_t evictUnrepliedIfOlderThan,
                                    struct SearchStore_Search* search)
{
    struct SearchNode* node;
    if (search->nodeCount < SearchStore_SEARCH_NODES) {
        // Easy way, add it to the end...
        node = &search->nodes[search->nodeCount];
        search->indexOfLastInsertedNode = search->nodeCount;
        search->nodeCount++;
    } else {
        // Hard way, gotta find a node to replace.
        node = NULL;
        for (uint32_t i = 0; i < SearchStore_SEARCH_NODES; i++) {
            if (search->nodes[i].delayUntilReply == 0
                && search->nodes[i].timeOfRequest < evictUnrepliedIfOlderThan)
            {
                if (parent->nodeIndex == i) {
                    continue;
                }
                node = &search->nodes[i];
                search->indexOfLastInsertedNode = i;
                // Find the node which points to this node as its parent
                // Set that to NULL to prevent looping.
                for (i++; i != search->indexOfLastInsertedNode; i++) {
                    if (i == SearchStore_SEARCH_NODES) {
                        i = -1;
                    } else if (search->nodes[i].parent == node) {
                        search->nodes[i].parent = NULL;
                    }
                }
                break;
            }
        }
        if (node == NULL) {
            return -1;
        }
    }

    node->parent = (parent != NULL) ? &search->nodes[parent->nodeIndex] : NULL;
    node->timeOfRequest = 0;
    node->delayUntilReply = 0;
    Bits_memcpyConst(&node->address, addr, Address_SIZE);

    return 0;
}

/** See: SearchStore.h */
void SearchStore_requestSent(const struct SearchStore_Node* node,
                             const struct SearchStore* store)
{
    store->searches[node->searchIndex]->nodes[node->nodeIndex].timeOfRequest =
        Time_currentTimeMilliseconds(store->eventBase);
}

/** See: SearchStore.h */
uint32_t SearchStore_replyReceived(const struct SearchStore_Node* node,
                                   const struct SearchStore* store)
{
    struct SearchNode* searchNode = &store->searches[node->searchIndex]->nodes[node->nodeIndex];
    uint32_t delay = Time_currentTimeMilliseconds(store->eventBase) - searchNode->timeOfRequest;
    AverageRoller_update(store->gmrtRoller, delay);
    Log_debug(store->logger,
               "Received response in %u milliseconds, gmrt now %u\n",
               delay,
               AverageRoller_getAverage(store->gmrtRoller));
    searchNode->delayUntilReply = delay;
    return delay;
}

/** See: SearchStore.h */
struct SearchStore_Node* SearchStore_getNextNode(struct SearchStore_Search* search,
                                                 const struct Allocator* allocator)
{
    uint16_t index = search->indexOfLastInsertedNode;
    do {
        if (search->nodes[index].timeOfRequest == 0) {
            // Found the next node.
            struct SearchStore_Node* out =
                allocator->malloc(sizeof(struct SearchStore_Node), allocator);
            out->address = &search->nodes[index].address;
            out->searchIndex = search->searchIndex;
            out->nodeIndex = index;
            return out;
        }
        if (index == 0) {
            index = search->nodeCount;
        }
        index--;
    } while (index != search->indexOfLastInsertedNode);

    return NULL;
}

/** See: SearchStore.h */
struct SearchStore_TraceElement* SearchStore_backTrace(const struct SearchStore_Node* end,
                                                       const struct SearchStore* store)
{
    const uint16_t searchIndex = end->searchIndex;
    const uint16_t nodeIndex = end->nodeIndex;
    struct Allocator* const allocator = store->searches[searchIndex]->allocator;
    struct SearchNode* const lastSearchNode = &store->searches[searchIndex]->nodes[nodeIndex];
    struct SearchNode* searchNode = lastSearchNode;
    struct SearchStore_TraceElement* element =
        allocator->malloc(sizeof(struct SearchStore_TraceElement), allocator);
    struct SearchStore_TraceElement* const out = element;

    for (;;) {
        element->delayUntilReply = searchNode->delayUntilReply;
        element->address = &searchNode->address;
        searchNode = searchNode->parent;
        if (searchNode == NULL) {
            element->next = NULL;
            break;
        }

        element->next = allocator->malloc(sizeof(struct SearchStore_TraceElement), allocator);
        element = element->next;
    }

    return out;
}
