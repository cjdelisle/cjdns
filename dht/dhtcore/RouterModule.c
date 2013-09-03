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
#define string_strlen

#include "benc/Int.h"
#include "crypto/AddressCalc.h"
#include "dht/Address.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/RouterModule_pvt.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/SearchStore.h"
#include "dht/dhtcore/VersionList.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "memory/BufferAllocator.h"
#include "switch/LabelSplicer.h"
#include "util/events/EventBase.h"
#include "util/AverageRoller.h"
#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Pinger.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"
#include "util/platform/libc/string.h"

#include <stdint.h>
#include <stdbool.h>

/*
 * The router module is the central part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming query, its job is to add nodes to the reply so that the asking node
 * can find other nodes which are closer to its target than us.
 *
 * This implementation does not split nodes explicitly into buckets not does it explicitly try to
 * distinguish between "good" and "bad" nodes. Instead it tries to determine which node will help
 * get to the requested record the fastest. Instead of periodicly pinging a random node in each
 * "bucket", this implementation periodically searches for a random[1] hash. When a node is sent a
 * query, the the distance[2] between it and the first node is divided by the amount of time it
 * takes the node to respond, for each successful search, this number is added to an attribute of
 * the node called "reach".
 *
 * Visually representing a node as an area whose location is defined by the node id and its size is
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
 * every time a node sends a query of its own. This has almost no effect except that it means a
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
 * to the target than its own. Such an event would lead to the possibility of "routing loops" and
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

/** The number of times the GMRT before pings should be timed out. */
#define PING_TIMEOUT_GMRT_MULTIPLIER 100

/** The minimum amount of time before a ping should timeout. */
#define PING_TIMEOUT_MINIMUM 3000

/** You are not expected to understand this. */
#define LINK_STATE_MULTIPLIER 536870

/** All searches will be killed after this amount of time nomatter how bad the GMRT is. */
#define MAX_TIMEOUT 10000

/** Never allow a search to be timed out in less than this number of milliseconds. */
#define MIN_TIMEOUT 10

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message, void* vcontext);
static int handleOutgoing(struct DHTMessage* message, void* vcontext);


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
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Admin* admin,
                                           struct Random* rand)
{
    struct RouterModule* const out = Allocator_calloc(allocator, sizeof(struct RouterModule), 1);

    struct DHTModule* dm = Allocator_clone(allocator, (&(struct DHTModule) {
        .name = "RouterModule",
        .context = out,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }));
    DHTModuleRegistry_register(dm, registry);

    Hex_decode(out->gitVersionBytes, 20, Version_gitVersion(), 40);
    out->gitVersion.len = 20;
    out->gitVersion.bytes = (char*) out->gitVersionBytes;

    Address_forKey(&out->address, myAddress);

    out->gmrtRoller = AverageRoller_new(GMRT_SECONDS, eventBase, allocator);
    AverageRoller_update(out->gmrtRoller, GMRT_INITAL_MILLISECONDS);
    out->searchStore = SearchStore_new(allocator, logger);
    out->nodeStore = NodeStore_new(&out->address, NODE_STORE_SIZE, allocator, logger, rand, admin);
    out->registry = registry;
    out->eventBase = eventBase;
    out->logger = logger;
    out->allocator = allocator;
    out->admin = admin;
    out->rand = rand;
    out->pinger = Pinger_new(eventBase, logger, allocator);
    out->janitor = Janitor_new(LOCAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               out,
                               out->nodeStore,
                               allocator,
                               eventBase,
                               rand);
    Identity_set(out);
    return out;
}

/**
 * The amount of time to wait before skipping over the first node and trying another in a search.
 * Any node which can't beat this time will have its reach set to 0.
 *
 * @param module this module.
 * @return the timeout time.
 */
