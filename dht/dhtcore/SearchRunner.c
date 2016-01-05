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
#include "dht/dhtcore/SearchRunner.h"
#include "dht/dhtcore/SearchStore.h"
#include "dht/dhtcore/RumorMill.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/ReplySerializer.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/CJDHTConstants.h"
#include "util/Identity.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"

struct SearchRunner_pvt
{
    struct SearchRunner pub;
    struct SearchStore* searchStore;
    struct NodeStore* nodeStore;
    struct Log* logger;
    struct EventBase* eventBase;
    struct RouterModule* router;
    struct RumorMill* rumorMill;
    uint8_t myAddress[16];

    /** Number of concurrent searches in operation. */
    int searches;

    /** Maximum number of concurrent searches allowed. */
    int maxConcurrentSearches;

    /** Beginning of a linked list of searches. */
    struct SearchRunner_Search* firstSearch;

    Identity
};


/**
 * A context for the internals of a search.
 */
struct SearchRunner_Search;
struct SearchRunner_Search
{
    struct RouterModule_Promise pub;

    /** The router module carrying out the search. */
    struct SearchRunner_pvt* const runner;

    /** The number of requests which have been sent out so far for this search. */
    uint32_t totalRequests;

    /** Maximum number of requests to make before terminating the search. */
    uint32_t maxRequests;

    uint32_t maxRequestsIfFound;

    uint32_t numFinds;

    /** The address which we are searching for. */
    struct Address target;

    /** String form of the 16 byte ipv6 address. */
    String* targetStr;

    /**
     * The SearchStore_Search structure for this search,
     * used to keep track of which nodes are participating.
     */
    struct SearchStore_Search* search;

    /** The last node sent a search request. */
    struct Address lastNodeAsked;

    /**
     * The timeout if this timeout is hit then the search will continue
     * but the node will still be allowed to respond and it will be counted as a pong.
     */
    struct Timeout* continueSearchTimeout;

    /** Next search in the linked list. */
    struct SearchRunner_Search* nextSearch;

    /** Self pointer for this search so that the search can be removed from the linked list. */
    struct SearchRunner_Search** thisSearch;

    Identity
};

/**
 * Spot a duplicate entry in a node list.
 * If a router sends a response containing duplicate entries,
 * only the last (best) entry should be accepted.
 *
 * @param nodes the list of nodes.
 * @param index the index of the entry to check for being a duplicate.
 * @return true if duplicate, otherwise false.
 */
