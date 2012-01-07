#ifndef NODE_H
#define NODE_H

#include "crypto/Session.h"
#include "dht/Address.h"

#include <stdint.h>
#include <stdbool.h>

/** A network address for reaching a peer, in the format which is sent over the wire. */
struct Node
{
    /**
     * The reach of the node (how big/fast/close it is).
     * Since reach is a fraction, the reach number represents a percentage where 0xFFFFFFFF = 100%
     */
    uint32_t reach;

    /** The address of the node. */
    struct Address address;
};

#endif
