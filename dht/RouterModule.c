// Endian swapping crap
#include <netinet/in.h>
// memcmp()
#include <string.h>

#include <sys/time.h>

#ifdef IS_TESTING
    #include <assert.h>
#endif


#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"
#include "util/AverageRoller.h"

/**
 * The router module is the central part of the DHT engine.
 * It's job is to maintain a routing table which is updated by all incoming packets.
 * When it gets an incoming find_node or get_* request, it's job is to add nodes to the reply
 * so that the asking node can find other nodes which are closer to it's target than us.
 *
 * This implementation does not split nodes explicitly into buckets not does it explicitly try to
 * distinguish between "good" and "bad" nodes. Instead it tries to determine which node will help
 * get to the requested record the fastest. Instead of periodicly pinging a random node in each
 * "bucket", this implementation periodically searches for a random[1] hash. When a node is sent a
 * find_node request, the response time ratio is subtracted from the distance[2] between it and the
 * first node in it's response making a number which represents the node's "reach".
 *
 * The response time ratio is a number ranging between 0 and UINT32_MAX which is a function of the
 * amount of time it takes for a node to respond and the global mean response time.
 * See: calculateResponseTimeRatio() for more information about how it is derived.
 *
 * The global mean response time is the average amount of time it takes a node to respond to a
 * find_node request. It is a rolling average over the past 256 seconds.
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
 * Nodes who time out will have a reach set to 0 so bad/dead nodes are ignored but not removed.
 * New nodes are inserted into the table but with a reach of 0. It is up to the search client to
 * send search requests to them so they can prove their validity and have their reach number
 * updated.
 *
 * When a search is carried out, the next k returned nodes are not necessarily the closest known
 * nodes to the id of the record. The nodes returned will be the nodes with the lowest
 * distance:reach ratio. The distance:reach ratio is calculated by dividing the distance between
 * the node and the record by the node's reach number.
 *
 * Since information about a node becomes stale over time, all reach numbers are decreased
 * periodically by a configuration parameter reachDecreasePerSecond times the number of seconds in
 * the last period. Reach numbers which are already equal to 0 are left there.
 *
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
 * and return the nodes which belong to the insides of the nearest 8 borders, this guarantees return
 * of the nodes whose distance:reach ratio is the lowest for that location.
 *
 * This implementation must never respond to a search by sending any node who's id is not closer
 * to the target than it's own. Such an event would lead to the possibility of "routing loops" and
 * must be prevented. This node's "opinion of it's own reach" is defined as equal to the reach of
 * the longest reaching node which it knows. Searches forwhich this node has the lowest
 * distance:reach ratio will be replied to with nodes which have 0 reach but are closer than this
 * node or, if there are no such nodes, no nodes at all.
 *
 * The search consumer in this routing module tries to minimize the amount of traffic sent when
 * doing a lookup. To achieve this, it sends a request only to the first node in the search response
 * packet, after the global mean response time has passed without it getting a response, it sends
 * requests to the second third and forth nodes. If after the global mean response time has passed
 * again and it still has not gotten any responses, it will finally send requests to the fifth,
 * sixth, seventh, and eighth nodes.
 *
 * In order to minimize the number of searches which must be replied to with 0 reach nodes because
 * this node is the closest non 0 reach node to the record, this implementation runs periodic
 * searches for random locations where it is the node with the lowest distance:reach ratio.
 * These searches are run periodicly every number of seconds given by the configuration parameter
 * localMaintainenceSearchPeriod.
 *
 * To maximize the quality of service offered by this node and to give other nodes who have 0 reach
 * a chance to prove that they can handle searches, this implementation will repeat searches which
 * it handles every number of seconds given by the configuration parameter
 * globalMaintainenceSearchPeriod.
 *
 * A node which has not responded to a search request in a number of seconds given by the
 * configuration parameter searchTimeoutSeconds will have it's reach set to 0. If a node does this
 * a number of times in a row given by the configuration parameter maxTimeouts, it will be removed
 * from the routing table entirely.
 *
 * [1] The implementation runs periodic searches for random hashes but unless the search target
 *     falls within it's own reach footprint (where this node has the lowest distance:reach ratio)
 *     the search is not preformed. This means that the node will send out lots of searches early
 *     on when it is training in the network but as it begins to know other nodes with reach,
 *     the contrived searches taper off.
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
 * The number of nodes which will be held in a buffer when performing a search.
 * When new nodes are found which are closer to the search target, they will evict the existing
 * ones from candidates to send further search requests to.
 * It is important that this number is large enough because when a search yields results, the
 * nodes which helped in get to those results have their reach number recalculated and if they are
 * prematurely evicted, they will not have their number recalculated.
 */