static inline bool isDuplicateEntry(struct Address_List* list, uint32_t index)
{
    for (int i = index+1; i < list->length; i++) {
        if (Bits_memcmp(&list->elems[i].key, &list->elems[i].key, Address_KEY_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

static void searchStep(struct SearchRunner_Search* search);

static void searchReplyCallback(struct RouterModule_Promise* promise,
                                uint32_t lagMilliseconds,
                                struct Address* from,
                                Dict* result)
{
    struct SearchRunner_Search* search =
        Identity_check((struct SearchRunner_Search*)promise->userData);

    if (!Bits_memcmp(from->ip6.bytes, search->lastNodeAsked.ip6.bytes, 16)) {
        Timeout_resetTimeout(search->continueSearchTimeout,
                RouterModule_searchTimeoutMilliseconds(search->runner->router));
    }

    if (!Bits_memcmp(from->ip6.bytes, search->target.ip6.bytes, 16)) {
        search->numFinds++;
    }

    struct Address_List* nodeList =
        ReplySerializer_parse(from, result, search->runner->logger, true, promise->alloc);

    struct Address* best = NULL;

    for (int i = 0; nodeList && i < nodeList->length; i++) {
        if (isDuplicateEntry(nodeList, i)) {
            continue;
        }

        if (Address_closest(&search->target, &nodeList->elems[i], from) >= 0) {
            // Too much noise.
            //Log_debug(search->runner->logger, "Answer was further from the target than us.\n");
            continue;
        }

        if (search->lastNodeAsked.path != from->path) {
            // old queries coming in late...
            continue;
        }

        struct Node_Two* nn =
            NodeStore_getBest(search->runner->nodeStore, nodeList->elems[i].ip6.bytes);

        if (!nn) {
            RumorMill_addNode(search->runner->rumorMill, &nodeList->elems[i]);
        }

        //nodeList->elems[i].path =
        //    NodeStore_optimizePath(search->runner->nodeStore, nodeList->elems[i].path);

        if (!Bits_memcmp(nodeList->elems[i].ip6.bytes, search->target.ip6.bytes, 16)) {
            if (!best) {
                best = &nodeList->elems[i];
                continue;
            } else if (nodeList->elems[i].path < best->path) {
                SearchStore_addNodeToSearch(best, search->search);
                best = &nodeList->elems[i];
                continue;
            }
        }

        SearchStore_addNodeToSearch(&nodeList->elems[i], search->search);
    }
    if (best) {
        SearchStore_addNodeToSearch(best, search->search);
    }
}

static void searchCallback(struct RouterModule_Promise* promise,
                           uint32_t lagMilliseconds,
                           struct Address* from,
                           Dict* result)
{
    struct SearchRunner_Search* search =
        Identity_check((struct SearchRunner_Search*)promise->userData);

    if (from) {
        searchReplyCallback(promise, lagMilliseconds, from, result);
    }

    if (search->pub.callback) {
        search->pub.callback(&search->pub, lagMilliseconds, from, result);
    }
    searchStep(search);
}

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 */
static void searchStep(struct SearchRunner_Search* search)
{
    struct SearchRunner_pvt* ctx = Identity_check((struct SearchRunner_pvt*)search->runner);

    struct SearchStore_Node* nextSearchNode;
    for (;;) {
        nextSearchNode = SearchStore_getNextNode(search->search);

        // If the number of requests sent has exceeded the max search requests, let's stop there.
        if (search->totalRequests >= search->maxRequests) {
            // fallthrough
        } else if (search->numFinds > 0 && search->totalRequests >= search->maxRequestsIfFound) {
            // fallthrough
        } else if (nextSearchNode == NULL) {
            // fallthrough
        } else {
            break;
        }
        if (search->pub.callback) {
            search->pub.callback(&search->pub, 0, NULL, NULL);
        }
        Allocator_free(search->pub.alloc);
        return;
    }

    Bits_memcpy(&search->lastNodeAsked, &nextSearchNode->address, sizeof(struct Address));

    struct RouterModule_Promise* rp =
        RouterModule_newMessage(&nextSearchNode->address, 0, ctx->router, search->pub.alloc);

    Dict* message = Dict_new(rp->alloc);

    if (!Bits_memcmp(nextSearchNode->address.ip6.bytes, search->target.ip6.bytes, 16)) {
        Dict_putString(message, CJDHTConstants_QUERY, CJDHTConstants_QUERY_GP, rp->alloc);
    } else {
        Dict_putString(message, CJDHTConstants_QUERY, CJDHTConstants_QUERY_FN, rp->alloc);
    }
    Dict_putString(message, CJDHTConstants_TARGET, search->targetStr, rp->alloc);

    rp->userData = search;
    rp->callback = searchCallback;

    RouterModule_sendMessage(rp, message);

    search->totalRequests++;
}

// Triggered by a search timeout (the message may still come back and will be treated as a ping)
static void searchNextNode(void* vsearch)
{
    struct SearchRunner_Search* search = Identity_check((struct SearchRunner_Search*) vsearch);

    // Timeout for trying the next node.
    Timeout_resetTimeout(search->continueSearchTimeout,
                         RouterModule_searchTimeoutMilliseconds(search->runner->router));

    searchStep(search);
}

static int searchOnFree(struct Allocator_OnFreeJob* job)
{
    struct SearchRunner_Search* search =
        Identity_check((struct SearchRunner_Search*)job->userData);

    *search->thisSearch = search->nextSearch;
    if (search->nextSearch) {
        search->nextSearch->thisSearch = search->thisSearch;
    }
    Assert_true(search->runner->searches > 0);
    search->runner->searches--;
    return 0;
}

struct SearchRunner_SearchData* SearchRunner_showActiveSearch(struct SearchRunner* searchRunner,
                                                              int number,
                                                              struct Allocator* alloc)
{
    struct SearchRunner_pvt* runner = Identity_check((struct SearchRunner_pvt*)searchRunner);
    struct SearchRunner_Search* search = runner->firstSearch;
    while (search && number > 0) {
        search = search->nextSearch;
        number--;
    }

    struct SearchRunner_SearchData* out =
        Allocator_calloc(alloc, sizeof(struct SearchRunner_SearchData), 1);

    if (search) {
        Bits_memcpy(out->target, &search->target.ip6.bytes, 16);
        Bits_memcpy(&out->lastNodeAsked, &search->lastNodeAsked, sizeof(struct Address));
        out->totalRequests = search->totalRequests;
    }
    out->activeSearches = runner->searches;

    return out;
}

struct RouterModule_Promise* SearchRunner_search(uint8_t target[16],
                                                 int maxRequests,
                                                 int maxRequestsIfFound,
                                                 struct SearchRunner* searchRunner,
                                                 struct Allocator* allocator)
{
    struct SearchRunner_pvt* runner = Identity_check((struct SearchRunner_pvt*)searchRunner);

    if (runner->searches > runner->maxConcurrentSearches) {
        Log_debug(runner->logger, "Skipping search because there are already [%d] searches active",
                  runner->searches);
        return NULL;
    }

    if (maxRequests < 1) {
        maxRequests = SearchRunner_DEFAULT_MAX_REQUESTS;
    }
    if (maxRequestsIfFound < 1) {
        maxRequestsIfFound = SearchRunner_DEFAULT_MAX_REQUESTS_IF_FOUND;
    }

    struct Allocator* alloc = Allocator_child(allocator);

    struct Address targetAddr = { .path = 0 };
    Bits_memcpy(targetAddr.ip6.bytes, target, Address_SEARCH_TARGET_SIZE);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(runner->nodeStore,
                                  &targetAddr,
                                  maxRequests,
                                  Version_CURRENT_PROTOCOL,
                                  alloc);

    if (nodes->size == 0) {
        Log_debug(runner->logger, "No nodes available for beginning search");
        Allocator_free(alloc);
        return NULL;
    }

    struct SearchStore_Search* sss = SearchStore_newSearch(target, runner->searchStore, alloc);

    for (int i = 0; i < (int)nodes->size; i++) {
        SearchStore_addNodeToSearch(&nodes->nodes[i]->address, sss);
    }

    struct SearchRunner_Search* search = Allocator_clone(alloc, (&(struct SearchRunner_Search) {
        .pub = {
            .alloc = alloc
        },
        .runner = runner,
        .search = sss,
        .maxRequests = maxRequests,
        .maxRequestsIfFound = maxRequestsIfFound
    }));
    Identity_set(search);
    runner->searches++;
    Allocator_onFree(alloc, searchOnFree, search);
    Bits_memcpy(&search->target, &targetAddr, sizeof(struct Address));

    if (runner->firstSearch) {
        search->nextSearch = runner->firstSearch;
        runner->firstSearch->thisSearch = &search->nextSearch;
    }
    runner->firstSearch = search;
    search->thisSearch = &runner->firstSearch;

    search->targetStr = String_newBinary((char*)targetAddr.ip6.bytes, 16, alloc);

    // Trigger the searchNextNode() immedietly but asynchronously.
    search->continueSearchTimeout =
        Timeout_setTimeout(searchNextNode, search, 0, runner->eventBase, alloc);

    return &search->pub;
}

struct SearchRunner* SearchRunner_new(struct NodeStore* nodeStore,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct RouterModule* module,
                                      uint8_t myAddress[16],
                                      struct RumorMill* rumorMill,
                                      struct Allocator* allocator)
{
    struct Allocator* alloc = Allocator_child(allocator);
    struct SearchRunner_pvt* out = Allocator_clone(alloc, (&(struct SearchRunner_pvt) {
        .nodeStore = nodeStore,
        .logger = logger,
        .eventBase = base,
        .router = module,
        .rumorMill = rumorMill,
        .maxConcurrentSearches = SearchRunner_DEFAULT_MAX_CONCURRENT_SEARCHES
    }));
    out->searchStore = SearchStore_new(alloc, logger);
    Bits_memcpy(out->myAddress, myAddress, 16);
    Identity_set(out);

    return &out->pub;
}
