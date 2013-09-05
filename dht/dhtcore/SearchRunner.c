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
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/VersionList.h"
#include "dht/CJDHTConstants.h"
#include "switch/LabelSplicer.h"
#include "util/Identity.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"


/** The maximum number of requests to make before calling a search failed. */
#define MAX_REQUESTS_PER_SEARCH 8


struct SearchRunner_pvt
{
    struct SearchRunner pub;
    struct SearchStore* searchStore;
    struct NodeStore* nodeStore;
    struct Log* logger;
    struct EventBase* eventBase;
    struct RouterModule* router;
    uint8_t myAddress[16];
    Identity
};


/**
 * A context for the internals of a search.
 */
struct SearchRunner_Search
{
    struct RouterModule_Promise pub;

    /** The router module carrying out the search. */
    struct SearchRunner_pvt* const runner;

    /** The number of requests which have been sent out so far for this search. */
    uint32_t totalRequests;

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
    uint64_t lastNodeAsked;

    /**
     * The timeout if this timeout is hit then the search will continue
     * but the node will still be allowed to respond and it will be counted as a pong.
     */
    struct Timeout* continueSearchTimeout;

    Identity
};

static inline int xorcmp(uint32_t target, uint32_t negativeIfCloser, uint32_t positiveIfCloser)
{
    if (negativeIfCloser == positiveIfCloser) {
        return 0;
    }
    uint32_t ref = Endian_bigEndianToHost32(target);
    return ((Endian_bigEndianToHost32(negativeIfCloser) ^ ref)
               < (Endian_bigEndianToHost32(positiveIfCloser) ^ ref)) ? -1 : 1;
}

/**
 * Return which node is closer to the target.
 *
 * @param target the address to test distance against.
 * @param negativeIfCloser one address to check distance.
 * @param positiveIfCloser another address to check distance.
 * @return -1 if negativeIfCloser is closer to target, 1 if positiveIfCloser is closer
 *         0 if they are both the same distance.
 */
static inline int xorCompare(struct Address* target,
                             struct Address* negativeIfCloser,
                             struct Address* positiveIfCloser)
{
    Address_getPrefix(target);
    Address_getPrefix(negativeIfCloser);
    Address_getPrefix(positiveIfCloser);

    int ret = 0;

    #define COMPARE(part) \
        if ((ret = xorcmp(target->ip6.ints.part,               \
                          negativeIfCloser->ip6.ints.part,     \
                          positiveIfCloser->ip6.ints.part)))   \
        {                                                      \
            return ret;                                        \
        }

    COMPARE(one)
    COMPARE(two)
    COMPARE(three)
    COMPARE(four)

    return 0;

    #undef COMPARE
}

/**
 * Spot a duplicate entry in a node list.
 * If a router sends a response containing duplicate entries,
 * only the last (best) entry should be accepted.
 *
 * @param nodes the list of nodes.
 * @param index the index of the entry to check for being a duplicate.
 * @return true if duplicate, otherwise false.
 */
