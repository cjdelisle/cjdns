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
#include "benc/Int.h"
#include "benc/Object.h"
#include "dht/Address.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_struct.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/SearchStore.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "util/Log.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "switch/LabelSplicer.h"
#include "util/AverageRoller.h"
#include "util/Endian.h"
#include "util/Pinger.h"
#include "util/Time.h"
#include "util/Timeout.h"

#include <string.h>
#include <stdint.h>
#include <event2/event.h>
#include <stdbool.h>

/*
 * The router module is the central part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming query, it's job is to add nodes to the reply so that the asking node
 * can find other nodes which are closer to it's target than us.
 *
 * This implementation does not split nodes explicitly into buckets not does it explicitly try to
 * distinguish between "good" and "bad" nodes. Instead it tries to determine which node will help
 * get to the requested record the fastest. Instead of periodicly pinging a random node in each
 * "bucket", this implementation periodically searches for a random[1] hash. When a node is sent a
 * query, the the distance[2] between it and the first node is divided by the amount of time it
 * takes the node to respond, for each successful search, this number is added to an attribute of
 * the node called "reach".
 *
 * Visually representing a node as an area whose location is defined by the node id and it's size is
 * defined by the node reach, you can see that there is a possibility for a record to be closer in
 * key space to node2 while is is still further inside of node1's reach thus node1 is a better
 * choice for the next node to ask.
 *
 * |<--------- Node 1 ---------->|
 *                      |<--- Node 2 ---->|
 *                         ^----- Desired record location.
 *
 * New nodes are inserted into the table but with a reach of 0. It is up to the search client to
 * send search requests to them so they can prove their validity and have their reach number
 * updated.
 *
 * Reach of a node is incremented by 2 every time the node responds to a query and incremented by 1
 * every time a node sends a query of it's own. This has almost no effect except that it means a
 * node which has recently sent data will be preferred over one which has not.
 *
 * When a search is carried out, the next K returned nodes are not necessarily the closest known
 * nodes to the id of the record. The nodes returned will be the nodes with the lowest
 * distance:reach ratio. The distance:reach ratio is calculated by dividing the distance between
 * the node and the record by the node's reach number. Actually it is done by multiplying
 * UINT32_MAX minus the distance by the reach so that it does not need to use slower divison.
 * See: NodeCollector.h
 *
 * Since information about a node becomes stale over time, all reach numbers are decreased by
 * the constant REACH_DECREASE_PER_SECOND times the number of seconds since the last cycle,
 * this operation is performed periodicly every LOCAL_MAINTENANCE_SEARCH_MILLISECONDS unless
 * a local maintainence search is being run which is not often once the network is stable.
 *
 * TODO ---
 * In order to have the nodes with least distance:reach ratio ready to handle any incoming search,
 * we precompute the borders where the "best next node" changes. This computation is best understood
 * by graphing the nodes with their location in keyspace on the X axis and their reach on the Y
 * axis. The border between two nodes, nodeA and nodeB is the location where a line drawn from the
 * X axis up to either node location would be the same angle.
 *
 *  ^                                              ^
 *  |     nodeA                                    |     nodeA
 *  |       |\                                     |       |\__
 *  |       | \                                    |       |   \__
 *  |       |  \    nodeB                          |       |      \nodeB
 *  |       |   \    /|                            |       |         \__
 *  |       |    \  / |                            |       |         |  \__
 *  |       |     \/  |                            |       |         |     \__
 *  +--------------------------------------->      +--------------------------------------->
 *                 ^-- border                                                 ^-- border2
 *
 * Everything to the left of the border and everything to the right of border2 is to be serviced by
 * nodeA. Everything between the two borders is serviced by nodeB. Border2 is found by
 * drawing a line from the point given for nodeA to through the point given for nodeB and finding
 * the intersection of that line with the Y axis. border and border2 are shown on different graphs
 * only to limit clutter, they are the same nodeA and nodeB.
 *
 * When resolving a search, this implementation will lookup the location of the searched for record
 * and return the nodes which belong to the insides of the nearest K borders, this guarantees return
 * of the nodes whose distance:reach ratio is the lowest for that location.
 * ---
 *
 * This implementation must never respond to a search by sending any node who's id is not closer
 * to the target than it's own. Such an event would lead to the possibility of "routing loops" and
 * must be prevented. Searches for which this node has the lowest distance:reach ratio will be
 * replied to with nodes which have 0 reach but are closer than this node or, if there are no such
 * nodes, no nodes at all.
 *
 * The search consumer in this routing module tries to minimize the amount of traffic sent when
 * doing a lookup. To achieve this, it sends a request only to the last node in the search response
 * packet, after the global mean response time has passed without it getting a response, it sends
 * requests to the second to last and so forth, working backward. Nodes which fail to respond in
 * time have their reach immedietly set to zero.
 *
 * The global mean response time is the average amount of time it takes a node to respond to a
 * search query. It is a rolling average over the past 256 seconds.
 *
 * To maximize the quality of service offered by this node this implementation will repeat
 * searches which it handles every number of seconds given by the constant:
 * GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS.
 * Any incoming search with a get_peers request is eligable to be repeated.
 *
 * [1] The implementation runs periodic searches for random hashes but unless the search target
 *     is closer in keyspace to this node than it is to any node with non-zero reach, the search
 *     is not performed. This means that the node will send out lots of searches when it doesn't
 *     know many reliable nodes but it will taper off like a governer as it becomes more
 *     integrated in the network. These searches are run every number of milliseconds given
 *     by the constant LOCAL_MAINTENANCE_SEARCH_MILLISECONDS.
 *
 * [2] If a response "overshoots" the record requested then it is calculated as if it had undershot
 *     by the same amount so as not to provide arbitrage advantage to nodes who return results which
 *     are very far away yet very inaccurate. If it overshoots by more than the distance between the
 *     node and the searched for location (this should never happen), it is considered to be 0.
 */

