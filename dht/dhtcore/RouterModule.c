#include "dht/Address.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModuleInternal.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/SearchStore.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "switch/LabelSplicer.h"
#include "util/AverageRoller.h"
#include "util/Endian.h"
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
#define LOCAL_MAINTENANCE_SEARCH_MILLISECONDS 7500

/**
 * The number of milliseconds to pass between global maintainence searches.
 * These are searches for random targets which are used to discover new nodes.
 */
#define GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS 30000

#define SEARCH_REPEAT_MILLISECONDS 7500

/**
 * The amount to decrease the reach of *every* node per second.
 * This guarantees that no matter how high the reach is, it will never
 * allow a node to be used if it has not replied to a search within
 * an hour. Most nodes will be forced to reply much sooner.
 */
#define REACH_DECREASE_PER_SECOND 20 //(UINT32_MAX / (60 * 30))

/** The maximum number of requests to make before calling a search failed. */
#define MAX_REQUESTS_PER_SEARCH 100

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
                                           struct MemAllocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct event_base* eventBase)
{
    struct RouterModule* const out = allocator->malloc(sizeof(struct RouterModule), allocator);

    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "RouterModule",
        .context = out,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }), registry);

    Address_forKey(&out->address, myAddress);

    out->gmrtRoller = AverageRoller_new(GMRT_SECONDS, allocator);
    AverageRoller_update(out->gmrtRoller, GMRT_INITAL_MILLISECONDS);
    out->searchStore = SearchStore_new(allocator, out->gmrtRoller);
    out->nodeStore = NodeStore_new(&out->address, NODE_STORE_SIZE, allocator);
    out->registry = registry;
    out->eventBase = eventBase;
    out->janitor = Janitor_new(LOCAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               REACH_DECREASE_PER_SECOND,
                               SEARCH_REPEAT_MILLISECONDS,
                               out,
                               out->nodeStore,
                               allocator,
                               eventBase);

    return out;
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
    return (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 3) / 2;
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
    return Time_currentTimeMilliseconds() - tryNextNodeAfter(module);
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
    const struct MemAllocator* allocator = BufferAllocator_new(buffer, 4096);

    message.allocator = allocator;
    message.asDict = benc_newDictionary(allocator);

    // "t":"1234"
    benc_putString(message.asDict, CJDHTConstants_TXID, transactionId, allocator);

    if (searchTarget != NULL) {
        // Otherwise we're sending a ping.
        benc_putString(message.asDict,
                       targetKey,
                       &(String) { .bytes = (char*) searchTarget->ip6.bytes,
                                   .len = Address_SEARCH_TARGET_SIZE },
                       allocator);
    }

    /* "q":"fn" */
    benc_putString(message.asDict, CJDHTConstants_QUERY, queryType, allocator);

    message.address = address;

    DHTModules_handleOutgoing(&message, registry);
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
};

/**
 * Called when a search has completed.
 * Frees the memory that was used in the search and adjusts the reach of the participating nodes.
 *
 * @param store the SearchStore which carried out the search.
 * @param lastNode the last node to respond (presumably with results.)
 * @param targetAddress the address of the thing we're looking for.
 * @param module this router module.
 * @param searchSuccessful true if the search found the desired content, otherwise false.
 */