static inline uint64_t tryNextNodeAfter(struct RouterModule* module)
{
    uint64_t x = (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 4);
    x = x + (Random_uint32(module->rand) % (x | 1)) / 2;
    return (x > MAX_TIMEOUT) ? MAX_TIMEOUT : (x < MIN_TIMEOUT) ? MIN_TIMEOUT : x;
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
    Bits_memset(&message, 0, sizeof(struct DHTMessage));

    char buffer[4096];
    struct Allocator* allocator = BufferAllocator_new(buffer, 4096);

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

static uint32_t reachAfterTimeout(const uint32_t oldReach)
{
    switch (oldReach) {
        case 2: return 1;
        case 1:
        case 0: return 0;
        default: return oldReach / 2 + 2;
    }
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
        if (Bits_memcmp(&nodes->bytes[index], &nodes->bytes[i], Address_KEY_SIZE) == 0) {
            return true;
        }
    }
    return false;
}

static inline void responseFromNode(struct Node* node,
                                    uint32_t millisecondsSinceRequest,
                                    struct RouterModule* module)
{
    if (node) {
        uint64_t worst = tryNextNodeAfter(module);
        if (worst > millisecondsSinceRequest) {
            node->reach = (worst - millisecondsSinceRequest)  * LINK_STATE_MULTIPLIER;
            NodeStore_updateReach(node, module->nodeStore);
        }
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

    int64_t* versionPtr = Dict_getInt(query->asDict, CJDHTConstants_PROTOCOL);
    uint32_t version = (versionPtr && *versionPtr <= UINT32_MAX) ? *versionPtr : 0;

    // We got a query, the reach should be set to 1 in the new node.
    NodeStore_addNode(module->nodeStore, query->address, 1, version);

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
    Bits_memcpyConst(targetAddr.ip6.bytes, target->bytes, Address_SEARCH_TARGET_SIZE);

    // send the closest nodes
    struct NodeList* nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                                          &targetAddr,
                                                          query->address,
                                                          RouterModule_K + 5,
                                                          false,
                                                          version,
                                                          message->allocator);

    String* nodes = Allocator_malloc(message->allocator, sizeof(String));
    nodes->len = nodeList->size * Address_SERIALIZED_SIZE;
    nodes->bytes = Allocator_malloc(message->allocator, nodeList->size * Address_SERIALIZED_SIZE);

    struct VersionList* versions = VersionList_new(nodeList->size, message->allocator);

    uint32_t i;
    for (i = 0; i < nodeList->size; i++) {

        // We have to modify the reply in case this node uses a longer label discriminator
        // in our switch than its target address, the target address *must* have the same
        // length or longer.
        struct Address addr;
        Bits_memcpyConst(&addr, &nodeList->nodes[i]->address, sizeof(struct Address));

        addr.path = LabelSplicer_getLabelFor(addr.path, query->address->path);

        Address_serialize((uint8_t*) &nodes->bytes[i * Address_SERIALIZED_SIZE], &addr);

        versions->versions[i] = nodeList->nodes[i]->version;
    }
    if (i > 0) {
        Dict_putString(message->asDict, CJDHTConstants_NODES, nodes, message->allocator);
        String* versionsStr = VersionList_stringify(versions, message->allocator);
        Dict_putString(message->asDict,
                       CJDHTConstants_NODE_PROTOCOLS,
                       versionsStr,
                       message->allocator);
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
    struct RouterModule* module = Identity_cast((struct RouterModule*) vcontext);

    Dict_putInt(message->asDict,
                CJDHTConstants_PROTOCOL,
                Version_CURRENT_PROTOCOL,
                message->allocator);

    if (message->replyTo != NULL) {
        return handleQuery(message, module);
    }

    return 0;
}

/**
 * A context for the internals of a search.
 */
struct SearchContext
{
    struct RouterModule_Promise pub;

    /** The router module carrying out the search. */
    struct RouterModule* const routerModule;

    /** The number of requests which have been sent out so far for this search. */
    uint32_t totalRequests;

    /** The address which we are searching for. */
    struct Address target;

    /**
     * The SearchStore_Search structure for this search,
     * used to keep track of which nodes are participating.
     */
    struct SearchStore_Search* search;