/*--------------------Constants--------------------*/

/** The number of seconds of time overwhich to calculate the global mean response time. */
#define GMRT_SECONDS 256

/**
 * The number to initialize the global mean response time averager with so that it will
 * return sane results early on, this number can be much higher than the expected average.
 */
#define GMRT_INITAL_MILLISECONDS 5000

/** The number of nodes which we will keep track of. */
#define NODE_STORE_SIZE 8192

/** The number of milliseconds between attempting local maintenance searches. */
#define LOCAL_MAINTENANCE_SEARCH_MILLISECONDS 1000

/**
 * The number of milliseconds to pass between global maintainence searches.
 * These are searches for random targets which are used to discover new nodes.
 */
#define GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS 30000

#define SEARCH_REPEAT_MILLISECONDS 7500

/** The maximum number of requests to make before calling a search failed. */
#define MAX_REQUESTS_PER_SEARCH 8

/**
 * The number of times the GMRT before pings should be timed out.
 */
#define PING_TIMEOUT_GMRT_MULTIPLIER 100

/**
 * The minimum amount of time before a ping should timeout.
 */
#define PING_TIMEOUT_MINIMUM 3000

#define LINK_STATE_MULTIPLIER 536870

/*--------------------Structures--------------------*/
/**
 * A structure to give the user when performing a search so they can cancel it.
 */
struct RouterModule_Search
{
    struct SearchStore_Search* search;
};

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static int handleOutgoing(struct DHTMessage* message, void* vcontext);

static void pingNode(Dict* input, void* vrouter, String* txid);

/*--------------------Interface--------------------*/

/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param myAddress the address for this DHT node.
 * @return the RouterModule.
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct event_base* eventBase,
                                           struct Log* logger,
                                           struct Admin* admin)
{
    struct RouterModule* const out = allocator->calloc(sizeof(struct RouterModule), 1, allocator);

    DHTModuleRegistry_register(allocator->clone(sizeof(struct DHTModule),
                                                allocator,
                                                &(struct DHTModule)
    {
        .name = "RouterModule",
        .context = out,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }), registry);

    Hex_decode(out->gitVersionBytes, 20, (uint8_t*) GIT_VERSION, 40);
    out->gitVersion.len = 20;
    out->gitVersion.bytes = (char*) out->gitVersionBytes;

    Address_forKey(&out->address, myAddress);

    out->gmrtRoller = AverageRoller_new(GMRT_SECONDS, eventBase, allocator);
    AverageRoller_update(out->gmrtRoller, GMRT_INITAL_MILLISECONDS);
    out->searchStore = SearchStore_new(allocator, out->gmrtRoller, eventBase, logger);
    out->nodeStore = NodeStore_new(&out->address, NODE_STORE_SIZE, allocator, logger, admin);
    out->registry = registry;
    out->eventBase = eventBase;
    out->logger = logger;
    out->allocator = allocator;
    out->admin = admin;
    out->janitor = Janitor_new(LOCAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               out,
                               out->nodeStore,
                               allocator,
                               eventBase);

    struct Admin_FunctionArg adma[2] = {
        { .name = "path", .required = 1, .type = "String" },
        { .name = "timeout", .required = 0, .type = "Int" },
    };
    Admin_registerFunction("RouterModule_pingNode", pingNode, out, true, adma, admin);

    return out;
}

struct RouterModule_Ping
{
    struct RouterModule* module;
    struct Node* node;
    struct Timeout* timeout;
    uint8_t txid[4];
    bool isFromAdmin;
    uint32_t timeSent;
    struct Allocator* allocator;
};