#define SEARCH_NODES 32

/** Maximum number of concurrent searches. */
#define MAX_SEARCHES 256


#if (SEARCH_NODES & (SEARCH_NODES - 1))
  #error "SEARCH_NODES must be a power of 2"
#endif
#if (MAX_SEARCHES & (MAX_SEARCHES - 1))
  #error "MAX_SEARCHES must be a power of 2"
#endif


/*--------------------Structures--------------------*/

/** A network address for reaching a peer, in the format which is sent over the wire. */
struct Node
{
    /** The address of the node. */
    uint8_t address[20];

    /** The network address followed by the port, in network order. */
    uint8_t networkAddress[6];

    /**
     * Reach value for the node.
     * @see NodeHeader
     * TODO: use a union so that there is no need for duplication.
     */
    uint32_t reach;
};

/**
 * Information about a given node.
 * Takes 8 bytes, 512 headers per 4096 byte page of memory.
 */
struct NodeHeader
{
    /** The first 4 bytes of the node's address, swapped into host order for easy sorting. */
    uint32_t addressPrefix;

    /**
     * The reach of the node (how big/fast/close it is).
     * Since reach is a fraction, the reach number represents a percentage where 0xFFFFFFFF = 100%
     */
    uint32_t reach;
};

/** A list of DHT nodes. */
struct NodeList
{
    /** A pointer to the first of an array of node headers. */
    struct NodeHeader* headers;

    /**
     * A pointer to the first of the array of nodes
     * Each node corrisponds to the header at the same index in the header array.
     */
    struct Node* nodes;

    /** The maximum number of nodes which can be allocated. */
    uint32_t capacity;

    /** The number of nodes in the list. */
    uint32_t size;
};

/**
 * Information about a node which has responded to a search.
 * This is used to update the node's reach after the search has yielded results.
 */
struct SearchNode
{
    /**
     * The number of milliseconds since the epoch when the search request was sent to this node.
     * since it is stored as a uint32, it rolls over every 48 and a half days.
     * As long as a search doesn't take that long it's ok.
     */
    uint32_t timeOfRequest;

    /** The number of milliseconds between the original request and the reply to it. */
    uint32_t delayUntilReply;

    /** The first 4 bytes of the node address in host order. */
    uint32_t addressPrefix;

    /** The first 4 bytes of the address of the node which introduced us to this node. */
    uint32_t parentAddressPrefix;
};

/** An outstanding search for a target. */
struct Search
{
    /** The nodes to ask when performing the search. */
    struct SearchNode nodes[SEARCH_NODES];

    uint16_t nodeCount;

    uint8_t searchTarget[20];

    /** Number of milliseconds since the epoch when the last request was sent for this search. */
    uint32_t timeOfLastRequest;    

    /** Allocator which allocated this Search. */
    struct MemAllocator* allocator;
};

/** The context for this module. */
struct Context
{
    /** A bencoded string with this node's id tag. */
    String* id;

    /** The list of nodes which are known. */
    struct NodeList list;

    /** An AverageRoller for calculating the global mean response time. */
    void* gmrtRoller;

    /**
     * An array of pointers to all search slots.
     * When a search completes, it will be freed and it's pointer will be set to NULL.
     */
    struct Search* searches[MAX_SEARCHES];
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message, void* vcontext);

/*--------------------Interface--------------------*/

/**
 * Register a new RouterModule.
 *
 * @param registry the DHT module registry for signal handling.
 * @param allocator a means to allocate memory.
 * @param id a random id for this DHT node.
 */