    struct PingContext* currentReq;

    /**
     * The timeout if this timeout is hit then the search will continue
     * but the node will still be allowed to respond and it will be counted as a pong.
     */
    struct Timeout* continueSearchTimeout;

    Identity
};

struct PingContext
{
    struct RouterModule_Promise pub;

    /** nonNull if this ping is part of a search. */
    struct SearchContext* search;

    struct RouterModule* router;

    struct Address address;

    /** The internal ping structure */
    struct Pinger_Ping* pp;

    Identity
};

static void sendMsg(String* txid, void* vpingContext)
{
    struct PingContext* pc = Identity_cast((struct PingContext*) vpingContext);
    sendRequest(&pc->address,
                (pc->search) ? CJDHTConstants_QUERY_FN : CJDHTConstants_QUERY_PING,
                txid,
                (pc->search) ? &pc->search->target : NULL,
                (pc->search) ? CJDHTConstants_TARGET : NULL,
                pc->router->registry);
}

static void onTimeout(uint32_t milliseconds, struct PingContext* pctx)
{
    struct Node* n = NodeStore_getNodeByNetworkAddr(pctx->address.path, pctx->router->nodeStore);

    // Ping timeout -> decrease reach
    if (n) {

        uint32_t newReach = reachAfterTimeout(n->reach);

        #ifdef Log_DEBUG
            uint8_t addr[60];
            Address_print(addr, &n->address);
            Log_debug(pctx->router->logger,
                       "Ping timeout for %s, after %lums. changing reach from %u to %u\n",
                       addr,
                       (unsigned long)milliseconds,
                       n->reach,
                       newReach);
        #endif

        n->reach = newReach;

        if (newReach == 0 && LabelSplicer_isOneHop(n->address.path)) {
            // If the node is directly connected, don't allow the reach to be zeroed
            // because because the node is being periodically pinged at the switch level
            // if the link is broken, the node will be zeroed anyway.
            n->reach++;
        }

        NodeStore_updateReach(n, pctx->router->nodeStore);
    }

    if (pctx->pub.callback) {
        pctx->pub.callback(&pctx->pub, milliseconds, NULL, NULL);
    }
}

static uint64_t pingTimeoutMilliseconds(struct RouterModule* module)
{
    uint64_t out = AverageRoller_getAverage(module->gmrtRoller) * PING_TIMEOUT_GMRT_MULTIPLIER;
    return (out < PING_TIMEOUT_MINIMUM) ? PING_TIMEOUT_MINIMUM : out;
}

static void onResponseOrTimeout(String* data, uint32_t milliseconds, void* vping);

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 */
static void searchStep(struct SearchContext* search)
{
    struct RouterModule* module = search->routerModule;
    struct SearchStore_Node* nextSearchNode = SearchStore_getNextNode(search->search);

    // If the number of requests sent has exceeded the max search requests, let's stop there.
    if (search->totalRequests >= MAX_REQUESTS_PER_SEARCH || nextSearchNode == NULL) {
        if (search->pub.callback) {
            search->pub.callback(&search->pub, 0, NULL, NULL);
        }
        Allocator_free(search->pub.alloc);
        return;
    }

    struct Pinger_Ping* pp = Pinger_newPing(NULL,
                                            onResponseOrTimeout,
                                            sendMsg,
                                            pingTimeoutMilliseconds(module),
                                            module->pinger);

    struct PingContext* pctx = Allocator_clone(pp->pingAlloc, (&(struct PingContext) {
        .pub = {
            .alloc = pp->pingAlloc
        },
        .router = module,
        .search = search,
        .pp = pp
    }));
    Identity_set(pctx);
    Bits_memcpyConst(&pctx->address, &nextSearchNode->address, sizeof(struct Address));

    pp->context = pctx;

    // Get the node from the nodestore because there might be a much better path to the same node.
    struct Node* n = NodeStore_getBest(&pctx->address, module->nodeStore);
    if (n && !Bits_memcmp(n->address.ip6.bytes, pctx->address.ip6.bytes, 16)) {
        uint64_t nlabel = n->address.path;
        uint64_t nsn = pctx->address.path;
        if (nlabel < nsn) {
            pctx->address.path = n->address.path;
        }
    }

    search->totalRequests++;
    search->currentReq = pctx;

    // comes out at sendMsg()
    Pinger_sendPing(pp);
}

/**
 * The only type of message we handle on the incoming side is
 * a response to one of our queries.
 */
static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    String* txid = Dict_getString(message->asDict, CJDHTConstants_TXID);
    String* query = Dict_getString(message->asDict, CJDHTConstants_QUERY);
    if (query || !txid) {
        return 0;
    }