#define BSTR(x) (&(String) { .bytes = x, .len = strlen(x) })
static void pingNode(Dict* args, void* vrouter, String* txid)
{
    struct RouterModule* router = (struct RouterModule*) vrouter;
    String* pathStr = Dict_getString(args, BSTR("path"));
    int64_t* timeoutPtr = Dict_getInt(args, String_CONST("timeout"));
    uint32_t timeout = (timeoutPtr && *timeoutPtr > 0) ? *timeoutPtr : 0;

    #define ERROR_DICT(x) Dict_CONST(String_CONST("error"), String_OBJ(String_CONST(x)), NULL)
    Dict err = NULL;
    struct Address addr;
    struct Node* n = NULL;

    if (pathStr->len == 19) {
        if (Address_parsePath(&addr.networkAddress_be, (uint8_t*) pathStr->bytes)) {
            err = ERROR_DICT("parse path failed");
        } else {
            n = RouterModule_getNode(addr.networkAddress_be, router);
        }
    } else if (pathStr->len == 39) {
        if (Address_parseIp(addr.ip6.bytes, (uint8_t*) pathStr->bytes)) {
            err = ERROR_DICT("parsing address failed");
        } else {
            n = RouterModule_lookup(addr.ip6.bytes, router);
        }
    } else {
        err = ERROR_DICT("Unexpected length, must be either 39 char ipv6 address "
                         "(with leading zeros) eg: 'fc4f:000d:e499:8f5b:c49f:6e6b:01ae:3120' "
                         "or 19 char path eg: '0123.4567.89ab.cdef'");
    }

    if (!err) {
        if (!n || memcmp(addr.ip6.bytes, n->address.ip6.bytes, 16)) {
            err = ERROR_DICT("could not find node to ping");
        } else if (RouterModule_pingNode(n, router, timeout, txid)) {
            err = ERROR_DICT("no open slots to store ping, try later.");
        }
    }

    if (err) {
        Admin_sendMessage(&err, txid, router->admin);
    }
}

/**
 * The amount of time to wait before skipping over the first node and trying another in a search.
 * Any node which can't beat this time will have it's reach set to 0.
 *
 * @param module this module.
 * @return the timeout time.
 */
static inline uint64_t tryNextNodeAfter(struct RouterModule* module)
{
    uint64_t x = (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 4);
    return x + (rand() % (x | 1)) / 2;
}

/**
 * Get the time where any unreplied requests older than that should be timed out.
 * This implementation times out after twice the global mean response time.
 *
 * @param module this module.
 * @return the timeout time.
 */
static inline uint64_t evictUnrepliedIfOlderThan(struct RouterModule* module)
{
    return Time_currentTimeMilliseconds(module->eventBase) - tryNextNodeAfter(module);
}

/**
 * Calculate "how far this node got us" in our quest for a given record.
 *
 * When we ask node Alice a search query to find a record,
 * if she replies with a node which is further from the target than her, we are backpeddling,
 * Alice is not compliant and we will return 0 distance because her reach should become zero asap.
 *
 * If Alice responds with a node that is further from her than she is from the target, then she
 * has "overshot the target" so to speak, we return the distance between her and the node minus
 * the distance between the node and the target.
 *
 * If alice returns a node which is between her and the target, we just return the distance between
 * her and the node.
 *
 * @param nodeIdPrefix the first 4 bytes of Alice's node id in host order.
 * @param targetPrefix the first 4 bytes of the target id in host order.
 * @param firstResponseIdPrefix the first 4 bytes of the id of
 *                              the first node to respond in host order.
 * @return a number between 0 and UINT32_MAX representing the distance in keyspace which this
 *         node has helped us along.
 */
static inline uint32_t calculateDistance(const uint32_t nodeIdPrefix,
                                         const uint32_t targetPrefix,
                                         const uint32_t firstResponseIdPrefix)
{
    // Distance between Alice and the target.
    uint32_t at = nodeIdPrefix ^ targetPrefix;

    // Distance between Bob and the target.
    uint32_t bt = firstResponseIdPrefix ^ targetPrefix;

    if (bt > at) {
        // Alice is giving us nodes which are further from the target than her :(
        return 0;
    }

    // Distance between Alice and Bob.
    uint32_t ab = nodeIdPrefix ^ firstResponseIdPrefix;

    if (at < ab) {
        // Alice gave us a node which is beyond the target,
        // this is fine but should not be unjustly rewarded.
        return ab - bt;
    }

    // Alice gave us a node which is between her and the target.
    return ab;
}