void RouterModule_register(struct DHTModuleRegistry* registry,
                           const struct MemAllocator* allocator,
                           const char id[20])
{
    struct Context* context = allocator->malloc(sizeof(struct Context), allocator);
    context->id = benc_newBinaryString(id, 20, allocator);

    DHTModules_register(allocator->clone(sizeof(struct DHTModule), allocator, &(struct DHTModule) {
        .name = "RouterModule",
        .context = context,
        .handleOutgoing = handleOutgoing
    }), registry);

    context->gmrtRoller = AverageRoller_new(GMRT_SECONDS, allocator);
}

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;

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

    benc_putString(arguments, &DHTConstants_myId, context->id, message->allocator);

    return 0;
}

/** Get the first 4 bytes of the address, swapped into host order. */
#define GET_ADDRESS_PREFIX(address) ntohl(*((uint32_t*) address))

/**
 * Find a node in the list.
 *
 * @param list a list of nodes to get from.
 * @param address the identifier for the node to lookup.
 * @return A pointer to the node if one is found, otherwise NULL.
 */
__attribute__ ((unused))
static struct Node* getNode(struct NodeList list, const uint8_t address[20])
{
    // TODO: maintain a sorted list.

    uint32_t addressPrefix = GET_ADDRESS_PREFIX(address);

    for (uint32_t i = 0; i < list.size; i++) {
        if (addressPrefix == list.headers[i].addressPrefix
            && memcmp(address, list.nodes[i].address, 20) == 0)
        {
            // Synchronize the reach values.
            list.nodes[i].reach = list.headers[i].reach;

            return &list.nodes[i];
        }
    }
    return NULL;
}

/**
 * Insert a node in the list.
 *
 * @param list a list of nodes to insert into.
 * @param toInsert a pointer to the node to be inserted.
 */
__attribute__ ((unused))
static void putNode(struct NodeList list, struct Node* toInsert)
{
    // TODO: maintain a sorted list.

    uint32_t addressPrefix = GET_ADDRESS_PREFIX(toInsert->address);

    #define REPLACE_NODE(index, replaceWith, addressPrefix)            \
        list.headers[index].addressPrefix = addressPrefix;             \
        list.headers[index].reach = 0;                                 \
        memcpy(&list.nodes[index], replaceWith, sizeof(struct Node))

    if (list.size < list.capacity) {
        // Free space, regular insert.
        REPLACE_NODE(list.size, toInsert, addressPrefix);
        list.size++;
        return;
    }

    // The node whose reach difference + distance is the least.
    // This means nodes who are close and have larger reach will be favored.
    uint32_t indexOfNodeToReplace;
    uint32_t leastReachDifferencePlusDistance = 0xFFFFFFFF;
    for (uint32_t i = 0; i < list.size; i++) {

        register uint32_t reachDifferencePlusDistance =
            list.headers[i].reach - toInsert->reach
                + (list.headers[i].addressPrefix ^ addressPrefix);

        if (reachDifferencePlusDistance < leastReachDifferencePlusDistance) {
            leastReachDifferencePlusDistance = reachDifferencePlusDistance;
            indexOfNodeToReplace = i;
        }
    }

    REPLACE_NODE(indexOfNodeToReplace, toInsert, addressPrefix);

    #undef REPLACE_NODE
}

#undef GET_ADDRESS_PREFIX

/**
 * Calculate the response time ratio for a given response time.
 * This function also updates the global mean response time.
 *
 * @param gmrtRoller the AverageRoller for the global mean response time.
 * @param responseTime the response time from the node.
 * @return an integer between 0 and UINT32_MAX which represents the distance be node's response
 *         time and the global mean response time. If the node takes twice the global mean or
 *         longer, the number returned is UINT32_MAX. If the response time is equal to the global
 *         mean then the number returned is half of UINT32_MAX and if the response time is 0
 *         then 0 is returned.
 */