    struct RouterModule* module = Identity_cast((struct RouterModule*) vcontext);

    // This is retreived below by onResponseOrTimeout()
    module->currentMessage = message;
    Pinger_pongReceived(txid, module->pinger);
    module->currentMessage = NULL;

    return 0;
}

// ping or search response came in
static void onResponseOrTimeout(String* data, uint32_t milliseconds, void* vping)
{
    struct PingContext* pctx = Identity_cast((struct PingContext*) vping);

    if (data == NULL) {
        // This is how Pinger signals a timeout.
        onTimeout(milliseconds, pctx);
        return;
    }

    struct RouterModule* module = pctx->router;
    // Grab out the message which was put here in handleIncoming()
    struct DHTMessage* message = module->currentMessage;
    module->currentMessage = NULL;

    // This should never happen
    if (!Address_isSameIp(&pctx->address, message->address)) {
        #ifdef Log_WARN
            uint8_t expectedAddr[60];
            Address_print(expectedAddr, &pctx->address);
            uint8_t receivedAddr[60];
            Address_print(receivedAddr, message->address);
            Log_warn(module->logger,
                     "Got return packet from different address than search was sent!\n"
                     "Expected:%s\n"
                     "     Got:%s\n",
                     expectedAddr,
                     receivedAddr);
        #endif
        return;
    }

    // update the GMRT
    AverageRoller_update(pctx->router->gmrtRoller, milliseconds);
    Log_debug(pctx->router->logger,
               "Received response in %u milliseconds, gmrt now %u\n",
               milliseconds,
               AverageRoller_getAverage(pctx->router->gmrtRoller));

    // If we get a reply from a node which is not in our table
    // it probably means that we just flushed them from the table because
    // a node further up the tree has become unresponsive.
    // ignore their message because it would add orphaned entries to the node tree.
    struct Node* node = NodeStore_getNodeByNetworkAddr(message->address->path, module->nodeStore);
    if (!node || Bits_memcmp(node->address.key, message->address->key, 32)) {
        return;
    }

    int64_t* versionPtr = Dict_getInt(message->asDict, CJDHTConstants_PROTOCOL);
    uint32_t version = ((versionPtr) ? *versionPtr : 0);

    // this implementation only pings to get the address of a node, so lets add the node.
    node = NodeStore_addNode(module->nodeStore, message->address, 2, version);

    Assert_true(node);
    responseFromNode(node, milliseconds, module);

    //////////////////////////  Search Reply //////////////////////////


    String* nodes = Dict_getString(message->asDict, CJDHTConstants_NODES);

    if (nodes && (nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0)) {
        Log_debug(module->logger, "Dropping unrecognized reply");
        return;
    }

    struct VersionList* versions = NULL;
    String* versionsStr = Dict_getString(message->asDict, CJDHTConstants_NODE_PROTOCOLS);
    if (versionsStr) {
        versions = VersionList_parse(versionsStr, message->allocator);
        #ifdef Version_1_COMPAT
            // Version 1 lies about the versions of other nodes, assume they're all v1.
            if (version < 2) {
                for (int i = 0; i < (int)versions->length; i++) {
                    versions->versions[i] = 1;
                }
            }
        #endif
    }

    for (uint32_t i = 0; nodes && i < nodes->len; i += Address_SERIALIZED_SIZE) {
        if (isDuplicateEntry(nodes, i)) {
            continue;
        }
        struct Address addr;
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);

        if (Address_isSameIp(&module->address, &addr)) {
            // Any path which loops back through us is necessarily a dead route.
            NodeStore_brokenPath(addr.path, module->nodeStore);
            continue;
        }

        /* oww my ears
        #ifdef Log_DEBUG
            uint8_t fromAddr[60];
            uint8_t newAddr[60];
            Address_print(fromAddr, message->address);
            Address_print(newAddr, &addr);
            Log_debug(module->logger,
                       "Discovered new node:\n    %s\nvia:%s\n",
                       newAddr,
                       fromAddr);
        #endif
        */

        // We need to splice the given address on to the end of the
        // address of the node which gave it to us.
        addr.path = LabelSplicer_splice(addr.path, message->address->path);

        /*#ifdef Log_DEBUG
            uint8_t splicedAddr[60];
            Address_print(splicedAddr, &addr);
            Log_debug(module->logger, "Spliced Address is now:\n    %s\n", splicedAddr);
        #endif*/

        if (addr.path == UINT64_MAX) {
            Log_debug(module->logger, "Dropping node because route could not be spliced.\n");
            continue;
        }

        uint32_t newNodePrefix = Address_getPrefix(&addr);
        if (!AddressCalc_validAddress(addr.ip6.bytes)) {
            Log_debug(module->logger, "Was told garbage.\n");
            // This should never happen, badnode.
            break;
        }

        // Nodes we are told about are inserted with 0 reach and assumed version 1.
        uint32_t version = (versions) ? versions->versions[i / Address_SERIALIZED_SIZE] : 1;
        NodeStore_addNode(module->nodeStore, &addr, 0, version);

        // If this is an active search then add the node to the search.
        // If it's not then we still added the node to the routing table.
        if (pctx->search) {
            // If this node has sent us any entries which are further from the target than it is,
            // garbage the whole response.
            const uint32_t targetPrefix = Address_getPrefix(&pctx->search->target);
            const uint32_t parentDistance = Address_getPrefix(&pctx->address) ^ targetPrefix;

            if ((newNodePrefix ^ targetPrefix) >= parentDistance
                && xorCompare(&pctx->search->target, &addr, &pctx->address) >= 0)
            {
                // Too much noise.
                //Log_debug(module->logger, "Answer was further from the target than us.\n");
                continue;
            }

            struct Node* n = NodeStore_getBest(&addr, module->nodeStore);
            SearchStore_addNodeToSearch((n) ? &n->address : &addr, pctx->search->search);
        }
    }

    // if it's a search...
    if (pctx->search) {
        if (pctx->search->pub.callback) {
            pctx->search->pub.callback(&pctx->search->pub,
                                       milliseconds,
                                       message->address,
                                       message->asDict);
        }
        searchStep(pctx->search);
    }

    #ifdef Log_DEBUG
        String* versionBin = Dict_getString(message->asDict, CJDHTConstants_VERSION);
        if (versionBin && versionBin->len == 20) {
            uint8_t printedAddr[60];
            Address_print(printedAddr, message->address);

            uint8_t versionStr[41];
            Hex_encode(versionStr, 41, (uint8_t*) versionBin->bytes, 20);

            Log_debug(module->logger, "Got pong! [%s] ver[%s]\n", printedAddr, versionStr);
        }
    #endif

    if (pctx->pub.callback) {
        pctx->pub.callback(&pctx->pub, milliseconds, message->address, message->asDict);
    }
}