/**
 * Send off a query to another node.
 *
 * @param address the address to send the query to.
 * @param queryType what type of query eg: find_node or get_peers.
 * @param transactionId the tid to send with the query.
 * @param searchTarget the thing which we are looking for or null if it's a ping.
 * @param targetKey the key underwhich to send the target eg: target or info_hash
 * @param registry the DHT module registry to use for sending the message.
 */
static inline void sendRequest(struct Address* address,
                               String* queryType,
                               String* transactionId,
                               struct Address* searchTarget,
                               String* targetKey,
                               struct DHTModuleRegistry* registry)
{
    struct DHTMessage message;
    memset(&message, 0, sizeof(struct DHTMessage));

    char buffer[4096];
    const struct Allocator* allocator = BufferAllocator_new(buffer, 4096);

    message.allocator = allocator;
    message.asDict = Dict_new(allocator);

    // "t":"1234"
    Dict_putString(message.asDict, CJDHTConstants_TXID, transactionId, allocator);

    if (searchTarget != NULL) {
        // Otherwise we're sending a ping.
        Dict_putString(message.asDict,
                       targetKey,
                       &(String) { .bytes = (char*) searchTarget->ip6.bytes,
                                   .len = Address_SEARCH_TARGET_SIZE },
                       allocator);
    }

    /* "q":"fn" */
    Dict_putString(message.asDict, CJDHTConstants_QUERY, queryType, allocator);

    message.address = address;

    DHTModuleRegistry_handleOutgoing(&message, registry);
}

/**
 * A context for the internals of a search.
 */
struct SearchCallbackContext
{
    /** The router module carrying out the search. */
    struct RouterModule* const routerModule;

    /** The callback to call with results. */
    bool (* const resultCallback)(void* callbackContext, struct DHTMessage* result);

    /** The number of requests which have been sent out so far for this search. */
    uint32_t totalRequests;

    /** A context to pass when calling the results callback. */
    void* const resultCallbackContext;

    /** The address which we are searching for. */
    struct Address targetAddress;

    /** The timeout event for this search. */
    struct Timeout* const timeout;

    /**
     * The SearchStore_Search structure for this search,
     * used to keep track of which nodes are participating.
     */
    struct SearchStore_Search* const search;

    /** The type of query which we send in this search IE: find_node or get_peers. */
    String* const requestType;

    String* const targetKey;

    struct SearchStore_Node* lastNodeCalled;

    /** The number of milliseconds before timing out a request. */
    uint64_t timeoutMilliseconds;
};

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 *
 * @param scc the SearchCallbackContext for this search.
 */
static void searchStep(struct SearchCallbackContext* scc)
{
    struct RouterModule* module = scc->routerModule;
    struct Allocator* searchAllocator = SearchStore_getAllocator(scc->search);

    struct SearchStore_Node* nextSearchNode =
        SearchStore_getNextNode(scc->search, searchAllocator);

    // If the number of requests sent has exceeded the max search requests, let's stop there.
    if (scc->totalRequests >= MAX_REQUESTS_PER_SEARCH || nextSearchNode == NULL) {
        if (scc->resultCallback != NULL) {
            scc->resultCallback(scc->resultCallbackContext, NULL);
        }
        SearchStore_freeSearch(scc->search);
        return;
    }

    // Get the node from the nodestore because there might be a much better path to the same node.
    struct Node* n = NodeStore_getBest(nextSearchNode->address, scc->routerModule->nodeStore);
    if (n && !memcmp(n->address.ip6.bytes, nextSearchNode->address->ip6.bytes, 16)) {
        uint64_t nlabel = Endian_bigEndianToHost64(n->address.networkAddress_be);
        uint64_t nsn = Endian_bigEndianToHost64(nextSearchNode->address->networkAddress_be);
        if (nlabel < nsn) {
            nextSearchNode->address = &n->address;
        }
    }

    sendRequest(nextSearchNode->address,
                scc->requestType,
                SearchStore_tidForNode(nextSearchNode, searchAllocator),
                &scc->targetAddress,
                scc->targetKey,
                scc->routerModule->registry);

    scc->totalRequests++;
    scc->lastNodeCalled = nextSearchNode;
    SearchStore_requestSent(nextSearchNode, module->searchStore);
    scc->timeoutMilliseconds = tryNextNodeAfter(module);
    Timeout_resetTimeout(scc->timeout, scc->timeoutMilliseconds);
}

/**
 * Callback for when a search has timed out.
 *
 * @param vcontext the SearchCallbackContext, cast to void.
 */
