#ifndef NODE_LIST_H
#define NODE_LIST_H

#include <stdint.h>

#include "dht/dhtcore/Node.h"

/**
 * List of nodes.
 */
struct NodeList
{
    /** The number of nodes in the list. */
    uint32_t size;

    /** An array of pointers to nodes. */
    struct Node** nodes;
};

#endif