// Triggered by a search timeout (the message may still come back and will be treated as a ping)
static void searchNextNode(void* vsearch)
{
    struct SearchContext* search = Identity_cast((struct SearchContext*) vsearch);

    // Clear the currentReq so if the message comes back it's treated as a ping.
    search->currentReq->search = NULL;

    // Timeout for trying the next node.
    Timeout_resetTimeout(search->continueSearchTimeout, tryNextNodeAfter(search->routerModule));

    searchStep(search);
}

struct RouterModule_Promise* RouterModule_pingNode(struct Node* node,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    if (timeoutMilliseconds == 0) {
        timeoutMilliseconds =
            AverageRoller_getAverage(module->gmrtRoller) * PING_TIMEOUT_GMRT_MULTIPLIER;
        if (timeoutMilliseconds < PING_TIMEOUT_MINIMUM) {
            timeoutMilliseconds = PING_TIMEOUT_MINIMUM;
        }
    }

    struct Pinger_Ping* pp =
        Pinger_newPing(NULL, onResponseOrTimeout, sendMsg, timeoutMilliseconds, module->pinger);

    struct PingContext* pctx = Allocator_clone(pp->pingAlloc, (&(struct PingContext) {
        .pub = {
            .alloc = pp->pingAlloc
        },
        .router = module,
        .pp = pp
    }));
    Identity_set(pctx);
    Bits_memcpyConst(&pctx->address, &node->address, sizeof(struct Address));

    pp->context = pctx;

    // comes out at sendMsg()
    Pinger_sendPing(pp);

    return &pctx->pub;
}