static inline bool isDuplicateEntry(String* nodes, uint32_t index)
{
    for (uint32_t i = index; i < nodes->len; i += Address_SERIALIZED_SIZE) {
        if (i == index) {
            continue;
        }
        if (Bits_memcmp(&nodes->bytes[index], &nodes->bytes[i], Address_KEY_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

static void searchStep(struct SearchRunner_Search* search);


static void searchCallback(struct RouterModule_Promise* promise,
                           uint32_t lagMilliseconds,
                           struct Node* fromNode,
                           Dict* result)
{
    struct SearchRunner_Search* search =
        Identity_cast((struct SearchRunner_Search*)promise->userData);
    String* nodes = Dict_getString(result, CJDHTConstants_NODES);

    if (nodes && (nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0)) {
        Log_debug(search->runner->logger, "Dropping unrecognized reply");
        return;
    }

    struct VersionList* versions = NULL;
    String* versionsStr = Dict_getString(result, CJDHTConstants_NODE_PROTOCOLS);
    if (versionsStr) {
        versions = VersionList_parse(versionsStr, promise->alloc);
        #ifdef Version_1_COMPAT
            // Version 1 lies about the versions of other nodes, assume they're all v1.
            if (fromNode->version < 2) {
                for (int i = 0; i < (int)versions->length; i++) {
                    versions->versions[i] = 1;
                }
            }
        #endif
    }

    const uint32_t targetPrefix = Address_getPrefix(&search->target);
    const uint32_t parentDistance = Address_getPrefix(&fromNode->address) ^ targetPrefix;

    for (uint32_t i = 0; nodes && i < nodes->len; i += Address_SERIALIZED_SIZE) {
        if (isDuplicateEntry(nodes, i)) {
            continue;
        }
        struct Address addr;
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);

        // calculate the ipv6
        Address_getPrefix(&addr);

        if (!Bits_memcmp(search->runner->myAddress, addr.ip6.bytes, 16)) {
            // Any path which loops back through us is necessarily a dead route.
            NodeStore_brokenPath(addr.path, search->runner->nodeStore);
            continue;
        }

        // We need to splice the given address on to the end of the
        // address of the node which gave it to us.
        addr.path = LabelSplicer_splice(addr.path, fromNode->address.path);

        /*#ifdef Log_DEBUG
            uint8_t splicedAddr[60];
            Address_print(splicedAddr, &addr);
            Log_debug(search->runner->logger, "Spliced Address is now:\n    %s", splicedAddr);
        #endif*/

        if (addr.path == UINT64_MAX) {
            Log_debug(search->runner->logger, "Dropping node because route could not be spliced");
            continue;
        }

        uint32_t newNodePrefix = Address_getPrefix(&addr);
        if (!AddressCalc_validAddress(addr.ip6.bytes)) {
            Log_debug(search->runner->logger, "Was told garbage.\n");
            // This should never happen, badnode.
            break;
        }

        // Nodes we are told about are inserted with 0 reach and assumed version 1.
        uint32_t version = (versions) ? versions->versions[i / Address_SERIALIZED_SIZE] : 1;
        NodeStore_addNode(search->runner->nodeStore, &addr, 0, version);

        if ((newNodePrefix ^ targetPrefix) >= parentDistance
            && xorCompare(&search->target, &addr, &fromNode->address) >= 0)
        {
            // Too much noise.
            //Log_debug(search->runner->logger, "Answer was further from the target than us.\n");
            continue;
        }

        if (search->lastNodeAsked != fromNode->address.path) {
            continue;
        }

        struct Node* n = NodeStore_getBest(&addr, search->runner->nodeStore);
        SearchStore_addNodeToSearch((n) ? &n->address : &addr, search->search);
    }

    if (search->lastNodeAsked != fromNode->address.path) {
        //Log_debug(search->runner->logger, "Late answer in search");
        return;
    }

    if (search->pub.callback) {
        search->pub.callback(&search->pub, lagMilliseconds, fromNode, result);
    }
    searchStep(search);
}

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 */
static void searchStep(struct SearchRunner_Search* search)
{
    struct SearchRunner_pvt* ctx = Identity_cast((struct SearchRunner_pvt*)search->runner);

    struct Node* node;
    struct SearchStore_Node* nextSearchNode;
    do {
        nextSearchNode = SearchStore_getNextNode(search->search);

        // If the number of requests sent has exceeded the max search requests, let's stop there.
        if (search->totalRequests >= MAX_REQUESTS_PER_SEARCH || nextSearchNode == NULL) {
            if (search->pub.callback) {
                search->pub.callback(&search->pub, 0, NULL, NULL);
            }
            Allocator_free(search->pub.alloc);
            return;
        }

        node = NodeStore_getBest(&nextSearchNode->address, ctx->nodeStore);

    } while (!node || Bits_memcmp(node->address.ip6.bytes, nextSearchNode->address.ip6.bytes, 16));

    search->lastNodeAsked = node->address.path;

    struct RouterModule_Promise* rp =
        RouterModule_newMessage(node, 0, ctx->router, search->pub.alloc);

    Dict* message = Dict_new(rp->alloc);
    Dict_putString(message, CJDHTConstants_QUERY, CJDHTConstants_QUERY_FN, rp->alloc);
    Dict_putString(message, CJDHTConstants_TARGET, search->targetStr, rp->alloc);

    rp->userData = search;
    rp->callback = searchCallback;

    RouterModule_sendMessage(rp, message);

    search->totalRequests++;
}

// Triggered by a search timeout (the message may still come back and will be treated as a ping)
static void searchNextNode(void* vsearch)
{
    struct SearchRunner_Search* search = Identity_cast((struct SearchRunner_Search*) vsearch);

    // Timeout for trying the next node.
    Timeout_resetTimeout(search->continueSearchTimeout,
                         RouterModule_searchTimeoutMilliseconds(search->runner->router));

    searchStep(search);
}

struct RouterModule_Promise* SearchRunner_search(uint8_t target[16],
                                                 struct SearchRunner* searchRunner,
                                                 struct Allocator* allocator)
{
    struct SearchRunner_pvt* runner = Identity_cast((struct SearchRunner_pvt*)searchRunner);
    struct Allocator* alloc = Allocator_child(allocator);
    struct SearchStore_Search* sss = SearchStore_newSearch(target, runner->searchStore, alloc);

    struct Address targetAddr;
    Bits_memcpyConst(targetAddr.ip6.bytes, target, Address_SEARCH_TARGET_SIZE);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(runner->nodeStore,
                                  &targetAddr,
                                  NULL,
                                  RouterModule_K,
                                  true,
                                  Version_CURRENT_PROTOCOL,
                                  alloc);

    if (nodes->size == 0) {
        Log_debug(runner->logger, "Can't find any nodes to begin search.");
        Allocator_free(alloc);
        return NULL;
    }

    for (int i = 0; i < (int)nodes->size; i++) {
        SearchStore_addNodeToSearch(&nodes->nodes[i]->address, sss);
    }

    struct SearchRunner_Search* search = Allocator_clone(alloc, (&(struct SearchRunner_Search) {
        .pub = {
            .alloc = alloc
        },
        .runner = runner,
        .search = sss
    }));
    Identity_set(search);
    Bits_memcpyConst(&search->target, &targetAddr, sizeof(struct Address));

    search->targetStr = String_newBinary((char*)targetAddr.ip6.bytes, 16, alloc);

    // this timeout triggers the next search step if the response has not come in.
    search->continueSearchTimeout =
        Timeout_setTimeout(searchNextNode,
                           search,
                           RouterModule_searchTimeoutMilliseconds(runner->router),
                           runner->eventBase,
                           alloc);

    // kick it off
    searchStep(search);

    return &search->pub;
}

struct SearchRunner* SearchRunner_new(struct NodeStore* nodeStore,
                                      struct Log* logger,
                                      struct EventBase* base,
                                      struct RouterModule* module,
                                      uint8_t myAddress[16],
                                      struct Allocator* alloc)
{
    struct SearchRunner_pvt* out = Allocator_clone(alloc, (&(struct SearchRunner_pvt) {
        .nodeStore = nodeStore,
        .logger = logger,
        .eventBase = base,
        .router = module
    }));
    out->searchStore = SearchStore_new(alloc, logger);
    Bits_memcpyConst(out->myAddress, myAddress, 16);
    Identity_set(out);

    return &out->pub;
}