static void searchRequestTimeout(void* vcontext)
{
    struct SearchCallbackContext* scc = (struct SearchCallbackContext*) vcontext;
    struct Node* n = NodeStore_getNodeByNetworkAddr(scc->lastNodeCalled->address->networkAddress_be,
                                                    scc->routerModule->nodeStore);

    // Search timeout -> set to 0 reach.
    if (n) {

        #ifdef Log_DEBUG
            uint8_t addr[60];
            Address_print(addr, &n->address);
            Log_debug2(scc->routerModule->logger,
                       "Search timeout for %s, after %lums. halving reach\n",
                       addr,
                       (unsigned long)scc->timeoutMilliseconds);
        #endif

        n->reach /= 2;
        NodeStore_updateReach(n, scc->routerModule->nodeStore);
    }

    searchStep(scc);
}

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
        if (memcmp(&nodes->bytes[index], &nodes->bytes[i], Address_KEY_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

static inline void pingResponse(struct RouterModule_Ping* ping,
                                bool timeout,
                                uint32_t lag,
                                String* versionBin,
                                struct RouterModule* module)
{
    Dict versionDict = NULL;
    String* result = String_CONST("timeout");
    if (!timeout) {
        String* version = String_CONST("old");
        result = String_CONST("pong");
        versionDict = Dict_CONST(String_CONST("version"), String_OBJ(version), NULL);
        if (versionBin && versionBin->len == 20) {
            uint8_t versionStr[40];
            Hex_encode(versionStr, 40, (uint8_t*) versionBin->bytes, 20);
            version->bytes = (char*) versionStr;
            version->len = 40;
        }
    }

    Dict response = Dict_CONST(
        String_CONST("ms"), Int_OBJ(lag), Dict_CONST(
        String_CONST("result"), String_OBJ(result), versionDict
    ));

    String txid = { .len = 4, .bytes = (char*)ping->txid };
    Admin_sendMessage(&response, &txid, module->admin);
}

static inline void responseFromNode(struct Node* node,
                                    uint32_t millisecondsSinceRequest,
                                    struct RouterModule* module)
{
    if (node) {
        uint64_t worst = tryNextNodeAfter(module);
        node->reach = (worst - millisecondsSinceRequest) * LINK_STATE_MULTIPLIER;
        NodeStore_updateReach(node, module->nodeStore);
    }
}

/**
 * Handle an incoming reply to one of our queries.
 * This will handle the reply on the incoming direction.
 *
 * @param message the incoming reply message.
 * @param module the router module context.
 * @return 0 if the reply should be allowed to run through the rest of the modules,
 *         -1 if it is blatently invalid and should be stopped.
 */
static inline int handleReply(struct DHTMessage* message, struct RouterModule* module)
{
    // this implementation only pings to get the address of a node, so lets add the node.
    struct Node* node = NodeStore_addNode(module->nodeStore, message->address, 0);

    String* tid = Dict_getString(message->asDict, CJDHTConstants_TXID);

    if (!tid) {
        return -1;
    }

    String* nodes = Dict_getString(message->asDict, CJDHTConstants_NODES);
    if (nodes == NULL && tid && tid->len == 2 && tid->bytes[0] == 'p') {
        uint8_t index = tid->bytes[1];
        if (index < RouterModule_MAX_CONCURRENT_PINGS && module->pings[index] != NULL) {
            #ifdef Log_DEBUG
                uint8_t printedAddr[60];
                Address_print(printedAddr, message->address);
                Log_debug1(module->logger, "Got pong! %s\n", printedAddr);
            #endif

            Timeout_clearTimeout(module->pings[index]->timeout);

            uint32_t lag =
                Time_currentTimeMilliseconds(module->eventBase) - module->pings[index]->timeSent;
            responseFromNode(node, lag, module);

            if (module->pings[index]->isFromAdmin) {
                String* version = Dict_getString(message->asDict, CJDHTConstants_VERSION);
                pingResponse(module->pings[index], false, lag, version, module);
            }

            struct Allocator* pa = module->pings[index]->allocator;
            pa->free(pa);
            module->pings[index] = NULL;

        } else {
            Log_debug1(module->logger,
                       "Looks like a ping but unrecognized slot. slot=%u\n",
                       (int) index);
        }
        return 0;
    } else if (nodes && (nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0)) {
        Log_debug(module->logger, "Dropping unrecognized reply.\n");
        return -1;
    }

    struct SearchStore_Node* parent =
        SearchStore_getNode(tid, module->searchStore, message->allocator);

    if (parent == NULL) {
        // Couldn't find the node, perhaps we were sent a malformed packet.
        return -1;
    }

    // If the search has already replaced the node's location or it has already finished
    // and another search is taking place in the same slot, drop this reply because it is late.
    if (!Address_isSameIp(parent->address, message->address)) {
        #ifdef Log_DEBUG
            uint8_t expectedAddr[60];
            Address_print(expectedAddr, parent->address);
            uint8_t receivedAddr[60];
            Address_print(receivedAddr, message->address);
            Log_debug2(module->logger,
                       "Got return packet from different address than search was sent!\n"
                       "Expected:%s\n"
                       "     Got:%s\n",
                       expectedAddr,
                       receivedAddr);
        #endif
        return -1;
    }

    responseFromNode(node, SearchStore_replyReceived(parent, module->searchStore), module);

    struct SearchStore_Search* search = SearchStore_getSearchForNode(parent, module->searchStore);
    struct SearchCallbackContext* scc = SearchStore_getContext(search);

    if (!nodes) {
        // They didn't have anything to give us.
        searchStep(scc);
        return 0;
    }

    // If this node has sent us any entries which are further from the target than it is,
    // garbage the whole response.
    uint32_t targetPrefix = Address_getPrefix(&scc->targetAddress);
    uint32_t parentDistance = Address_getPrefix(parent->address) ^ targetPrefix;
    uint32_t ourAddressPrefix = Address_getPrefix(&module->address);

    uint64_t evictTime = evictUnrepliedIfOlderThan(module);
    for (uint32_t i = 0; i < nodes->len; i += Address_SERIALIZED_SIZE) {
        if (isDuplicateEntry(nodes, i)) {
            continue;
        }
        struct Address addr;
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);
        uint32_t newNodePrefix = Address_getPrefix(&addr);

        if (newNodePrefix == ourAddressPrefix
            && memcmp(module->address.ip6.bytes, addr.ip6.bytes, Address_SEARCH_TARGET_SIZE) == 0)
        {
            // This happens constantly.
            //Log_debug(module->logger, "They just told us about ourselves.\n");
            continue;
        } else if (addr.ip6.bytes[0] != 0xfc) {
            Log_debug(module->logger, "Was told garbage.\n");
            // This should never happen, badnode.
            break;
        }

        /*#ifdef Log_DEBUG
            uint8_t fromAddr[60];
            uint8_t newAddr[60];
            Address_print(fromAddr, message->address);
            Address_print(newAddr, &addr);
            Log_debug2(module->logger,
                       "Discovered new node:\n    %s\nvia:%s\n",
                       newAddr,
                       fromAddr);
        #endif*/

        // We need to splice the given address on to the end of the
        // address of the node which gave it to us.
        addr.networkAddress_be = LabelSplicer_splice(addr.networkAddress_be,
                                                     message->address->networkAddress_be);

        /*#ifdef Log_DEBUG
            uint8_t splicedAddr[60];
            Address_print(splicedAddr, &addr);
            Log_debug1(module->logger, "Spliced Address is now:\n    %s\n", splicedAddr);
        #endif*/

        if (addr.networkAddress_be == UINT64_MAX) {
            Log_debug(module->logger, "Dropping node because route could not be spliced.\n");
            continue;
        }

        // Nodes we are told about are inserted with 0 reach.
        NodeStore_addNode(module->nodeStore, &addr, 0);

        if ((newNodePrefix ^ targetPrefix) >= parentDistance
            && xorCompare(&scc->targetAddress, &addr, parent->address) >= 0)
        {
            // Too much noise.
            //Log_debug(module->logger, "Answer was further from the target than us.\n");
            continue;
        }

        struct Node* n = NodeStore_getBest(&addr, module->nodeStore);
        SearchStore_addNodeToSearch(parent, (n) ? &n->address : &addr, evictTime, search);
    }

    // Ask the callback if we should continue.
    if (scc->resultCallback == NULL
        || scc->resultCallback(scc->resultCallbackContext, message) == false)
    {
        searchStep(scc);
    } else {
        SearchStore_freeSearch(search);
    }
    return 0;
}

/**
 * The only type of message we handle on the incoming side is
 * a response to one of our queries.
 */
static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    if (Dict_getString(message->asDict, CJDHTConstants_QUERY) == NULL) {
        return handleReply(message, (struct RouterModule*) vcontext);
    } else {
        return 0;
    }
}

