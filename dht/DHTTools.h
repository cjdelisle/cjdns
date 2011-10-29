#ifndef DHT_TOOLS
#define DHT_TOOLS

#include <stdint.h>

#include "dht/DHTModules.h"
#include "libbenc/benc.h"

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
                          struct DHTModuleRegistry* registry);

#endif