#include <stdio.h>
static inline void cleanup(struct SearchStore* store,
                           struct SearchStore_Node* lastNode,
                           struct Address* targetAddress,
                           struct RouterModule* module,
                           bool searchSuccessful)
{
    struct SearchStore_Search* search = SearchStore_getSearchForNode(lastNode, store);

    // Add a fake node to the search for the target,
    // this allows us to track the amount of time it took for the last node to get us
    // the result and adjust it's reach accordingly.
    int32_t ret = SearchStore_addNodeToSearch(lastNode, targetAddress, UINT64_MAX, search);

    // if addNode fails because the search is full, don't bother with the last node.
    struct SearchStore_Node* fakeNode =
        (ret == -1) ? lastNode : SearchStore_getNextNode(search, SearchStore_getAllocator(search));

    struct SearchStore_TraceElement* child = SearchStore_backTrace(fakeNode, store);
    struct SearchStore_TraceElement* parent = child->next;
    uint32_t milliseconds = 0;
    uint32_t childPrefix = Address_getPrefix(child->address);
    const uint32_t targetPrefix = Address_getPrefix(targetAddress);
    while (parent != NULL) {
        uint32_t parentPrefix = Address_getPrefix(parent->address);
        struct Node* parentNode = NodeStore_getNode(module->nodeStore, parent->address);
        // If parentNode is NULL then it must have been replaced in the node store.
        if (parentNode != NULL) {
            // anti-divide-by-0 hack
            milliseconds += parent->delayUntilReply | 1;
            uint32_t oldReach = parentNode->reach;
            uint32_t distanceOverTime =
                calculateDistance(parentPrefix, targetPrefix, childPrefix) / milliseconds;
            // If satisfactory result was not returned then the max value is 65535
            // low 16 bits represent effort, high 16 represent results.
            if (!searchSuccessful) {
            //    distanceOverTime >>= 16;
            }

            // ORing the reach is a possible solution to avoid number rollover.
            // TODO: is this a good idea?
            uint32_t newReach = oldReach | distanceOverTime;

            parentNode->reach = newReach;
            module->totalReach += newReach - oldReach;

uint8_t nodeAddr[40];
Address_printIp(nodeAddr, &parentNode->address);
printf("increasing reach for node (%s) by %d\n", nodeAddr, ((int) (newReach - oldReach)));

            NodeStore_updateReach(parentNode, module->nodeStore);
        }

        child = parent;
        childPrefix = parentPrefix;
        parent = parent->next;
    }

    SearchStore_freeSearch(search);
}

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 *
 * @param scc the SearchCallbackContext for this search.
 */