/**
 * Handle an incoming search query.
 * This is setup to handle the outgoing *response* to the query, it should
 * be called from handleOutgoing() and populate the response with nodes.
 *
 * @param message the empty response message to populate.
 * @param replyArgs the arguments dictionary in the response (to be populated).
 * @param module the routing module context.
 * @return 0 as long as the packet should not be stopped (at this point always 0).
 */
static inline int handleQuery(struct DHTMessage* message,
                              struct RouterModule* module)
{
    struct DHTMessage* query = message->replyTo;

    // We got a query, the reach should be set to 1 in the new node.
    NodeStore_addNode(module->nodeStore, query->address, 1);

    // get the target
    String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
    if (target == NULL || target->len != Address_SEARCH_TARGET_SIZE) {
        // No target, probably a ping, tell them the version.
        Dict_putString(message->asDict,
                       CJDHTConstants_VERSION,
                       &module->gitVersion,
                       message->allocator);
        return 0;
    }

    struct Address targetAddr;
    memcpy(targetAddr.ip6.bytes, target->bytes, Address_SEARCH_TARGET_SIZE);

    // send the closest nodes
    struct NodeList* nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                                          &targetAddr,
                                                          query->address,
                                                          RouterModule_K + 5,
                                                          false,
                                                          message->allocator);

    String* nodes = message->allocator->malloc(sizeof(String), message->allocator);
    nodes->len = nodeList->size * Address_SERIALIZED_SIZE;
    nodes->bytes = message->allocator->malloc(nodeList->size * Address_SERIALIZED_SIZE,
                                              message->allocator);

    uint32_t i;
    for (i = 0; i < nodeList->size; i++) {

        // We have to modify the reply in case this node uses a longer label discriminator
        // in our switch than it's target address, the target address *must* have the same
        // length or longer.
        struct Address addr;
        memcpy(&addr, &nodeList->nodes[i]->address, sizeof(struct Address));

        addr.networkAddress_be = LabelSplicer_getLabelFor(addr.networkAddress_be,
                                                          query->address->networkAddress_be);

        Address_serialize((uint8_t*) &nodes->bytes[i * Address_SERIALIZED_SIZE], &addr);
    }
    if (i > 0) {
        Dict_putString(message->asDict, CJDHTConstants_NODES, nodes, message->allocator);
    }

    return 0;
}

