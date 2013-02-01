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
#ifndef SearchStore_H
#define SearchStore_H

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "benc/Object.h"
#include "util/log/Log.h"
#include "util/AverageRoller.h"

#include <stdint.h>
#include "util/events/EventBase.h"

/*--------------------Constants--------------------*/

/**
 * The number of nodes which will be held in a buffer when performing a search.
 * It is important that this number is large enough because when a search yields results, the
 * nodes which helped in get to those results have their reach number recalculated and if
 * they are prematurely evicted, they will not have their number recalculated.
 */
#define SearchStore_SEARCH_NODES 256

/** Maximum number of concurrent searches. */
#define SearchStore_MAX_SEARCHES 8


#if (SearchStore_SEARCH_NODES & (SearchStore_SEARCH_NODES - 1))
  #error "SearchStore_SEARCH_NODES must be a power of 2"
#endif
#if (SearchStore_MAX_SEARCHES & (SearchStore_MAX_SEARCHES - 1))
  #error "SearchStore_MAX_SEARCHES must be a power of 2"
#endif

/*--------------------Structures--------------------*/

/** Represents a single search */
struct SearchStore_Search;

struct SearchStore
{
    /** The means of getting memory to store each search. */
    struct Allocator* allocator;

    /**
     * An array of pointers to all search slots.
     * When a search completes, it will be freed and its pointer will be set to NULL.
     */
    struct SearchStore_Search* searches[SearchStore_MAX_SEARCHES];

    /** Averager for milliseconds wait for request turnaround. */
    struct AverageRoller* gmrtRoller;

    /** Libevent event base for getting current time. */
    struct EventBase* eventBase;

    struct Log* logger;
};

struct SearchStore_Node;
struct SearchStore_Node
{
    /** The index of the search which this node belongs to. */
    uint16_t searchIndex;

    /** This node's location in the search buffer for the search. */
    uint16_t nodeIndex;

    struct Address* address;
};

struct SearchStore_TraceElement;
struct SearchStore_TraceElement
{
    /** The number of milliseconds between the original request and the reply to it. */
    uint32_t delayUntilReply;

    /** The next TraceElement or NULL if this is the end of the trace. */
    struct SearchStore_TraceElement* next;

    struct Address* address;
};

/*--------------------Prototypes--------------------*/

/**
 * Create a new SearchStore.
 *
 * @param allocator the means of aquiring memory for the new store.
 * @param gmrtRoller averager of the mean response time for all nodes.
 * @param eventBase the libevent event base for getting current time.
 * @param logger
 */
struct SearchStore* SearchStore_new(struct Allocator* allocator,
                                    struct AverageRoller* gmrtRoller,
                                    struct EventBase* eventBase,
                                    struct Log* logger);

/**
 * Create a new search.
 *
 * @param searchTarget the ID of the thing which we are searching for.
 * @param store the SearchStore to allocate the search in.
 * @return the new search or NULL if MAX_SEARCHES are already running.
 */
struct SearchStore_Search* SearchStore_newSearch(
    const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
    struct SearchStore* store);

/**
 * Get the memory allocator for this search.
 * Allows other data to be stores with the search which will be freed when the search is freed.
 *
 * @param search the search.
 * @return the allocator for the search.
 */
struct Allocator* SearchStore_getAllocator(const struct SearchStore_Search* search);

/**
 * Set some additional data to go along with the search.
 *
 * @param vcontext the data to associate with the search.
 * @param search the search to associate the context with.
 */
void SearchStore_setContext(void* vcontext, struct SearchStore_Search* search);

/**
 * Get data which is associated with the search.
 *
 * @param search the search which has data associated with it.
 * @return the context which is associated with this search.
 */
void* SearchStore_getContext(struct SearchStore_Search* search);

/**
 * Free a search.
 *
 * @param search the search which we are done with.
 */
void SearchStore_freeSearch(struct SearchStore_Search* search);

/**
 * Get the search which a perticular node belongs to.
 *
 * @param node the search node.
 * @param store the search store where the search is being carried out.
 * @return the search.
 */
struct SearchStore_Search* SearchStore_getSearchForNode(const struct SearchStore_Node* node,
                                                        const struct SearchStore* store);

/**
 * Get a transaction ID which will work to get the same node again later.
 *
 * @param node the search node to get a TID for.
 * @param allocator the means of getting memory to store the output.
 * @return a string representing the TID to get the same node.
 */
String* SearchStore_tidForNode(const struct SearchStore_Node* node,
                               const struct Allocator* allocator);

/**
 * Get a node from the search store.
 *
 * @param tid the transaction ID which was generated by SearchStore_tidForNode().
 * @param store the search store where the node is.
 * @param allocator the memory allocator to use for getting the memory for the output.
 * @return a pointer to the node.
 */
struct SearchStore_Node* SearchStore_getNode(const String* tid,
                                             const struct SearchStore* store,
                                             const struct Allocator* allocator);

/**
 * Add a node to a search.
 *
 * @param the node which told us about this node or NULL if this is the beginning of the search.
 * @param address the address of the node to add.
 * @param evictUnrepliedIfOlderThan Any node which has not responded to the request and was sent
 *                                  a request before this time (number of milliseconds) is
 *                                  can be replaced.
 * @param search the search to add the node to.
 * @return 0 if the node was added successfully or -1 if there is no more space to add a node.
 */
int32_t SearchStore_addNodeToSearch(const struct SearchStore_Node* parent,
                                    struct Address* address,
                                    const uint64_t evictUnrepliedIfOlderThan,
                                    struct SearchStore_Search* search);

/**
 * Log that a node has been sent a request.
 * The amount of time it takes for that node to reply will be include in the backtrace.
 *
 * @param node the node which sent the reply.
 * @param store the search store which holds the search.
 */
void SearchStore_requestSent(const struct SearchStore_Node* node,
                             const struct SearchStore* store);

/**
 * Log that a node has sent us a valid reply.
 *
 * @param node the node which sent the reply.
 * @param store the search store which holds the search.
 * @return number of milliseconds between when the request was sent and when the reply was received.
 */
uint32_t SearchStore_replyReceived(const struct SearchStore_Node* node,
                                   const struct SearchStore* store);

/**
 * Get the next node to ask in this search.
 * This will get the last node to be added to the search which has not been sent a request yet.
 *
 * @param search the search to get the node for.
 * @param allocator the allocator to use for allocating the memory to store the output.
 * @return the node which is copied from the storage to the allocated space.
 */
struct SearchStore_Node* SearchStore_getNextNode(struct SearchStore_Search* search,
                                                 const struct Allocator* allocator);

/**
 * Get a linked list of trace elements showing the speed of each reply.
 * This is used to tell how fast each node performed to get us the result of the search.
 *
 * @param end the node which found the result.
 * @param store the SearchStore where the search took place.
 * @return a trace back from the end node to the original node which was first requested.
 */
struct SearchStore_TraceElement* SearchStore_backTrace(const struct SearchStore_Node* end,
                                                       const struct SearchStore* store);
#endif
