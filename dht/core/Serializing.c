#include "dht/core/LegacyConnectorModuleInternal.h"
#include "dht/DHTConstants.h"
#include "net/NetworkTools.h"
#include <string.h>

/**
 * Get a bobj_t for a string.
 *
 * @param string a benc_bstr_t* object to wrap.
 * @return a pointer to a bobj_t object allocated on the stack
 *         which wraps the given string.
 */
#define OBJ_PTR_FOR_STRING(string)             \
    &(bobj_t) {                                \
        .type = BENC_BSTR,                     \
        .as.bstr = string                      \
    }

/**
 * Get a bobj_t for a dictionary.
 *
 * @param dictionary a benc_dict_entry_t* object to wrap.
 * @return a bobj_t* object allocated on the stack
 *         which wraps the given dictionary.
 */
#define OBJ_PTR_FOR_DICT(dictionary)           \
    &(bobj_t) {                                \
        .type = BENC_DICT,                     \
        .as.dict = dictionary                  \
    }

#define STRING_OBJ(length, chars)              \
    &(bobj_t) {                                \
        .type = BENC_BSTR,                     \
        .as.bstr = &(benc_bstr_t) {            \
            .len = length,                     \
            .bytes = chars                     \
        }                                      \
    }

/**
 * Send a generic message.
 *
 * @param address the ip address of the node to ping.
 * @param messageType the type of message,
 *                    DHTDHTConstants_query or DHTDHTConstants_reply
 * @param queryType if messageType is reply then this should be NULL,
 *                  one of [ping, find_node, get_peers, announce_peer]
 * @param transactionId a string to expect back if a query, return back if a
 *                      response.
 * @param argumentsKey the key to put arguments under.
 *                     DHTDHTConstants_arguments or DHTDHTConstants_reply.
 * @param arguments what will go under the "q" or "r" key.
 * @param wantProtocols if set then additional entries will be added for ip4
 *                      and/or ip6.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
static int sendMessage(struct sockaddr* address,
                       benc_bstr_t* messageType,
                       benc_bstr_t* queryType,
                       benc_bstr_t* transactionId,
                       benc_bstr_t* argumentsKey,
                       benc_dict_entry_t* arguments,
                       int wantProtocols)
{
    benc_dict_entry_t* entry = NULL;

    if (arguments && argumentsKey) {
       entry = &(benc_dict_entry_t) {
            .next = entry,
            .key = argumentsKey,
            .val = OBJ_PTR_FOR_DICT(arguments)
        };
    }

    /* "t":"aa" */
    if (transactionId) {
        entry = &(benc_dict_entry_t) {
            .next = entry,
            .key = &DHTConstants_transactionId,
            .val = OBJ_PTR_FOR_STRING(transactionId)
        };
    }

    /* "y":"q" */
    if (messageType) {
        entry = &(benc_dict_entry_t) {
            .next = entry,
            .key = &DHTConstants_messageType,
            .val = OBJ_PTR_FOR_STRING(messageType)
        };
    }

    /* "q":"find_node" */
    if (queryType) {
        entry = &(benc_dict_entry_t) {
            .next = entry,
            .key = &DHTConstants_query,
            .val = OBJ_PTR_FOR_STRING(queryType)
        };
    }

    /* IP6 compatibility addon.
     * Not in original specification.
     * "want": ["n4", "n6"]
     */
    if (wantProtocols > 0) {
        benc_list_entry_t* wishList = NULL;
        if (wantProtocols & WANT4) {
            wishList = &(benc_list_entry_t) {
                .next = wishList,
                .elem = OBJ_PTR_FOR_STRING(&DHTConstants_wantIp4)
            };
        }
        if (wantProtocols & WANT6) {
            wishList = &(benc_list_entry_t) {
                .next = wishList,
                .elem = OBJ_PTR_FOR_STRING(&DHTConstants_wantIp6)
            };
        }
        if (wishList != NULL) {
            entry = &(benc_dict_entry_t) {
                .next = entry,
                .key = &DHTConstants_want,
                .val = &(bobj_t) {
                    .type = BENC_LIST,
                    .as.list = wishList
                }
            };
        }
    }

    struct DHTMessage message;
    memset(&message, 0, sizeof(struct DHTMessage));

    message.addressLength =
        NetworkTools_addressFromSockaddr((struct sockaddr_storage*) address,
                                         message.peerAddress);
    if (message.addressLength < 6) {
        /* Failed to get the sockaddr. */
        return -1;
    }


    /* The last entry in the list is considered the dictionary. */
    message.bencoded = OBJ_PTR_FOR_DICT(entry);

    DHTModules_handleOutgoing(&message, context->registry);

    /* TODO register an error if one occurs. */
    return 0;
}


