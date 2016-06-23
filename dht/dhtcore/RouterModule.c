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
#include "benc/String.h"
#include "dht/Address.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtcore/RouterModule_pvt.h"
#include "dht/dhtcore/Node.h"
#include "dht/dhtcore/NodeList.h"
#include "dht/dhtcore/NodeStore.h"
#include "dht/dhtcore/VersionList.h"
#include "dht/CJDHTConstants.h"
#include "dht/DHTMessage.h"
#include "dht/DHTModule.h"
#include "dht/DHTModuleRegistry.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "switch/LabelSplicer.h"
#include "switch/NumberCompress.h"
#include "util/events/EventBase.h"
#include "util/AverageRoller.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/Endian.h"
#include "util/Pinger.h"
#include "util/events/Time.h"
#include "util/events/Timeout.h"
#include "util/version/Version.h"
#include "wire/Message.h"

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
 * TODO(cjd): ---
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

/** The number of times the GMRT before pings should be timed out. */
#define PING_TIMEOUT_GMRT_MULTIPLIER 10

/** The minimum amount of time before a ping should timeout. */
#define PING_TIMEOUT_MINIMUM 3000
#define PING_TIMEOUT_MAXIMUM 30000

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
 * @param nodeStore the place to put the nodes
 * @return the RouterModule.
 */
struct RouterModule* RouterModule_register(struct DHTModuleRegistry* registry,
                                           struct Allocator* allocator,
                                           const uint8_t myAddress[Address_KEY_SIZE],
                                           struct EventBase* eventBase,
                                           struct Log* logger,
                                           struct Random* rand,
                                           struct NodeStore* nodeStore)
{
    struct RouterModule* const out = Allocator_calloc(allocator, sizeof(struct RouterModule), 1);

    struct DHTModule* dm = Allocator_clone(allocator, (&(struct DHTModule) {
        .name = "RouterModule",
        .context = out,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    }));
    DHTModuleRegistry_register(dm, registry);

    Address_forKey(&out->address, myAddress);

    out->gmrtRoller = AverageRoller_new(GMRT_SECONDS, eventBase, allocator);
    AverageRoller_update(out->gmrtRoller, GMRT_INITAL_MILLISECONDS);
    out->nodeStore = nodeStore;
    out->registry = registry;
    out->eventBase = eventBase;
    out->logger = logger;
    out->allocator = allocator;
    out->rand = rand;
    out->pinger = Pinger_new(eventBase, rand, logger, allocator);

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
uint64_t RouterModule_searchTimeoutMilliseconds(struct RouterModule* module)
{
    uint64_t x = (((uint64_t) AverageRoller_getAverage(module->gmrtRoller)) * 4);
    x = x + (Random_uint32(module->rand) % (x | 1)) / 2;
    return (x > MAX_TIMEOUT) ? MAX_TIMEOUT : (x < MIN_TIMEOUT) ? MIN_TIMEOUT : x;
}

static inline int sendNodes(struct NodeList* nodeList,
                            struct DHTMessage* message,
                            struct RouterModule* module,
                            uint32_t askerVersion)
{
    struct DHTMessage* query = message->replyTo;
    String* nodes =
        String_newBinary(NULL, nodeList->size * Address_SERIALIZED_SIZE, message->allocator);

    struct VersionList* versions = VersionList_new(nodeList->size, message->allocator);

    int i = 0;
    for (; i < (int)nodeList->size; i++) {

        // We have to modify the reply in case this node uses a longer label discriminator
        // in our switch than its target address, the target address *must* have the same
        // length or longer.
        struct Address addr;
        Bits_memcpy(&addr, &nodeList->nodes[i]->address, sizeof(struct Address));

        addr.path = NumberCompress_getLabelFor(addr.path, query->address->path);

        Address_serialize(&nodes->bytes[i * Address_SERIALIZED_SIZE], &addr);

        versions->versions[i] = nodeList->nodes[i]->address.protocolVersion;

        Assert_ifParanoid(!Bits_isZero(&nodes->bytes[i * Address_SERIALIZED_SIZE],
                                       Address_SERIALIZED_SIZE));
    }
    nodes->len = i * Address_SERIALIZED_SIZE;
    versions->length = i;
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

    struct NodeList* nodeList = NULL;

    String* queryType = Dict_getString(query->asDict, CJDHTConstants_QUERY);
    if (String_equals(queryType, CJDHTConstants_QUERY_FN)) {
        // get the target
        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == NULL || target->len != Address_SEARCH_TARGET_SIZE) {
            return 0;
        }

        struct Address targetAddr = { .path = 0 };
        Bits_memcpy(targetAddr.ip6.bytes, target->bytes, Address_SEARCH_TARGET_SIZE);

        // send the closest nodes
        nodeList = NodeStore_getClosestNodes(module->nodeStore,
                                             &targetAddr,
                                             RouterModule_K,
                                             version,
                                             message->allocator);

    } else if (String_equals(queryType, CJDHTConstants_QUERY_GP)) {
        // get the target
        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == NULL || target->len != 8) {
            return 0;
        }
        uint64_t targetPath;
        Bits_memcpy(&targetPath, target->bytes, 8);
        targetPath = Endian_bigEndianToHost64(targetPath);

