#ifndef NODE_HEADER_H
#define NODE_HEADER_H

/**
 * Information about a given node.
 * Takes 8 bytes, 512 headers per 4096 byte page of memory.
 */
struct NodeHeader
{
    /** The first 4 bytes of the node's address, swapped into host order for easy sorting. */
    uint32_t addressPrefix;

    /**
     * Reach value for the node.
     * See: Node.h
     */
    uint32_t reach;
};

#endif