/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send a ping to another node.
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of the address, unused.
 * @param transactionId a string to expect back from the node.
 * @param transactionIdLength length of transactionId.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_ping(struct sockaddr *address,
              int addressLength,
              const unsigned char *transactionId,
              int transactionIdLength)
{
    /* Unused. */
    addressLength = addressLength;

    /* We're creating this:
     * ping Query = {
     *    "t":"aa",
     *    "y":"q",
     *    "q":"ping",
     *    "a":{
     *        "id":"abcdefghij0123456789"
     *    }
     * }
     *
     * bencoded = d1:ad2:id20:abcdefghij0123456789e1:q4:ping1:t2:aa1:y1:qe
     */

    benc_dict_entry_t arguments = {
        .next = NULL,
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId)
    };

    return sendMessage(address,
                       &DHTConstants_query,
                       &DHTConstants_ping,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_arguments,
                       &arguments,
                       0);
}

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send a pong to another node who pinged us.
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of the address, unused.
 * @param transactionId a string to expect back from the node.
 * @param transactionIdLength length of transactionId.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_pong(struct sockaddr *address,
              int addressLength,
              const unsigned char *transactionId,
              int transactionIdLength)
{
    /* Unused. */
    addressLength = addressLength;

    /* We are creating this:
     * Response = {"t":"aa", "y":"r", "r": {"id":"mnopqrstuvwxyz123456"}}
     * bencoded = d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re
     */

    benc_dict_entry_t arguments = {
        .next = NULL,
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId)
    };

    return sendMessage(address,
                       &DHTConstants_reply,
                       NULL,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_reply,
                       &arguments,
                       0);
}

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send a find_node request
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of the address, unused.
 * @param transactionId a string to expect back from the node.
 * @param transactionIdLength length of transactionId.
 * @param targetId the ID of the node we are looking for.
 * @param wantProtocols do we want ip4 or ip6 or both? WANT4 = 1, WANT6 = 2
 *                      WANT4 & WANT6 = 3.
 * @param confirm intended to tell the kernel to skip sending an arp packet
 *                before sending the udp packet. Unused as that functionality
 *                is the responsibility of the network module.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_find_node(struct sockaddr *address,
                   int addressLength,
                   const unsigned char *transactionId,
                   int transactionIdLength,
                   const unsigned char *targetId,
                   int wantProtocols,
                   int confirm)
{
    /* Unused. */
    addressLength = addressLength;
    confirm = confirm;

    /* We're creating this:
     * find_node Query = {
     *     "t":"aa",
     *     "y":"q",
     *     "q":"find_node",
     *     "a": {
     *         "id":"abcdefghij0123456789",
     *         "target":"mnopqrstuvwxyz123456"
     *     }
     * }
     *
     * bencoded:
     * d1:ad2:id20:abcdefghij01234567896:target20:mnopqrstuvwxyz123456e
     * 1:q9:find_node1:t2:aa1:y1:qe
     */

    /* "a": {
     *     "id":"abcdefghij0123456789",
     *     "target":"mnopqrstuvwxyz123456"
     * }
     */
    benc_dict_entry_t arguments = {
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId),
        .next = &(benc_dict_entry_t) {
            .next = NULL,
            .key = &DHTConstants_targetId,
            .val = STRING_OBJ(20, (char*) targetId)
        }
    };

    return sendMessage(address,
                       &DHTConstants_query,
                       &DHTConstants_findNode,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_arguments,
                       &arguments,
                       wantProtocols);
}

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send a find_node request
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of the address, unused.
 * @param transactionId a string to expect back from the node.
 * @param transactionIdLength length of transactionId.
 * @param infohash the hash for the torrent which we are looking for.
 *                 this will always be 20 bytes long.
 * @param wantProtocols do we want ip4 or ip6 or both? WANT4 = 1, WANT6 = 2
 *                      WANT4 & WANT6 = 3.
 * @param confirm intended to tell the kernel to skip sending an arp packet
 *                before sending the udp packet. Unused as that functionality
 *                is the responsibility of the network module.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_get_peers(struct sockaddr *address,
                   int addressLength,
                   unsigned char *transactionId,
                   int transactionIdLength,
                   unsigned char *infoHash,
                   int wantProtocols,
                   int confirm)
{
    /* Unused. */
    addressLength = addressLength;
    confirm = confirm;

    /* We're creating this:
     * get_peers Query = {
     *     "t":"aa",
     *     "y":"q",
     *     "q":"get_peers",
     *     "a": {
     *         "id":"abcdefghij0123456789",
     *         "info_hash":"mnopqrstuvwxyz123456"
     *     }
     * }
     *
     * bencoded:
     * d1:ad2:id20:abcdefghij01234567899:info_hash20:mnopqrstuvwxyz123456e
     * 1:q9:get_peers1:t2:aa1:y1:qe
     */

    /* {
     *     "id":"abcdefghij0123456789",
     *     "info_hash":"mnopqrstuvwxyz123456"
     * }
     */
    benc_dict_entry_t arguments = {
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId),
        .next = &(benc_dict_entry_t) {
            .next = NULL,
            .key = &DHTConstants_infoHash,
            .val = STRING_OBJ(20, (char*) infoHash)
        }
    };

    return sendMessage(address,
                       &DHTConstants_query,
                       &DHTConstants_getPeers,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_arguments,
                       &arguments,
                       wantProtocols);
}

