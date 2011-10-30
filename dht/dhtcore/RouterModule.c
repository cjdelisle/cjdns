#include <string.h>
#include <stdint.h>
#include <event2/event.h>
#include <stdbool.h>

#include "dht/dhtcore/AddrPrefix.h"
#include "dht/dhtcore/Janitor.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModuleInternal.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/SearchStore.h"
#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "util/AverageRoller.h"
#include "util/Endian.h"
#include "util/Time.h"
#include "util/Timeout.h"

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
 * When a search is carried out, the next K returned nodes are not necessarily the closest known
 * nodes to the id of the record. The nodes returned will be the nodes with the lowest
 * distance:reach ratio. The distance:reach ratio is calculated by dividing the distance between
 * the node and the record by the node's reach number.
 *
 * Since information about a node becomes stale over time, all reach numbers are decreased
 * periodically by 1/8 the average reach. Reach numbers which are already equal to 0 are left there.
 * See: Janitor.c amountToDecreaseReach() for more information.
 *
 * TODO
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
 * requests to the second to last and so forth, working backward.
 *
 * The global mean response time is the average amount of time it takes a node to respond to a
 * search query. It is a rolling average over the past 256 seconds.
 *
 * TODO
 * To maximize the quality of service offered by this node this implementation will repeat
 * searches which it handles every number of seconds given by the configuration parameter:
 * globalMaintainenceSearchPeriod.
 *
 * [1] The implementation runs periodic searches for random hashes but unless the search target
 *     falls within it's own reach footprint (where this node has the lowest distance:reach ratio)
 *     the search is not preformed. This means that the node will send out lots of searches early
 *     on when it is training in the network but as it begins to know other nodes with reach,
 *     the contrived searches taper off. These searches are run every number of milliseconds given
 *     by the configuration parameter localMaintainenceSearchPeriod.
 *
 * [2] If a response "overshoots" the record requested then it is calculated as if it had undershot
 *     by the same amount so as not to provide arbitrage advantage to nodes who return results which
 *     are very far away yet very inaccurate. If it overshoots by more than the distance between the
 *     node and the searched for location (this should never happen), it is considered to be 0.
 */

/*
 * NOTES:
 * TODO every time the search backtracks, it runs over the same group of dead nodes again.
 */

/*--------------------Constants--------------------*/

/** The number of seconds of time overwhich to calculate the global mean response time. */
#define GMRT_SECONDS 256

/**
 * The number to initialize the global mean response time averager with so that it will
 * return sane results
 */
#define GMRT_INITAL_MILLISECONDS 1000

/** The number of nodes which we will keep track of. */
#define NODE_STORE_SIZE 16384

/**
 * The number of milliseconds between attempting local maintenance searches.
 * These searches will only happen if this is the closest node so the number only has
 * any effect on cold start.
 */
#define LOCAL_MAINTENANCE_SEARCH_MILLISECONDS 1000

/**
 * The number of milliseconds to pass between global maintainence searches.
 * These are searches for random targets which are used to discover new nodes.
 */
#define GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS 30000

/**
 * The amount to decrease the reach of *every* node per second.
 * This guarantees that no matter how high the reach is, it will never
 * allow a node to be used if it has not replied to a search within
 * an hour. Most nodes will be forced to reply much sooner.
 */
#define REACH_DECREASE_PER_SECOND (UINT32_MAX / (60 * 30))

/** The maximum number of requests to make before calling a search failed. */
#define MAX_REQUESTS_PER_SEARCH 100