/** See: RouterModule.h */
struct RouterModule_Promise* RouterModule_search(uint8_t searchTarget[16],
                                                 struct RouterModule* module,
                                                 struct Allocator* alloc)
{
    struct Allocator* searchAllocator = Allocator_child(alloc);
    struct SearchStore_Search* sss =
        SearchStore_newSearch(searchTarget, module->searchStore, searchAllocator);

    struct Address targetAddr;
    Bits_memcpyConst(targetAddr.ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(module->nodeStore,
                                  &targetAddr,
                                  NULL,
                                  RouterModule_K,
                                  true,
                                  Version_CURRENT_PROTOCOL,
                                  searchAllocator);

    if (nodes->size == 0) {
        Log_debug(module->logger, "Can't find any nodes to begin search.");
        Allocator_free(searchAllocator);
        return NULL;
    }

    for (int i = 0; i < (int)nodes->size; i++) {
        SearchStore_addNodeToSearch(&nodes->nodes[i]->address, sss);
    }

    struct SearchContext* search = Allocator_clone(searchAllocator, (&(struct SearchContext) {
        .pub = {
            .alloc = searchAllocator
        },
        .routerModule = module,
        .search = sss
    }));
    Identity_set(search);
    Bits_memcpyConst(&search->target, &targetAddr, sizeof(struct Address));

    // this timeout triggers the next search step if the response has not come in.
    search->continueSearchTimeout = Timeout_setTimeout(searchNextNode,
                                                       search,
                                                       tryNextNodeAfter(module),
                                                       module->eventBase,
                                                       searchAllocator);

    // kick it off
    searchStep(search);

    return &search->pub;
}

/** See: RouterModule.h */
void RouterModule_addNode(struct RouterModule* module, struct Address* address, uint32_t version)
{
    Address_getPrefix(address);
    NodeStore_addNode(module->nodeStore, address, 1, version);
    struct Node* best = RouterModule_lookup(address->ip6.bytes, module);
    if (best && best->address.path != address->path) {
        RouterModule_pingNode(best, 0, module, module->allocator);
    }
}

struct Node* RouterModule_lookup(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                 struct RouterModule* module)
{
    struct Address addr;
    Bits_memcpyConst(addr.ip6.bytes, targetAddr, Address_SEARCH_TARGET_SIZE);

    return NodeStore_getBest(&addr, module->nodeStore);
}

/** see RouterModule.h */
struct Node* RouterModule_getNode(uint64_t path, struct RouterModule* module)
{
    return NodeStore_getNodeByNetworkAddr(path, module->nodeStore);
}

int RouterModule_brokenPath(const uint64_t path, struct RouterModule* module)
{
    return NodeStore_brokenPath(path, module->nodeStore);
}