struct LegacyConnectorModule_internal_node {
    char addressPort[18];
    benc_list_entry_t entry;
    bobj_t obj;
    benc_bstr_t string;
};

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Respond to either a find node or a get peers request.
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of address, unused.
 * @param transactionId a number to prevent confusion if multiple
 *                      "transactions" are happening with the same peer.
 * @param transactionIdLength length of transactionId.
 * @param nodes a buffer containing 20 byte ids of nodes to send.
 * @param nodesLength the length of nodes.
 * @param nodes6 a buffer containing 20 byte ids of ip6 nodes to send.
 * @param nodes6Length the length of nodes6.
 * @param addressType one of AF_INET or AF_INET6, the protocol which our peer
 *                    used to connect to us.
 * @param store a storage structure for ip addresses of peers.
 * @param token a random number which must be reflected back to us if the node
 *              announces that he has a torrent.
 * @param tokenLength the length of token.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_nodes_peers(struct sockaddr *address,
                     int addressLength,
                     const unsigned char *transactionId,
                     int transactionIdLength,
                     const unsigned char *nodes,
                     int nodesLength,
                     const unsigned char *nodes6,
                     int nodes6Length,
                     int addressType,
                     struct storage *store,
                     const unsigned char *token,
                     int tokenLength)
{
    addressLength = addressLength;

    /* Might be doing any of 3 things :/
     *
     * if tokenLength == 0 then this is a response to a find_node
     * else if storage == NULL then this is a response to a get_peers
     *     request but we don't know any peers so we send a node instead.
     * else this is a response to a get_peers request and we do know
     *     peers.
     */

    benc_dict_entry_t* arguments = NULL;

    if(nodes && nodesLength > 0) {
        arguments = &(benc_dict_entry_t) {
            .next = arguments,
            .key = &DHTConstants_nodes,
            .val = STRING_OBJ(nodesLength, (char*) nodes)
        };
    }

    if(nodes6 && nodes6Length > 0) {
        arguments = &(benc_dict_entry_t) {
            .next = arguments,
            .key = &DHTConstants_nodes6,
            .val = STRING_OBJ(nodes6Length, (char*) nodes6)
        };
    }

    if(token && tokenLength > 0) {
        arguments = &(benc_dict_entry_t) {
            .next = arguments,
            .key = &DHTConstants_authToken,
            .val = STRING_OBJ(tokenLength, (char*) token)
        };
    }

    if(store && store->numpeers > 0) {
        int numPeers = store->numpeers;
        int startingPoint = random() % numPeers;
        int i = startingPoint;
        int valuesReturned = 0;
        int addrLength = (addressType == AF_INET) ? 4 : 16;

        /* Since you can't define stack variables in a loop. Next best... */
        struct LegacyConnectorModule_internal_node nodes[51];
        memset(nodes, 0, sizeof(struct LegacyConnectorModule_internal_node) * 51);
        struct LegacyConnectorModule_internal_node* node = nodes;
        node->entry.next = NULL;

        do {
            if (store->peers[i].len == addrLength) {
                unsigned short bigEndianPort = htons(store->peers[i].port);

                memcpy(node->addressPort, store->peers[i].ip, addrLength);
                memcpy((char*) (node->addressPort + addrLength),
                       (char*) &bigEndianPort,
                       2);

                node->string.bytes = node->addressPort;
                node->string.len = addrLength + 2;
                node->obj.type = BENC_BSTR;
                node->obj.as.bstr = &node->string;
                node->entry.elem = &node->obj;

                valuesReturned++;
                node++;
                node->entry.next = &node[-1].entry;
            }
            i = (i + 1) % numPeers;
        } while (i != startingPoint && valuesReturned < 50);

        if (node->entry.next != NULL) {
            arguments = &(benc_dict_entry_t) {
                .next = arguments,
                .key = &DHTConstants_values,
                .val = &(bobj_t) {
                    .type = BENC_LIST,
                    .as.list = node->entry.next
                }
            };
        }
    }

    arguments = &(benc_dict_entry_t) {
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId),
        .next = arguments
    };

    return sendMessage(address,
                       &DHTConstants_reply,
                       NULL,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_reply,
                       arguments,
                       0);
}

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send an announce_peer query.
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of address, unused.
 * @param transactionId a number to prevent confusion if multiple
 *                      "transactions" are happening with the same peer.
 * @param transactionIdLength length of transactionId.
 * @param infohash the hash for the torrent which we are looking for.
 *                 this will always be 20 bytes long.
 * @param port the port number which this node is announcing from.
 * @param token a cookie which we use to prove that we own this ip address
 *              and we are not spoofing.
 * @param tokenLength the length of token.
 * @param confirm intended to tell the kernel to skip sending an arp packet
 *                before sending the udp packet. Unused as that functionality
 *                is the responsibility of the network module.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_announce_peer(struct sockaddr *address,
                       int addressLength,
                       unsigned char *transactionId,
                       int transactionIdLength,
                       unsigned char *infoHash,
                       unsigned short port,
                       unsigned char *token,
                       int tokenLength,
                       int confirm)
{
    /* Unused. */
    addressLength = addressLength;
    confirm = confirm;

    /* We are creating this:
     * announce_peer Query = {
     *     "t":"aa",
     *     "y":"q",
     *     "q":"announce_peer",
     *     "a": {
     *         "id":"abcdefghij0123456789",
     *         "info_hash":"mnopqrstuvwxyz123456",
     *         "port": 6881,
     *         "token": "aoeusnth"
     *     }
     * }
     *
     * bencoded:
     * d1:ad2:id20:abcdefghij01234567899:info_hash20:mnopqrstuvwxyz123456
     * 4:porti6881e5:token8:aoeusnthe1:q13:announce_peer1:t2:aa1:y1:qe
     */

    benc_dict_entry_t* arguments = NULL;

    if (infoHash) {
        arguments = &(benc_dict_entry_t) {
            .next = arguments,
            .key = &DHTConstants_infoHash,
            .val = STRING_OBJ(20, (char*) infoHash)
        };
    }

    if(token && tokenLength > 0) {
        arguments = &(benc_dict_entry_t) {
            .next = arguments,
            .key = &DHTConstants_authToken,
            .val = STRING_OBJ(tokenLength, (char*) token)
        };
    }

    arguments = &(benc_dict_entry_t) {
        .next = arguments,
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId)
    };

    arguments = &(benc_dict_entry_t) {
        .next = arguments,
        .key = &DHTConstants_port,
        .val = &(bobj_t) {
            .type = BENC_INT,
            .as.int_ = port
        }
    };

    return sendMessage(address,
                       &DHTConstants_query,
                       &DHTConstants_announcePeer,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_arguments,
                       arguments,
                       0);
}