__attribute__ ((unused))
static uint32_t calculateResponseTimeRatio(void* gmrtRoller, const uint32_t responseTime)
{
    const uint32_t gmrt = AverageRoller_update(gmrtRoller, responseTime);
    return (responseTime > 2 * gmrt) ? UINT32_MAX : ((UINT32_MAX / 2) / gmrt) * responseTime;
}

/**
 * Calculate "how far this node got us" in our quest for a given record.
 *
 * When we ask node Alice a search query to find a record,
 * if she replies with a node which is further from the target than her, we are backpeddling,
 * Alice is not compliant and we will return 0 distance because her reach should become zero asap.
 *
 * If Alice responds with a node which is further from her than she is from the target, then she
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
__attribute__ ((unused))
static uint32_t calculateDistance(const uint32_t nodeIdPrefix,
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

    #undef BT
}

/**
 * When a packet comes in, 
 */
__attribute__ ((unused))
static int discoverNode()
{
    return 0;
}

__attribute__ ((unused))
static uint64_t currentTimeMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t) tv.tv_sec) + (tv.tv_usec / 1024);
}

/**
 * Calculate the log base 2 of an integer.
 * Get the position of the most significant bit counting from the right.
 * This implementation is slow but can be calculated at compile time for constants.
 *
 * @param number the number to test
 * @return the log base 2 of that number
 */
static inline uint32_t intLog2_constant(uint32_t number)
{
    #define BIT(K) (((number & (1 << K)) >> K) * K)

    return BIT( 0) + BIT( 1) + BIT( 2) + BIT( 3) + BIT( 4) + BIT( 5) + BIT( 6) + BIT( 7)
         + BIT( 8) + BIT( 9) + BIT(10) + BIT(11) + BIT(12) + BIT(13) + BIT(14) + BIT(15)
         + BIT(16) + BIT(17) + BIT(18) + BIT(19) + BIT(20) + BIT(21) + BIT(22) + BIT(23)
         + BIT(24) + BIT(25) + BIT(26) + BIT(27) + BIT(28) + BIT(29) + BIT(30) + BIT(31);

    #undef BIT
}

#if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
    #if (__BYTE_ORDER == __BIG_ENDIAN)
        #define isBigEndian() 1
    #else
        #define isBigEndian() 0
    #endif
#else
    static uint32_t isBigEndian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1; 
    }
#endif

struct SearchNodeIndex
{
    uint16_t search;

    uint16_t node;
};

__attribute__ ((unused))
static String* tidForSearchNodeIndex(const struct SearchNodeIndex* index,
                                     const struct MemAllocator* allocator)
{
    const uint32_t searchNodesBits = intLog2_constant(SEARCH_NODES);
    const uint32_t maxSearchBits = intLog2_constant(MAX_SEARCHES);
    const uint32_t totalBits = searchNodesBits + maxSearchBits;
    const uint32_t totalBytes = totalBits / 8 + ((totalBits % 8 > 0) ? 1 : 0);

    uint32_t out = (index->node << maxSearchBits) | index->search;

    #ifdef IS_TESTING
        // Make sure the above computations are run at compile time.
        // Only works if using -O3 and obviously not platform independent.
        //assert(__builtin_constant_p(totalBytes));

        // If we are testing then try to confuse the decoder by
        // padding the unused edge with random bits.
        out |= rand() << totalBits;
    #endif

    if (isBigEndian()) {
        out <<= (32 - totalBits);
    }

    return benc_newBinaryString((char*) &out, totalBytes, allocator);
}

__attribute__ ((unused))
static struct SearchNodeIndex searchNodeIndexForTid(const String* tid)
{
    const uint32_t maxNodesBits = intLog2_constant(SEARCH_NODES);
    const uint32_t maxSearchBits = intLog2_constant(MAX_SEARCHES);

    uint32_t number = 0;

    memcpy((char*) &number, tid->bytes, tid->len);

    if (isBigEndian()) {
        number >>= (64 - maxNodesBits + maxSearchBits);
    }

    return (struct SearchNodeIndex) {
        .search = number & (UINT32_MAX >> (32 - maxSearchBits)),

        .node = (number >> maxSearchBits) & (UINT32_MAX >> (32 - maxNodesBits))
    };
}
