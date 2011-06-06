//#include <string.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "libbenc/benc.h"

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
 * find_node request, the distance[2] between it and the first node in it's response is divided by the
 * amount of time it takes to respond over the global mean response time making up a number which
 * represents the node's "reach".
 *
 * Visually representing a node as an area whose location is defined by the node id and it's size is
 * defined by the node reach, you can see that there is a possibility for a record to be closer in
 * key space to node2 while is is still further inside of node1's reach thus node1 is a better choice
 * for the next node to ask.
 *
 * |<--------- Node 1 ---------->|
 *                      |<--- Node 2 ---->|
 *                         ^----- Desired record location.
 *
 * Nodes who time out will have a reach set to 0 so bad/dead nodes are ignored but not removed.
 * New nodes are inserted into the table but with a reach of 0. It is up to the search client to send
 * search requests to them so they can prove their validity and have their reach number updated.
 *
 * When a search is carried out, the next k returned nodes are not necessarily the closest known
 * nodes to the id of the record. The nodes returned will be the nodes with the lowest distance:reach
 * ratio. The distance:reach ratio is calculated by dividing the distance between the node and the record
 * by the node's reach number.
 *
 * Since information about a node becomes stale over time, all reach numbers are decreased periodicly
 * by a configuration parameter reachDecreasePerSecond times the number of seconds in the last period.
 * Reach numbers which are already equal to 0 are left there.
 *
 * In order to have the nodes with least distance:reach ratio ready to handle any incoming search, we
 * precompute the borders where the "best next node" changes. This computation is best understood by
 * graphing the nodes with their location in keyspace on the X axis and their reach on the Y axis.
 * The border between two nodes, nodeA and nodeB is the location where a line drawn from the X axis up
 * to either node location would be the same angle.
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
 * drawing a line from the point given for nodeA to through the point given for nodeB and finding the
 * intersection of that line with the Y axis. border and border2 are shown on different graphs only to
 * limit clutter, they are the same nodeA and nodeB.
 *
 * When resolving a search, this implementation will lookup the location of the searched for record and
 * return the nodes which belong to the insides of the nearest 8 borders, this guarantees return of the
 * nodes whose distance:reach ratio is the lowest for that location.
 *
 * This implementation must never respond to a search by sending any node who's ids are not closer to the
 * target than it's own. Such an event would lead to the possibility of "routing loops" and must be
 * prevented. This node's reach is defined as equal to the reach of the longest reaching node which it
 * knows. Searches forwhich this node has the lowest distance:reach ratio will be replied to with
 * nodes which have 0 reach but are closer than this node or, if there are no such nodes, no nodes at all.
 *
 * The search consumer in this routing module tries to minimize the amount of traffic sent when doing a
 * lookup. To achieve this, it sends a request only to the first node in the search response packet,
 * after the global mean response time has passed without it getting a response, it sends requests to
 * the second third and forth nodes. If after the global mean response time has passed again and it still
 * has not gotten any responses, it will finally send requests to the fifth, sixth, seventh, and eighth
 * nodes.
 *
 * In order to minimize the number of searches which must be replied to with 0 reach nodes because this
 * node is the closest non 0 reach node to the record, this implementation runs periodic searches for
 * random locations where it is the node with the lowest distance:reach ratio. These searches are run
 * periodicly every number of seconds given by the configuration parameter localMaintainenceSearchPeriod.
 *
 * To maximize the quality of service offered by this node and to give other nodes who have 0 reach a
 * chance to prove that they can handle searches, this implementation will repeat searches which it handles
 * every number of seconds given by the configuration parameter globalMaintainenceSearchPeriod.
 *
 * A node which has not responded to a search request in a number of seconds given by the configuration
 * parameter searchTimeoutSeconds will have it's reach set to 0. If a node does this a number of times
 * in a row given by the configuration parameter maxTimeouts, it will be removed from the routing table
 * entirely.
 *
 * [1] Random hashes searched for are random but within the keyspace forwhich this node is the node with
 *     the lowest distance:reach ratio. see paragraph, 3 up from the bottom.
 *
 * [2] If a response "overshoots" the record requested then it is calculated as if it had undershot by
 *     the same amount so as not to provide arbitrage advantage to nodes who return results which are
 *     very far away yet very inaccurate. If it overshoots by more than the distance between the node and
 *     the searched for location (this should never happen), it is considered to be 0.
 */

struct Context
{
    /** A bencoded string with the id tag. */
    String* id;
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

/**
 * When a packet comes in, 
 */
static int discoverNode()
{
}