/**
 * INTERNAL Please do not use.
 * This is used to bridge to legacy dht.
 *
 * Send a response to an announce_peer query.
 *
 * @param address the ip address of the node to ping.
 * @param addressLength the length of address, unused.
 * @param transactionId a number to prevent confusion if multiple
 *                      "transactions" are happening with the same peer.
 * @param transactionIdLength length of transactionId.
 * @return 0 if all goes well, -1 if there is an error and errno will be set.
 */
int send_peer_announced(struct sockaddr *address,
                        int addressLength,
                        unsigned char *transactionId,
                        int transactionIdLength)
{
    /* Unused. */
    addressLength = addressLength;

    /* We are creating this:
     * Response = {
     *     "t":"aa",
     *     "y":"r",
     *     "r": {
     *         "id":"mnopqrstuvwxyz123456"
     *     }
     * }
     *
     * bencoded = d1:rd2:id20:mnopqrstuvwxyz123456e1:t2:aa1:y1:re
     */

    benc_dict_entry_t* arguments = &(benc_dict_entry_t) {
        .next = NULL,
        .key = &DHTConstants_myId,
        .val = OBJ_PTR_FOR_STRING(&context->myId)
    };

    return sendMessage(address,
                       &DHTConstants_reply,
                       NULL,
                       &(benc_bstr_t) {transactionIdLength, (char*) transactionId},
                       &DHTConstants_reply,
                       arguments,
                       0);
}