        nodeList =
            NodeStore_getPeers(targetPath, RouterModule_K, message->allocator, module->nodeStore);

    } else if (String_equals(queryType, CJDHTConstants_QUERY_NH)) {
        // get the target
        String* target = Dict_getString(query->asDict, CJDHTConstants_TARGET);
        if (target == NULL || target->len != Address_SEARCH_TARGET_SIZE) {
            return 0;
        }
        struct Node_Two* nn = NodeStore_getBest(module->nodeStore, target->bytes);
        nodeList = Allocator_calloc(message->allocator, sizeof(struct NodeList), 1);
        if (nn) {
            nodeList->size = 1;
            nodeList->nodes = Allocator_calloc(message->allocator, sizeof(char*), 1);
            nodeList->nodes[0] = nn;
        }
    }

    return (nodeList) ? sendNodes(nodeList, message, module, version) : 0;
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
    struct RouterModule* module = Identity_check((struct RouterModule*) vcontext);

    Dict_putInt(message->asDict,
                CJDHTConstants_PROTOCOL,
                Version_CURRENT_PROTOCOL,
                message->allocator);

    if (message->replyTo != NULL) {
        return handleQuery(message, module);
    }

    return 0;
}

struct PingContext
{
    struct RouterModule_Promise pub;

    /** nonNull if this ping is part of a search. */
    struct SearchContext* search;

    struct RouterModule* router;

    struct Address address;

    /** The internal ping structure */
    struct Pinger_Ping* pp;

    /** A template of the message to be sent. */
    Dict* messageDict;

    Identity
};

static void sendMsg(String* txid, void* vpingContext)
{
    struct PingContext* pc = Identity_check((struct PingContext*) vpingContext);

    // "t":"1234"
    Dict_putString(pc->messageDict, CJDHTConstants_TXID, txid, pc->pp->pingAlloc);

    struct Allocator* temp = Allocator_child(pc->pp->pingAlloc);
    struct Message* msg = Message_new(0, DHTMessage_MAX_SIZE + 512, temp);
    struct DHTMessage* dmesg = Allocator_calloc(temp, sizeof(struct DHTMessage), 1);
    dmesg->binMessage = msg;
    dmesg->address = &pc->address;
    dmesg->asDict = pc->messageDict;
    dmesg->allocator = temp;

    DHTModuleRegistry_handleOutgoing(dmesg, pc->router->registry);
}

static void onTimeout(uint32_t milliseconds, struct PingContext* pctx)
{
    struct Node_Two* n = NodeStore_closestNode(pctx->router->nodeStore, pctx->address.path);

    // Ping timeout -> decrease reach
    if (n && !Bits_memcmp(pctx->address.key, n->address.key, 32)) {
        NodeStore_pathTimeout(pctx->router->nodeStore, pctx->address.path);
    }

    if (pctx->pub.callback) {
        pctx->pub.callback(&pctx->pub, milliseconds, NULL, NULL);
    }
}

static uint64_t pingTimeoutMilliseconds(struct RouterModule* module)
{
    uint64_t out = AverageRoller_getAverage(module->gmrtRoller) * PING_TIMEOUT_GMRT_MULTIPLIER;
    out = (out < PING_TIMEOUT_MINIMUM) ? PING_TIMEOUT_MINIMUM : out;
    return (out > PING_TIMEOUT_MAXIMUM) ? PING_TIMEOUT_MAXIMUM : out;
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

    struct RouterModule* module = Identity_check((struct RouterModule*) vcontext);

    // This is retreived below by onResponseOrTimeout()
    module->currentMessage = message;
    Pinger_pongReceived(txid, module->pinger);
    module->currentMessage = NULL;

    return 0;
}