/**
 * We handle 2 kinds of packets on the outgoing.
 * 1. our requests
 * 2. our replies to others' requests.
 * Everything is tagged with our address, replies to requests which are not ping requests
 * will also be given a list of nodes.
 */
static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct RouterModule* module = (struct RouterModule*) vcontext;

    if (message->replyTo != NULL) {
        return handleQuery(message, module);
    }

    return 0;
}

/** See: RouterModule.h */
struct RouterModule_Search* RouterModule_beginSearch(
    const uint8_t searchTarget[Address_SEARCH_TARGET_SIZE],
    bool (* const callback)(void* callbackContext, struct DHTMessage* result),
    void* callbackContext,
    struct RouterModule* module)
{
    struct SearchStore_Search* search = SearchStore_newSearch(searchTarget, module->searchStore);
    if (!search) {
        return NULL;
    }
    struct Allocator* searchAllocator = SearchStore_getAllocator(search);

    struct Address targetAddr;
    memcpy(targetAddr.ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(module->nodeStore,
                                  &targetAddr,
                                  NULL,
                                  RouterModule_K,
                                  true,
                                  searchAllocator);

    if (nodes->size == 0) {
        // no nodes found!
        return NULL;
    }

    for (uint32_t i = 0; i < nodes->size; i++) {
        SearchStore_addNodeToSearch(NULL,
                                    &nodes->nodes[i]->address,
                                    evictUnrepliedIfOlderThan(module),
                                    search);
    }

    struct SearchStore_Node* firstSearchNode = SearchStore_getNextNode(search, searchAllocator);

    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, firstSearchNode->address);
        Log_debug1(module->logger, "Search %s\n", addr);
    #endif

    // Send out the request.
    sendRequest(firstSearchNode->address,
                CJDHTConstants_QUERY_FN,
                SearchStore_tidForNode(firstSearchNode, searchAllocator),
                &targetAddr,
                CJDHTConstants_TARGET,
                module->registry);

    SearchStore_requestSent(firstSearchNode, module->searchStore);

    struct SearchCallbackContext* scc =
        searchAllocator->malloc(sizeof(struct SearchCallbackContext), searchAllocator);

    scc->timeoutMilliseconds = tryNextNodeAfter(module);

    struct Timeout* timeout = Timeout_setTimeout(searchRequestTimeout,
                                                 scc,
                                                 scc->timeoutMilliseconds,
                                                 module->eventBase,
                                                 searchAllocator);

    struct SearchCallbackContext sccLocal = {
        .routerModule = module,
        .resultCallback = callback,
        .resultCallbackContext = callbackContext,
        .timeout = timeout,
        .search = search,
        .requestType = CJDHTConstants_QUERY_FN,
        .targetKey = CJDHTConstants_TARGET,
        .lastNodeCalled = firstSearchNode,
    };
    memcpy(scc, &sccLocal, sizeof(struct SearchCallbackContext));
    memcpy(&scc->targetAddress, &targetAddr, sizeof(struct Address));

    SearchStore_setContext(scc, search);

    struct RouterModule_Search* out =
        searchAllocator->malloc(sizeof(struct RouterModule_Search), searchAllocator);
    out->search = search;