int send_error(struct sockaddr *address,
               int addressLength,
               unsigned char *transactionId,
               int transactionIdLength,
               int code,
               const char *errorMessage)
{
    /* Unused. */
    addressLength = addressLength;

    /* We are creating this:
     * generic error = {
     *    "t":"aa",
     *    "y":"e",
     *    "e":[
     *        201,
     *        "A Generic Error Ocurred"
     *    ]
     * }
     *
     * bencoded = d1:eli201e23:A Generic Error Ocurrede1:t2:aa1:y1:ee
     */

    benc_dict_entry_t* entry = NULL;

    /* "t":"aa" */
    if (transactionId && transactionIdLength > 0) {
        entry = &(benc_dict_entry_t) {
            .next = entry,
            .key = &DHTConstants_transactionId,
            .val = STRING_OBJ(transactionIdLength, (char*) transactionId)
        };
    }

    /* "y":"e" */
    entry = &(benc_dict_entry_t) {
        .next = entry,
        .key = &DHTConstants_messageType,
        .val = OBJ_PTR_FOR_STRING(&DHTConstants_error)
    };

    /* "e": [201, "A Generic Error Ocurred"] */
    entry = &(benc_dict_entry_t) {
        .next = entry,
        .key = &DHTConstants_error,
        .val = &(bobj_t) {
            .type = BENC_LIST,
            .as.list = &(benc_list_entry_t) {
                .elem = &(bobj_t) { .type = BENC_INT, .as.int_ = code },
                .next = &(benc_list_entry_t) {
                    .elem = STRING_OBJ(strlen(errorMessage), (char*) errorMessage),
                    .next = NULL
                }
            }
        }
    };

    struct DHTMessage message;
    memset(&message, 0, sizeof(struct DHTMessage));

    message.addressLength =
        NetworkTools_addressFromSockaddr((struct sockaddr_storage*) address,
                                         message.peerAddress);
    if (message.addressLength < 6) {
        /* Failed to get the sockaddr. */
        return -1;
    }

    /* The last entry in the list is considered the dictionary. */
    message.bencoded = OBJ_PTR_FOR_DICT(entry);

    DHTModules_handleOutgoing(&message, context->registry);

    /* TODO register an error if one occurs. */
    return 0;
}


#undef OBJ_PTR_FOR_STRING
#undef OBJ_PTR_FOR_DICT
#undef STRING_OBJ
