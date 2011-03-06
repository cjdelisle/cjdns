#include <string.h>
#include <stdio.h>
#include "dht/DHTConstants.h"
#include "dht/MessageTypes.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/bencode.h"

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

struct MessageTypeModule_context {
    struct DHTModule module;
};

/**
 * @param allocator a means to allocate memory.
 */
struct DHTModule* MessageTypeModule_new(struct MemAllocator* allocator)
{
    struct MessageTypeModule_context* context =
        allocator->malloc(sizeof(struct MessageTypeModule_context), allocator);

    struct DHTModule module = {
        .name = "DebugInputModule",
        .context = context,
        .handleIncoming = handleIncoming,
    };
    memcpy(&context->module, &module, sizeof(struct DHTModule));
    return &context->module;
}

/*--------------------Internals--------------------*/

/**
 * Get the transaction id of the message.
 * 
 */
static benc_bstr_t* getTransactionId(struct DHTMessage* message)
{
    bobj_t* tid = bobj_dict_lookup(message->bencoded, &DHTConstants_transactionId);
    if (tid != NULL && tid->type == BENC_BSTR) {
        return tid->as.bstr;
    }
    return NULL;
}

/**
 * @param message the message to check, must have already been parsed by the serialization module.
 * @return a MessageTypes_QUERY,
 *           MessageTypes_REPLY,
 *           MessageTypes_ERROR or 0 if the type cannot be determined.
 */
static unsigned short getMessageClass(struct DHTMessage* message)
{
    bobj_t* msgType = bobj_dict_lookup(message->bencoded, &DHTConstants_messageType);
    if (msgType != NULL && msgType->type == BENC_BSTR) {
        benc_bstr_t* msgTypeString = msgType->as.bstr;
        if (benc_bstr_compare(msgTypeString, &DHTConstants_query) == 0) {
            return MessageTypes_QUERY;
        }
        if (benc_bstr_compare(msgTypeString, &DHTConstants_reply) == 0) {
            return MessageTypes_REPLY;
        }
        if (benc_bstr_compare(msgTypeString, &DHTConstants_error) == 0) {
            return MessageTypes_ERROR;
        }
    }
    return 0;
}

/**
 * Check the query type of the message.
 *
 * @param message the message to check, must have already been parsed by the serialization module.
 * @param messageClass if this is MessageTypes_QUERY then this function will look for a query entry
 *                     if this is MessageTypes_REPLY then this function will attempt to extract the
 *                     type of query from the transaction id.
 * @return a MessageType such as MessageType_PING or MessageType_GOT_PEERS, or 0 if a message type
 *         could not be determined.
 */
static unsigned short getMessageType(struct DHTMessage* message,
                                     unsigned short messageClass)
{
    switch (messageClass) {
        case MessageTypes_QUERY : ;
            bobj_t* queryType = bobj_dict_lookup(message->bencoded, &DHTConstants_query);
            if (queryType == NULL || queryType->type != BENC_BSTR) {
                return 0;
            }
            benc_bstr_t* queryString = queryType->as.bstr;
            if (benc_bstr_compare(queryString, &DHTConstants_ping) == 0) {
                return MessageTypes_PING;
            }
            if (benc_bstr_compare(queryString, &DHTConstants_findNode) == 0) {
                return MessageTypes_FIND_NODE;
            }
            if (benc_bstr_compare(queryString, &DHTConstants_getPeers) == 0) {
                return MessageTypes_GET_PEERS;
            }
            if (benc_bstr_compare(queryString, &DHTConstants_announcePeer) == 0) {
                return MessageTypes_ANNOUNCE_PEER;
            }
            return 0;

        case MessageTypes_REPLY : ;
            /* The jch dht engine puts message type into tid. 
             * TODO: Move that into this module and remove the tid from the core entirely.
             */
            benc_bstr_t* tid = getTransactionId(message);
            if (tid == NULL || tid->len < 2) {
                return 0;
            }
            if (memcmp(tid->bytes, "pn", 2) == 0) {
                return MessageTypes_PONG;
            }
            if (memcmp(tid->bytes, "fn", 2) == 0) {
                return MessageTypes_FOUND_NODE;
            }
            if (memcmp(tid->bytes, "gp", 2) == 0) {
                return MessageTypes_GOT_PEERS;
            }
            if (memcmp(tid->bytes, "ap", 2) == 0) {
                return MessageTypes_PEER_ANNOUNCED;
            }
            return 0;
    };
    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    /* Don't need any context at the moment. */
    vcontext = vcontext;

    unsigned short messageType = 0;
    messageType |= getMessageClass(message);
    messageType |= getMessageType(message, messageType);
    message->messageType = messageType;

    return 0;
}