// ping or search response came in
static void onResponseOrTimeout(String* data, uint32_t milliseconds, void* vping)
{
    struct PingContext* pctx = Identity_check((struct PingContext*) vping);
    struct RouterModule* module = pctx->router;
    module->pingsInFlight--;
    if (data == NULL) {
        // This is how Pinger signals a timeout.
        onTimeout(milliseconds, pctx);
        return;
    }

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


    // prevent division by zero
    if (milliseconds == 0) { milliseconds++; }

    struct Node_Two* node = NodeStore_closestNode(module->nodeStore, message->address->path);
    if (node && !Bits_memcmp(node->address.key, message->address->key, 32)) {
        NodeStore_pathResponse(module->nodeStore, message->address->path, milliseconds);
    } else {
        NodeStore_discoverNode(module->nodeStore,
                               message->address,
                               message->encodingScheme,
                               message->encIndex,
                               milliseconds);
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

struct RouterModule_Promise* RouterModule_newMessage(struct Address* addr,
                                                     uint32_t timeoutMilliseconds,
                                                     struct RouterModule* module,
                                                     struct Allocator* alloc)
{
    Assert_ifParanoid(addr->path ==
        EncodingScheme_convertLabel(module->nodeStore->selfNode->encodingScheme,
                                    addr->path,
                                    EncodingScheme_convertLabel_convertTo_CANNONICAL));

    Assert_true(addr->protocolVersion);

    module->pingsInFlight++;
    if (timeoutMilliseconds == 0) {
        timeoutMilliseconds = pingTimeoutMilliseconds(module);
    }
    Log_debug(module->logger, "Sending ping with [%u] millisecond timeout, [%u] in flight now",
              timeoutMilliseconds, module->pingsInFlight);

    struct Pinger_Ping* pp = Pinger_newPing(NULL,
                                            onResponseOrTimeout,
                                            sendMsg,
                                            timeoutMilliseconds,
                                            alloc,
                                            module->pinger);

    struct PingContext* pctx = Allocator_clone(pp->pingAlloc, (&(struct PingContext) {
        .pub = {
            .alloc = pp->pingAlloc
        },
        .router = module,
        .pp = pp
    }));
    Identity_set(pctx);
    Bits_memcpy(&pctx->address, addr, sizeof(struct Address));

    pp->context = pctx;

    return &pctx->pub;
}

void RouterModule_sendMessage(struct RouterModule_Promise* promise, Dict* request)
{
    struct PingContext* pctx = Identity_check((struct PingContext*)promise);
    pctx->messageDict = request;
    // actual send is triggered asynchronously
}

struct RouterModule_Promise* RouterModule_pingNode(struct Address* addr,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(addr, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_PING, promise->alloc);
    RouterModule_sendMessage(promise, d);

    #ifdef Log_DEBUG
        uint8_t buff[60];
        Address_print(buff, addr);
        Log_debug(module->logger, "Sending ping [%u] to [%s]",
                  ((struct PingContext*)promise)->pp->handle, buff);
    #endif

    Assert_true(addr->path != 0);

    return promise;
}

struct RouterModule_Promise* RouterModule_nextHop(struct Address* whoToAsk,
                                                  uint8_t target[16],
                                                  uint32_t timeoutMilliseconds,
                                                  struct RouterModule* module,
                                                  struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(whoToAsk, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_NH, promise->alloc);
    String* targetStr = String_newBinary(target, 16, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, targetStr, promise->alloc);
    RouterModule_sendMessage(promise, d);
    return promise;
}

struct RouterModule_Promise* RouterModule_findNode(struct Address* whoToAsk,
                                                   uint8_t target[16],
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(whoToAsk, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_FN, promise->alloc);
    String* targetStr = String_newBinary(target, 16, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, targetStr, promise->alloc);
    RouterModule_sendMessage(promise, d);
    return promise;
}

struct RouterModule_Promise* RouterModule_getPeers(struct Address* addr,
                                                   uint64_t nearbyLabel,
                                                   uint32_t timeoutMilliseconds,
                                                   struct RouterModule* module,
                                                   struct Allocator* alloc)
{
    struct RouterModule_Promise* promise =
        RouterModule_newMessage(addr, timeoutMilliseconds, module, alloc);
    Dict* d = Dict_new(promise->alloc);
    Dict_putString(d, CJDHTConstants_QUERY, CJDHTConstants_QUERY_GP, promise->alloc);

    uint64_t nearbyLabel_be = Endian_hostToBigEndian64(nearbyLabel);
    uint8_t nearbyLabelBytes[8];
    Bits_memcpy(nearbyLabelBytes, &nearbyLabel_be, 8);
    String* target = String_newBinary(nearbyLabelBytes, 8, promise->alloc);
    Dict_putString(d, CJDHTConstants_TARGET, target, promise->alloc);

    RouterModule_sendMessage(promise, d);
    return promise;
}

struct Node_Two* RouterModule_nodeForPath(uint64_t path, struct RouterModule* module)
{
    struct Node_Link* link = NodeStore_linkForPath(module->nodeStore, path);
    if (!link) { return NULL; }
    return link->child;
}

/*void RouterModule_brokenPath(const uint64_t path, struct RouterModule* module)
{
    NodeStore_brokenPath(path, module->nodeStore);
}*/

uint32_t RouterModule_globalMeanResponseTime(struct RouterModule* module)
{
    return (uint32_t) AverageRoller_getAverage(module->gmrtRoller);
}

void RouterModule_peerIsReachable(uint64_t pathToPeer,
                                  uint64_t lagMilliseconds,
                                  struct RouterModule* module)
{
    Assert_ifParanoid(EncodingScheme_isOneHop(module->nodeStore->selfNode->encodingScheme,
                                              pathToPeer));
    struct Node_Two* nn = RouterModule_nodeForPath(pathToPeer, module);
    for (struct Node_Link* peerLink = nn->reversePeers; peerLink; peerLink = peerLink->nextPeer) {
        if (peerLink->parent != module->nodeStore->selfNode) { continue; }
        if (peerLink->cannonicalLabel != pathToPeer) { continue; }
        struct Address address = { .path = 0 };
        Bits_memcpy(&address, &nn->address, sizeof(struct Address));
        address.path = pathToPeer;
        NodeStore_discoverNode(module->nodeStore,
                               &address,
                               nn->encodingScheme,
                               peerLink->inverseLinkEncodingFormNumber,
                               lagMilliseconds);
        return;
    }
    Assert_true(0);
}