/*--------------------Structures--------------------*/
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
                                           const uint8_t myAddress[20],
                                           struct event_base* eventBase)
{
    struct RouterModule* const out = allocator->malloc(sizeof(struct RouterModule), allocator);

    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "RouterModule",
        .context = out,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }), registry);

    out->myAddress = benc_newBinaryString((const char*) myAddress, 20, allocator);
    out->gmrtRoller = AverageRoller_new(GMRT_SECONDS, allocator);
    AverageRoller_update(out->gmrtRoller, GMRT_INITAL_MILLISECONDS);
    out->searchStore = SearchStore_new(allocator);
    out->nodeStore = NodeStore_new(myAddress, NODE_STORE_SIZE, allocator);
    out->registry = registry;
    out->eventBase = eventBase;
    out->janitor = Janitor_new(LOCAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               GLOBAL_MAINTENANCE_SEARCH_MILLISECONDS,
                               REACH_DECREASE_PER_SECOND,
                               out,
                               out->nodeStore,
                               allocator,
                               eventBase);

    return out;
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
    return Time_currentTimeMilliseconds()
        - (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 2);
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
 * @param networkAddress the address to send the query to.
 * @param queryType what type of query eg: find_node or get_peers.
 * @param transactionId the tid to send with the query.
 * @param searchTarget the thing which we are looking for or null if it's a ping.
 * @param targetKey the key underwhich to send the target eg: target or info_hash
 * @param registry the DHT module registry to use for sending the message.
 */
static inline void sendRequest(const uint8_t networkAddress[6],
                               String* queryType,
                               String* transactionId,
                               String* searchTarget,
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
    benc_putString(message.asDict, &DHTConstants_transactionId, transactionId, allocator);

    // "y":"q"
    benc_putString(message.asDict,
                   &DHTConstants_messageType,
                   &DHTConstants_query,
                   allocator);

    /* "a" : { ...... */
    if (searchTarget != NULL) {
        // Otherwise we're sending a ping.
        Dict* args = benc_newDictionary(allocator);
        benc_putString(args, targetKey, searchTarget, allocator);
        benc_putDictionary(message.asDict, &DHTConstants_arguments, args, allocator);
    }

    /* "q":"find_node" */
    benc_putString(message.asDict, &DHTConstants_query, queryType, allocator);

    memcpy(message.peerAddress, networkAddress, 6);
    message.addressLength = 6;

    DHTModules_handleOutgoing(&message, registry);
}

/**
 * The amount of time to wait before skipping over the first node and trying another in a search.
 *
 * @param module this module.
 * @return the timeout time.
 */