static void searchStep(struct SearchCallbackContext* scc)
{
    struct RouterModule* module = scc->routerModule;
    struct MemAllocator* searchAllocator = SearchStore_getAllocator(scc->search);

    struct SearchStore_Node* nextSearchNode =
        SearchStore_getNextNode(scc->search, searchAllocator);

    // If the number of requests sent has exceeded the max search requests, let's stop there.
    if (scc->totalRequests >= MAX_REQUESTS_PER_SEARCH || nextSearchNode == NULL) {
        if (scc->resultCallback != NULL) {
            scc->resultCallback(scc->resultCallbackContext, NULL);
        }
        cleanup(module->searchStore,
                (nextSearchNode == NULL) ? scc->lastNodeCalled : nextSearchNode,
                &scc->targetAddress,
                module,
                false);
        return;
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
    Timeout_resetTimeout(scc->timeout, tryNextNodeAfter(module));
}

/**
 * Callback for when a search has timed out.
 *
 * @param vcontext the SearchCallbackContext, cast to void.
 */
static void searchRequestTimeout(void* vcontext)
{
    struct SearchCallbackContext* scc = (struct SearchCallbackContext*) vcontext;
    // Go directly to 0 reach, do not pass go, do not collect 200$
    NodeStore_addNode(scc->routerModule->nodeStore,
                      scc->lastNodeCalled->address,
                      INT64_MIN);
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
    // A reply causes the reach to be bumped by 2
    NodeStore_addNode(module->nodeStore, message->address, 2);

    String* nodes = benc_lookupString(message->asDict, CJDHTConstants_NODES);
    if (nodes == NULL || nodes->len == 0 || nodes->len % Address_SERIALIZED_SIZE != 0) {
        return -1;
    }

    String* tid = benc_lookupString(message->asDict, CJDHTConstants_TXID);
    struct SearchStore_Node* parent =
        SearchStore_getNode(tid, module->searchStore, message->allocator);

    if (parent == NULL) {
        // Couldn't find the node, perhaps we were sent a malformed packet.
        return -1;
    }

    // If the search has already replaced the node's location or it has already finished
    // and another search is taking place in the same slot, drop this reply because it is late.
    if (!Address_isSame(parent->address, message->address)) {
        return -1;
    }

    SearchStore_replyReceived(parent, module->searchStore);
    struct SearchStore_Search* search = SearchStore_getSearchForNode(parent, module->searchStore);
    struct SearchCallbackContext* scc = SearchStore_getContext(search);

    // If this node has sent us any entries which are further from the target than it is,
    // garbage the whole response.
    uint32_t targetPrefix = Address_getPrefix(&scc->targetAddress);
    uint32_t parentDistance = Address_getPrefix(parent->address) ^ targetPrefix;
    uint32_t ourAddressPrefix = Address_getPrefix(&module->address);

    uint64_t evictTime = evictUnrepliedIfOlderThan(module);
    for (uint32_t i = 0; i < nodes->len; i += Address_SERIALIZED_SIZE) {
        struct Address addr;
        Address_parse(&addr, (uint8_t*) &nodes->bytes[i]);

        // We need to splice the given address on to the end of the
        // address of the node which gave it to us.
        uint64_t label = LabelSplicer_splice(*((uint64_t*) addr.networkAddress),
                                             *((uint64_t*) message->address->networkAddress));
        memcpy(addr.networkAddress, &label, 8);

        uint32_t thisNodePrefix = Address_getPrefix(&addr);

        // Nodes we are told about are inserted with 0 reach.
        NodeStore_addNode(module->nodeStore, &addr, 0);

        if ((thisNodePrefix ^ targetPrefix) >= parentDistance
            && xorCompare(&scc->targetAddress, &addr, parent->address) >= 0)
        {
            // Answer was further from the target than us.
        } else if (thisNodePrefix == ourAddressPrefix
            && memcmp(module->address.ip6.bytes, addr.ip6.bytes, Address_SEARCH_TARGET_SIZE) == 0)
        {
            // They just told us about ourselves.
        } else {
            SearchStore_addNodeToSearch(parent, &addr, evictTime, search);
        }
    }

    // Ask the callback if we should continue.
    if (scc->resultCallback == NULL
        || scc->resultCallback(scc->resultCallbackContext, message) == false)
    {
        searchStep(scc);
    } else {
        cleanup(module->searchStore, parent, &scc->targetAddress, module, true);
    }
    return 0;
}

/**
 * The only type of message we handle on the incoming side is
 * a response to one of our queries.
 */
static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    if (benc_lookupString(message->asDict, CJDHTConstants_QUERY) == NULL) {
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
    String* target = benc_lookupString(query->asDict, CJDHTConstants_TARGET);
    if (target == NULL || target->len != Address_SEARCH_TARGET_SIZE) {
        return 0;
    }

    struct Address targetAddr;
    memcpy(targetAddr.ip6.bytes, target->bytes, Address_SEARCH_TARGET_SIZE);

    // send the closest nodes
    struct NodeList* nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                                          &targetAddr,
                                                          RouterModule_K + 5,
                                                          false,
                                                          message->allocator);

    String* nodes = message->allocator->malloc(sizeof(String), message->allocator);
    nodes->len = nodeList->size * Address_SERIALIZED_SIZE;
    nodes->bytes = message->allocator->malloc(nodeList->size * Address_SERIALIZED_SIZE,
                                              message->allocator);

    bool hasNonZeroReach = false;
    uint32_t i;
    for (i = 0; i < nodeList->size; i++) {

        // We have to modify the reply in case this node uses a longer label discriminator
        // in our switch than it's target address, the target address *must* have the same
        // length or longer.
        struct Address addr;
        memcpy(&addr, &nodeList->nodes[i]->address, sizeof(struct Address));
        uint64_t label = LabelSplicer_getLabelFor(*((uint64_t*) addr.networkAddress),
                                                  *((uint64_t*) query->address->networkAddress));
        memcpy(&addr.networkAddress, &label, 8);


        Address_serialize((uint8_t*) &nodes->bytes[i * Address_SERIALIZED_SIZE], &addr);
        hasNonZeroReach |= nodeList->nodes[i]->reach;
    }
    if (i > 0) {
        benc_putString(message->asDict, CJDHTConstants_NODES, nodes, message->allocator);
    }

    if (!hasNonZeroReach) {
        Janitor_informOfRecentLocalSearch((uint8_t*) target, module->janitor);
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
    struct MemAllocator* searchAllocator = SearchStore_getAllocator(search);

    struct Address targetAddr;
    memcpy(targetAddr.ip6.bytes, searchTarget, Address_SEARCH_TARGET_SIZE);

    struct NodeList* nodes =
        NodeStore_getClosestNodes(module->nodeStore,
                                  &targetAddr,
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

    struct Timeout* timeout = Timeout_setTimeout(searchRequestTimeout,
                                                 scc,
                                                 tryNextNodeAfter(module),
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

/** See: RouterModule.h */
void RouterModule_addNode(const uint8_t key[Address_KEY_SIZE],
                          const uint8_t networkAddress[Address_NETWORK_ADDR_SIZE],
                          struct RouterModule* module)
{
    struct Address address;
    memset(&address, 0, sizeof(struct Address));
    memcpy(&address.key, key, Address_KEY_SIZE);
    memcpy(&address.networkAddress, networkAddress, Address_NETWORK_ADDR_SIZE);

Address_getPrefix(&address);
printf("Adding node:  ");
for (int i = 0; i < 16; i++) {
    printf("%02x", address.ip6.bytes[i]);
}
printf("\n");

    NodeStore_addNode(module->nodeStore, &address, 0);
}

/** See: RouterModule.h */
void RouterModule_pingNode(const uint8_t networkAddress[Address_NETWORK_ADDR_SIZE],
                           struct RouterModule* module)
{
    struct Address addr;
    memset(&addr, 0, sizeof(struct Address));
    memcpy(&addr.networkAddress, networkAddress, Address_NETWORK_ADDR_SIZE);
    // using "xx" as the tid is just convienent, it could be anything.
    sendRequest(&addr,
                CJDHTConstants_QUERY_PING,
                &(String){ .len = 2, .bytes = "xx" },
                NULL,
                NULL,
                module->registry);
}

struct Node* RouterModule_getNextBest(uint8_t targetAddr[Address_SEARCH_TARGET_SIZE],
                                      struct RouterModule* module)
{
    #define NUMBER_TO_GET 8

    struct Address addr;
    memcpy(addr.ip6.bytes, targetAddr, Address_SEARCH_TARGET_SIZE);

    uint8_t buffer[1024];
    struct MemAllocator* tmpAlloc = BufferAllocator_new(buffer, 1024);
    // Allow the message to be sent on to nodes which are further from the target than us!
    // This means packets sent to nonexistant nodes will loop around the network until they expire
    // or their priority is decreased to 0.
    struct NodeList* nodes =
        NodeStore_getClosestNodes(module->nodeStore, &addr, NUMBER_TO_GET, false, tmpAlloc);

    if (nodes->size == 0) {
        return NULL;
    }

    for (int i = nodes->size - 1; i >= 0; i--) {
        if (nodes->nodes[i]->session.exists) {
            return nodes->nodes[i];
        }
    }

    return nodes->nodes[nodes->size - 1];

    #undef NUMBER_TO_GET
}

struct Node* RouterModule_getNode(uint8_t networkAddress[Address_NETWORK_ADDR_SIZE],
                                  struct RouterModule* module)
{
    return NodeStore_getNodeByNetworkAddr(networkAddress, module->nodeStore);
}