    return out;
}

/** See: RouterModule.h */
void RouterModule_cancelSearch(struct RouterModule_Search* toCancel)
{
    SearchStore_freeSearch(toCancel->search);
}

int RouterModule_brokenPath(const uint64_t networkAddress_be, struct RouterModule* module)
{
    uint64_t path = Endian_bigEndianToHost64(networkAddress_be);
    return NodeStore_brokenPath(path, module->nodeStore);
}

static void pingTimeoutCallback(void* vping)
{
    struct RouterModule_Ping* ping = (struct RouterModule_Ping*) vping;
    struct RouterModule* module = ping->module;

    for (int i = 0; i < RouterModule_MAX_CONCURRENT_PINGS; i++) {
        if (ping == module->pings[i]) {
            #ifdef Log_DEBUG
                uint8_t addr[60];
                Address_print(addr, &ping->node->address);
                Log_debug1(module->logger, "Ping timeout %s\n", addr);
            #endif
            // If this node has already been invalidated by another ping timeout then
            // it's addr is 0 and calling brokenpath on it flushes one-hop nodes out.
            if (ping->node->address.networkAddress_be != 0) {
                RouterModule_brokenPath(ping->node->address.networkAddress_be, module);
            }
            module->pings[i] = NULL;
            break;
        }
    }

    uint32_t lag = Time_currentTimeMilliseconds(module->eventBase) - ping->timeSent;

    if (ping->isFromAdmin) {
        pingResponse(ping, true, lag, NULL, module);
    }

    ping->allocator->free(ping->allocator);
}

/** See: RouterModule.h */
int RouterModule_pingNode(struct Node* node,
                          struct RouterModule* module,
                          uint32_t timeoutMilliseconds,
                          String* txid)
{
    struct RouterModule_Ping** location = NULL;
    uint8_t index;
    for (index = 0; index < RouterModule_MAX_CONCURRENT_PINGS; index++) {
        if (module->pings[index] == NULL) {
            location = &module->pings[index];
            break;
        }
    }

    if (location == NULL) {
        return -1;
    }

    #ifdef Log_DEBUG
        uint8_t addr[60];
        Address_print(addr, &node->address);
        Log_debug1(module->logger, "Ping %s\n", addr);
    #endif

    struct Allocator* pingAllocator = module->allocator->child(module->allocator);
    struct RouterModule_Ping* ping =
        pingAllocator->malloc(sizeof(struct RouterModule_Ping), pingAllocator);
    *location = ping;
    ping->node = node;
    ping->module = module;
    ping->allocator = pingAllocator;

    if (timeoutMilliseconds == 0) {
        timeoutMilliseconds =
            AverageRoller_getAverage(module->gmrtRoller) * PING_TIMEOUT_GMRT_MULTIPLIER;
        if (timeoutMilliseconds < PING_TIMEOUT_MINIMUM) {
            timeoutMilliseconds = PING_TIMEOUT_MINIMUM;
        }
    }

    ping->timeout = Timeout_setTimeout(pingTimeoutCallback,
                                       ping,
                                       timeoutMilliseconds,
                                       module->eventBase,
                                       pingAllocator);

    ping->timeSent = Time_currentTimeMilliseconds(module->eventBase);

    uint8_t txidBuff[2];
    txidBuff[0] = 'p';
    txidBuff[1] = index;
    if (txid) {
        memcpy(ping->txid, txid->bytes, 4);
    }

    // Assume that if the ping has a txid, then it's from the admin interface.
    ping->isFromAdmin = (txid != NULL);

    sendRequest(&node->address,
                CJDHTConstants_QUERY_PING,
                &(String) { .len = 2, .bytes = (char*) txidBuff },
                NULL,
                NULL,
                module->registry);
    return 0;
}

/** See: RouterModule.h */
void RouterModule_addNode(struct Address* address, struct RouterModule* module)
{
    Address_getPrefix(address);
    NodeStore_addNode(module->nodeStore, address, 0);
    struct Node* best = RouterModule_lookup(address->ip6.bytes, module);
    if (best && best->address.networkAddress_be != address->networkAddress_be) {
        RouterModule_pingNode(best, module, 0, NULL);
    }
}

struct Node* RouterModule_lookup(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                 struct RouterModule* module)
{
    struct Address addr;
    memcpy(addr.ip6.bytes, targetAddr, Address_SEARCH_TARGET_SIZE);

    return NodeStore_getBest(&addr, module->nodeStore);
}

/** see RouterModule.h */
struct Node* RouterModule_getNode(uint64_t path, struct RouterModule* module)
{
    return NodeStore_getNodeByNetworkAddr(path, module->nodeStore);
}
