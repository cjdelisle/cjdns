#include <stdint.h>
#include <string.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "dht/DHTTools.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

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
void DHTTools_sendRequest(uint8_t networkAddress[6],
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
