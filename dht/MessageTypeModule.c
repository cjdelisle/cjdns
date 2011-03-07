#include <string.h>
#include <stdio.h>
#include "dht/DHTConstants.h"
#include "dht/MessageTypes.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/bencode.h"

#define WARN(errorString, dhtMessage) \
    fprintf(stderr, "%s", errorString) \

/*--------------------Prototypes--------------------*/
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

struct MessageTypeModule_context {
    struct DHTModule module;

    /** We allow 256 possible types of query. */
    benc_bstr_t queryTypes[256];

    /** A means to get memory for storing bytes of query type strings. */
    struct MemAllocator* allocator;
};

/**
 * @param allocator a means to allocate memory.
 */
struct DHTModule* MessageTypeModule_new(struct MemAllocator* allocator)
{
    struct MessageTypeModule_context* context =
        allocator->malloc(sizeof(struct MessageTypeModule_context), allocator);

    context->allocator = allocator;

    struct DHTModule module = {
        .name = "DebugInputModule",
        .context = context,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    };
    memcpy(&context->module, &module, sizeof(struct DHTModule));
    return &context->module;
}

/*--------------------Internals--------------------*/

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    struct MessageTypeModule_context* context =
        (struct MessageTypeModule_context*) vcontext;


    bobj_t* type = bobj_dict_lookup(message->bencoded, &DHTConstants_messageType);
    if (type != NULL && type->type == BENC_BSTR) {
        message->messageClass = type->as.bstr;
    }

    bobj_t* tid = bobj_dict_lookup(message->bencoded, &DHTConstants_transactionId);
    if (tid != NULL && tid->type == BENC_BSTR) {
        message->transactionId = tid->as.bstr;
    }

    if (message->messageClass == NULL || message->transactionId == NULL) {
        /* Non-conforming message, drop. */
        return -1;
    }

    if (benc_bstr_compare(message->messageClass, &DHTConstants_reply) == 0
        || benc_bstr_compare(message->messageClass, &DHTConstants_error) == 0)
    {
        if (message->transactionId->len < 1) {
            /* We did not send a 0 length tid in a query, this is bogus. */
            return -1;
        }
        unsigned char index = message->transactionId->bytes[message->transactionId->len - 1];
        if (context->queryTypes[index].bytes == NULL) {
            /* I didn't ask for that. */
            return -1;
        }
        message->queryType = &context->queryTypes[index];

        /* deincrement the length since this module added the byte which we just read. */
        message->transactionId->len--;
    } else {
        /* It's a query. */
        bobj_t* query = bobj_dict_lookup(message->bencoded, &DHTConstants_query);
        if (query != NULL && query->type == BENC_BSTR) {
            message->queryType = query->as.bstr;
        }
    }

    if (message->queryType == NULL) {
        /* One way of another this should be set. */
        return -1;
    }

    return 0;
}

static void errorOutOfSpaces(struct MessageTypeModule_context* context)
{
    fprintf(stderr, "MessageTypeModule: Out of spaces in message type array.\n"
                    "Registered message types:\n");
    unsigned char i = 0;
    do {
        fprintf(stderr, "%s\n", context->queryTypes[i].bytes);
        i++;
    } while (i > 0);
}

static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    struct MessageTypeModule_context* context =
        (struct MessageTypeModule_context*) vcontext;

    if (message->messageClass == NULL) {
        /* An outgoing message with no class. */
        WARN("An outgoing message with no class", message);
        return 0;
    }
    if (message->queryType == NULL) {
        /* A message with no queryType. */
        WARN("An outgoing message with no query type", message);
        return 0;
    }
    if (benc_bstr_compare(message->messageClass, &DHTConstants_query) == 0) {
        /* Preset the index to a position with some entropy
         * so that messages will not have to try each location before finding
         * a match. */
        unsigned char beginAt = message->queryType->bytes[0] + message->queryType->len;

        unsigned char index = beginAt + 1;
        while(context->queryTypes[index].bytes != NULL 
              && benc_bstr_compare(message->queryType, context->queryTypes + index) != 0) {
            if (index == beginAt) {
                errorOutOfSpaces(context);
            }
            index++;
        }
        /* Add an entry if it is not already there. */
        if (context->queryTypes[index].bytes == NULL) {
            char* type =
                context->allocator->calloc(message->queryType->len + 1, 1, context->allocator);
            memcpy(type, message->queryType->bytes, message->queryType->len);
            context->queryTypes[index].bytes = type;
            context->queryTypes[index].len = message->queryType->len;
        }

        if (message->transactionId->len >= MAX_TRANSACTION_ID_SIZE) {
            WARN("Killed an outgoing query for having too large a transaction id.", message);
        }
        message->transactionId->bytes[message->transactionId->len] = index;
        message->transactionId->len++;
    }
    return 0;
}