static inline uint64_t tryNextNodeAfter(struct RouterModule* module)
{
    return ((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 2;
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
    String* const target;

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
#include "util/Hex.h"
static inline void cleanup(struct SearchStore* store,
                           struct SearchStore_Node* lastNode,
                           uint8_t targetAddress[20],
                           struct RouterModule* module,
                           bool searchSuccessful)
{
    struct SearchStore_Search* search = SearchStore_getSearchForNode(lastNode, store);

    // Add a fake node to the search for the target,
    // this allows us to track the amount of time it took for the last node to get us
    // the result and adjust it's reach accordingly.
    int32_t ret = SearchStore_addNodeToSearch(lastNode,
                                              targetAddress,
                                              (uint8_t*) "Unused",
                                              UINT64_MAX,
                                              search);

    // if addNode fails because the search is full, don't bother with the last node.
    struct SearchStore_Node* fakeNode =
        (ret == -1) ? lastNode : SearchStore_getNextNode(search, SearchStore_getAllocator(search));

    struct SearchStore_TraceElement* child = SearchStore_backTrace(fakeNode, store);
    struct SearchStore_TraceElement* parent = child->next;
    const uint32_t targetPrefix = AddrPrefix_get(targetAddress);
    uint32_t milliseconds = 0;
    uint32_t childPrefix = AddrPrefix_get(child->address);
    while (parent != NULL) {
        uint32_t parentPrefix = AddrPrefix_get(parent->address);
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
            NodeStore_updateReach(parentNode, module->nodeStore);
        }

        child = parent;
        childPrefix = parentPrefix;
        parent = parent->next;
    }

    // This is a hack around a problem with the allocator not executing onfree's correctly!!!
    // TODO fix this at it's source
    Timeout_clearTimeout(((struct SearchCallbackContext*)SearchStore_getContext(search))->timeout);

    SearchStore_freeSearch(search);
}

/**
 * Send a search request to the next node in this search.
 * This is called whenever a response comes in or after the global mean response time passes.
 *
 * @param vcontext the SearchCallbackContext for this search.
 */
static void searchStep(void* vcontext)
{
    struct SearchCallbackContext* scc = (struct SearchCallbackContext*) vcontext;
    struct RouterModule* module = scc->routerModule;
    struct MemAllocator* searchAllocator = SearchStore_getAllocator(scc->search);

    struct SearchStore_Node* nextSearchNode =
        SearchStore_getNextNode(scc->search, searchAllocator);

    // If the number of requests sent has exceeded the max search requests, let's stop there.
    if (scc->totalRequests >= MAX_REQUESTS_PER_SEARCH || nextSearchNode == NULL) {
printf("terminating search.\n");
        if (scc->resultCallback != NULL) {
            scc->resultCallback(scc->resultCallbackContext, NULL);
        }
        cleanup(module->searchStore,
                (nextSearchNode == NULL) ? scc->lastNodeCalled : nextSearchNode,
                (uint8_t*) scc->target->bytes,
                module,
                false);
        return;
    }

    sendRequest(nextSearchNode->networkAddress,
                scc->requestType,
                SearchStore_tidForNode(nextSearchNode, searchAllocator),
                scc->target,
                scc->targetKey,
                scc->routerModule->registry);

    scc->totalRequests++;
    scc->lastNodeCalled = nextSearchNode;
    SearchStore_requestSent(nextSearchNode, module->searchStore);
    Timeout_resetTimeout(scc->timeout, tryNextNodeAfter(module));
}

/**
 * Return which node is closer to the target.
 *
 * @param target the address to test distance against.
 * @param negativeIfCloser one address to check distance.
 * @param positiveIfCloser another address to check distance.
 * @return -1 if negativeIfCloser is closer to target, 1 if positiveIfCloser is closer
 *         0 if they are both the same.
 */
static inline int32_t xorCompare(const uint8_t target[20],
                                 const uint8_t negativeIfCloser[20],
                                 const uint8_t positiveIfCloser[20])
{
    for (uint32_t i = 0; i < 5; i++) {
        uint32_t nic = ((uint32_t*) negativeIfCloser)[i];
        uint32_t pic = ((uint32_t*) positiveIfCloser)[i];
        if (nic == pic) {
            continue;
        }
        uint32_t ref = ntohl(((uint32_t*) target)[i]);
        if ((ntohl(nic) ^ ref) < (ntohl(pic) ^ ref)) {
            return -1;
        } else {
            return 1;
        }
    }
    return 0;
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
    Dict* arguments = benc_lookupDictionary(message->asDict, &DHTConstants_reply);
    if (arguments == NULL) {
        return 0;
    }

    // this implementation only pings to get the address of a node, so lets add the node.
    String* address = benc_lookupString(arguments, &DHTConstants_myId);
    NodeStore_addNode(module->nodeStore,
                      (uint8_t*) address->bytes,
                      (uint8_t*) message->peerAddress);

    String* nodes = benc_lookupString(arguments, &DHTConstants_nodes);
    if (nodes == NULL || nodes->len % 26 != 0) {
        return -1;
    }

    String* tid = benc_lookupString(message->asDict, &DHTConstants_transactionId);
    struct SearchStore_Node* parent =
        SearchStore_getNode(tid, module->searchStore, message->allocator);

    if (parent == NULL) {
        // Couldn't find the node, perhaps we were sent a malformed packet.
        return -1;
    }

    struct SearchStore_Search* search = SearchStore_getSearchForNode(parent, module->searchStore);
    struct SearchCallbackContext* scc = SearchStore_getContext(search);

    // If this node has sent us any entries which are further from the target than it is,
    // garbage the whole response.
    uint32_t targetPrefix = AddrPrefix_get((uint8_t*) scc->target->bytes);
    uint32_t parentDistance = AddrPrefix_get(parent->address) ^ targetPrefix;

    uint64_t evictTime = evictUnrepliedIfOlderThan(module);
    for (uint32_t i = 0; i < nodes->len; i += 26) {
//printf("adding node %s\n", Hex_encode(&(String) {20, &nodes->bytes[i]}, message->allocator)->bytes);
        NodeStore_addNode(module->nodeStore,
                          (uint8_t*) &nodes->bytes[i],
                          (uint8_t*) &nodes->bytes[i + 20]);

        uint32_t thisNodePrefix = AddrPrefix_get((uint8_t*) &nodes->bytes[i]);
        uint32_t thisNodeDistance = thisNodePrefix ^ targetPrefix;
        if (thisNodeDistance >= parentDistance && xorCompare((uint8_t*) scc->target->bytes,
                                                             (uint8_t*) &nodes->bytes[i],
                                                             parent->address) >= 0)
        {
//printf("dropping reply because it contains nodes further from us.\n");
        } else {
            SearchStore_addNodeToSearch(parent,
                                        (uint8_t*) &nodes->bytes[i],
                                        (uint8_t*) &nodes->bytes[i + 20],
                                        evictTime,
                                        search);
        }
    }

    // Ask the callback if we should continue.
    if (scc->resultCallback == NULL
        || scc->resultCallback(scc->resultCallbackContext, message) == false)
    {
        searchStep(SearchStore_getContext(search));
    } else {
        cleanup(module->searchStore, parent, (uint8_t*) scc->target->bytes, module, true);
    }
    return 0;
}

/**
 * The only type of message we handle on the incoming side is
 * a response to one of our queries.
 */
static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    String* messageType = benc_lookupString(message->asDict, &DHTConstants_messageType);

    if (benc_stringEquals(messageType, &DHTConstants_reply)) {
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
                              Dict* replyArgs,
                              struct RouterModule* module)
{
    struct DHTMessage* query = message->replyTo;

    Dict* queryArgs = benc_lookupDictionary(query->asDict, &DHTConstants_arguments);

    // Add the node to the store.
    String* address = benc_lookupString(queryArgs, &DHTConstants_myId);
    if (address == NULL || address->len != 20) {
        return 0;
    }
    NodeStore_addNode(module->nodeStore, (uint8_t*) address->bytes, (uint8_t*) query->peerAddress);

    // get the target
    String* target = benc_lookupString(queryArgs, &DHTConstants_targetId);
    if (target == NULL) {
        target = benc_lookupString(queryArgs, &DHTConstants_infoHash);
        if (target == NULL || target->len != 20) {
            return 0;
        }
        Janitor_informOfRecentSearch((uint8_t*) target->bytes, module->janitor);
    } else if (target == NULL || target->len != 20) {
        return 0;
    }

    // send the closest nodes
    struct NodeList* nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                                          (uint8_t*) target->bytes,
                                                          RouterModule_K,
                                                          false,
                                                          message->allocator);

    String* nodes = message->allocator->malloc(sizeof(String), message->allocator);
    nodes->len = nodeList->size * 26;
    nodes->bytes = message->allocator->malloc(nodeList->size * 26, message->allocator);

    uint32_t i;
    for (i = 0; i < nodeList->size; i++) {
        memcpy(&nodes->bytes[i * 26], &nodeList->nodes[i]->address, 20);
        memcpy(&nodes->bytes[i * 26 + 20], &nodeList->nodes[i]->networkAddress, 6);
    }
    if (i > 0) {
        benc_putString(replyArgs, &DHTConstants_nodes, nodes, message->allocator);
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

    // Get the key which the arguments section is under (either "r" or "a")
    String* argumentsKey;
    if (message->replyTo != NULL) {
        argumentsKey = &DHTConstants_reply;
    } else {
        argumentsKey = &DHTConstants_arguments;
    }

    Dict* arguments = benc_lookupDictionary(message->asDict, argumentsKey);
    if (arguments == NULL) {
        arguments = benc_newDictionary(message->allocator);
        benc_putDictionary(message->asDict, argumentsKey, arguments, message->allocator);
    }

    benc_putString(arguments, &DHTConstants_myId, module->myAddress, message->allocator);

    if (message->replyTo != NULL) {
        return handleQuery(message, arguments, module);
    }

    return 0;
}

/** See: RouterModule.h */
struct RouterModule_Search*
    RouterModule_beginSearch(String* requestType,
                             String* targetKey,
                             const uint8_t searchTarget[20],
                             bool (* const callback)(void* callbackContext,
                                                     struct DHTMessage* result),
                             void* callbackContext,
                             struct RouterModule* module)
{
    struct SearchStore_Search* search = SearchStore_newSearch(searchTarget, module->searchStore);
    struct MemAllocator* searchAllocator = SearchStore_getAllocator(search);
    struct NodeList* nodes = NodeStore_getClosestNodes(module->nodeStore,
                                                       searchTarget,
                                                       RouterModule_K,
                                                       true,
                                                       searchAllocator);

    if (nodes->size == 0) {
        // no nodes found!
        return NULL;
    }

    for (uint32_t i = 0; i < nodes->size; i++) {
        SearchStore_addNodeToSearch(NULL,
                                    nodes->nodes[i]->address,
                                    nodes->nodes[i]->networkAddress,
                                    evictUnrepliedIfOlderThan(module),
                                    search);
    }

    struct SearchStore_Node* firstSearchNode = SearchStore_getNextNode(search, searchAllocator);

    String* searchTargetString = benc_newBinaryString((char*) searchTarget, 20, searchAllocator);

    // Send out the request.
    sendRequest(firstSearchNode->networkAddress,
                requestType,
                SearchStore_tidForNode(firstSearchNode, searchAllocator),
                searchTargetString,
                targetKey,
                module->registry);

    SearchStore_requestSent(firstSearchNode, module->searchStore);

    struct SearchCallbackContext* scc =
        searchAllocator->malloc(sizeof(struct SearchCallbackContext), searchAllocator);

    struct Timeout* timeout = Timeout_setTimeout(searchStep,
                                                 scc,
                                                 tryNextNodeAfter(module),
                                                 module->eventBase,
                                                 searchAllocator);

    struct SearchCallbackContext sccLocal = {
        .routerModule = module,
        .resultCallback = callback,
        .resultCallbackContext = callbackContext,
        .target = searchTargetString,
        .timeout = timeout,
        .search = search,
        .requestType = benc_newBinaryString(requestType->bytes, requestType->len, searchAllocator),
        .targetKey = benc_newBinaryString(targetKey->bytes, targetKey->len, searchAllocator),
        .lastNodeCalled = firstSearchNode,
    };
    memcpy(scc, &sccLocal, sizeof(struct SearchCallbackContext));

    SearchStore_setContext(scc, search);

    struct RouterModule_Search* out =
        searchAllocator->malloc(sizeof(struct RouterModule_Search), searchAllocator);
    out->search = search;

    return out;
}

void RouterModule_cancelSearch(struct RouterModule_Search* toCancel)
{
    SearchStore_freeSearch(toCancel->search);
}

/** See: RouterModule.h */
void RouterModule_addNode(const uint8_t address[20],
                          const uint8_t networkAddress[6],
                          struct RouterModule* module)
{
    NodeStore_addNode(module->nodeStore, address, networkAddress);
}

void RouterModule_pingNode(const uint8_t networkAddress[6],
                           struct RouterModule* module)
{
    // using &DHTConstants_version as the tid is just convienent.
    // it could be anything.
    sendRequest(networkAddress,
                &DHTConstants_ping,
                &DHTConstants_version,
                NULL,
                NULL,
                module->registry);
}
