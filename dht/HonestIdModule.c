#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/benc.h"

/**
 * The number of seconds before a node must be made to
 * prove it's id is honest again.
 */
#define REVALIDATE_AFTER 1800

#define REMOVE_AFTER 

/**
 * After this number is reached, nodes will be expelled
 * from the buffer as new nodes are validated.
 */
#define MAX_HONEST_NODES 512

/**
 * A struct representing a single nose who has proven to be honest.
 */
struct HonestIdModule_HonestNode {
    /** The node's DHT id. */
    char id[20];

    

    /** The second when the node was last validated. */
    uint16_t lastValidated;
}

/**
 * The context for the module.
 */
struct HonestIdModule_Context {
    /** The module. */
    struct DHTModule module;

    /** Buffer to hold all of the honest nodes. */
    struct HonestIdModule_HonestNode[MAX_HONEST_NODES];

    /** The place to put the next added honest node. */
    int nextFreeSlot;
};

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

struct DHTModule* HonestIdModule_new(struct MemAllocator* allocator)
{
    struct HonestIdModule_context* context =
        allocator->malloc(sizeof(struct HonestIdModule_context), allocator);

    if (context == NULL) {
        return NULL;
    }

    struct DHTModule localModule = {
        .name = "HonestIdModule",
        .context = context,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .compareNodes = compareNodes,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    };
    memcpy(context->module, &localModule, sizeof(struct DHTModule));

    return context->module;
}

/*--------------------Internals--------------------*/

static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    /* Avoid unused warning. */
    vcontext = vcontext;

    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    /* Avoid unused warning. */
    vcontext = vcontext;

    return 0;
}

static int compareNodes(const char nodeId[20],
                        const char otherNodeId[20],
                        void* context)
{
    
}
