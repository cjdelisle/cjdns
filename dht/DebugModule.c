#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include "dht/DHTConstants.h"
#include "dht/DHTModules.h"
#include "memory/MemAllocator.h"
#include "libbenc/benc.h"

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

struct DebugModule_context {
    struct DHTModule inModule;
    struct DHTModule outModule;
    uint64_t messageCounter;
    FILE* log;
};

/**
 * @param allocator a means to allocate memory.
 */
struct DHTModule* DebugModule_new(struct MemAllocator* allocator)
{
    struct DebugModule_context* context =
        allocator->malloc(sizeof(struct DebugModule_context), allocator);

    struct DHTModule inModule = {
        .name = "DebugInputModule",
        .context = context,
        .handleIncoming = handleIncoming,
    };
    struct DHTModule outModule = {
        .name = "DebugModule",
        .context = context,
        .handleOutgoing = handleOutgoing
    };
    memcpy(&context->inModule, &inModule, sizeof(struct DHTModule));
    memcpy(&context->outModule, &outModule, sizeof(struct DHTModule));
    return &context->inModule;
}

struct DHTModule* DebugModule_forOutput(struct DHTModule* inputModule)
{
    return &((struct DebugModule_context*)inputModule->context)->outModule;
}

void DebugModule_setLog(FILE* file, struct DHTModule* module)
{
    ((struct DebugModule_context*)module->context)->log = file;
}

/*--------------------Internals--------------------*/

static void unparsable(struct DHTMessage* message)
{
    fprintf(stderr, "Message debug module was unable to parse:\n");
    size_t i;
    unsigned int chr;
    for (i = 0; i < (size_t) message->length; i++) {
        chr = (unsigned int) message->bytes[i] & 0xFF;
        /* Nonprinting chars, and \ and " are hex'd */
        if (chr < 0x7f && chr > 0x20 && chr != 0x5c && chr != 0x22) {
            fprintf(stderr, "%c", message->bytes[i]);
        } else {
            fprintf(stderr, "\\x%.2X", (unsigned int) message->bytes[i] & 0xFF);
        }
    }
    fprintf(stderr, "\n\n");
}

/** @return a dictionary type or NULL. */
static Dict* getArguments(struct DHTMessage* message)
{
    /* If query then arguments under 'a', otherwise under 'r' */
    Dict* args = benc_lookupDictionary(message->asDict, &DHTConstants_arguments);
    if (args == NULL) {
        args = benc_lookupDictionary(message->asDict, &DHTConstants_reply);
    }
    return args;
}

/** @return 0 on success -1 on failure. */
static int getTargetHex(struct DHTMessage* message, char hexOut[41])
{
    Dict* args = getArguments(message);
    String* id = benc_lookupString(args, &DHTConstants_targetId);
    if (id == NULL) {
        id = benc_lookupString(args, &DHTConstants_infoHash);
    }
    if (id != NULL && id->len == 20) {
        int i;
        unsigned char* idStr = (unsigned char*) id->bytes;
        for (i = 0; i < 20; i++) {
            sprintf(hexOut + (2 * i), "%02X", idStr[i]);
        }
        return 0;
    }
    return -1;
}

/** @return 0 on success -1 on failure. */
static int getNodeIdHex(struct DHTMessage* message, char hexOut[41])
{
    Dict* args = getArguments(message);
    String* id = benc_lookupString(args, &DHTConstants_myId);
    if (id != NULL && id->len == 20) {
        int i;
        unsigned char* idStr = (unsigned char*) id->bytes;
        for (i = 0; i < 20; i++) {
            sprintf(hexOut + (2 * i), "%02X", idStr[i]);
        }
        return 0;
    }
    return -1;
}

static void printPeer(struct DHTMessage* message)
{
    if (message->addressLength > 6) {
        fprintf(stderr, "Unexpected address length %d", (int) message->addressLength);
    }
    unsigned short port = message->peerAddress[5];
    port <<= 8;
    port |= message->peerAddress[4];

    int addr[5] = {
        (int) ((unsigned char) message->peerAddress[0]),
        (int) ((unsigned char) message->peerAddress[1]),
        (int) ((unsigned char) message->peerAddress[2]),
        (int) ((unsigned char) message->peerAddress[3]),
        (int) ntohs(port)
    };
    fprintf(stderr, "%d.%d.%d.%d:%d", addr[0], addr[1], addr[2], addr[3], addr[4]);
}

static void printError(struct DHTMessage* message)
{
    List* error = benc_lookupList(message->asDict, &DHTConstants_error);
    if (error != NULL && benc_itemCount(error) == 2) {
        Integer* errorNum = benc_getInteger(error, 0);
        String* errorMessage = benc_getString(error, 1);
        if (errorNum != NULL && errorMessage != NULL) {
            fprintf(stderr, "Error #%d - \"%s\"\n", (int) *errorNum, errorMessage->bytes);
            return;
        }
    }
    unparsable(message);
}

/* This will not scale but it should only be used for debugging. */
static String* getQueryType(struct DHTMessage* message)
{
    if (message->replyTo != NULL) {
        return benc_lookupString(message->replyTo->asDict, &DHTConstants_query);
    }
    String* q = benc_lookupString(message->asDict, &DHTConstants_query);
    if (q != NULL) {
        return q;
    }
    String* tid = benc_lookupString(message->asDict, &DHTConstants_transactionId);
    if (tid != NULL) {
        switch (tid->bytes[0]) {
            case 'p' :;
                return &DHTConstants_ping;
            case 'f' :;
                return &DHTConstants_findNode;
            case 'g' :;
                return &DHTConstants_getPeers;
            case 'a' :;
                return &DHTConstants_announcePeer;
        };
    }
    return NULL;
}

static void printMessage(struct DHTMessage* message, uint64_t counter)
{
    printPeer(message);
    fprintf(stderr, " ");

    String* msgClass = benc_lookupString(message->asDict, &DHTConstants_messageType);
    String* queryType = getQueryType(message);
    if (queryType == NULL) {
        fprintf(stderr, "Unable to determine query type ");
    } else {
        fprintf(stderr, "%s", queryType->bytes);
    }

    if (benc_stringEquals(msgClass, &DHTConstants_reply)) {
        fprintf(stderr, " reply");
    } else if (benc_stringEquals(msgClass, &DHTConstants_error)) {
        fprintf(stderr, " error");
        fprintf(stderr, " ");
        printError(message);
        return;
    }
    fprintf(stderr, " ");

    char hex[41];
    if (getNodeIdHex(message, hex) != 0) {
        unparsable(message);
        return;
    } else {
        fprintf(stderr, "%s ", hex);
    }

    if (getTargetHex(message, hex) == 0) {
        fprintf(stderr, "Target: %s ", hex);
    }

    String* version = benc_lookupString(message->asDict, &DHTConstants_version);
    if (version == NULL || version->len != 4) {
        fprintf(stderr, " NONE");
    } else {
        fprintf(stderr, "%c%c:", version->bytes[0], version->bytes[1]);
        /* TODO: This will fail on big endian boxes. */
        unsigned short num = (unsigned short) *(((char*)version->bytes) + 2);
        fprintf(stderr, "%d", num);
    }
    fprintf(stderr, (sizeof(long int) == 8) ? " #%ld " : " #%lld ", counter);

    fprintf(stderr, "\n");
}

static void writeMessage(struct DHTMessage* message,
                         struct DebugModule_context* context)
{
    if (context->log != NULL) {
        fprintf(context->log, (sizeof(long int) == 8) ? "\n%d %ld " : "\n%d %lld ",
                message->length, context->messageCounter);
        uint32_t discard = fwrite(message->bytes, 1, message->length, context->log);
        discard = discard;
    }
}

static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    struct DebugModule_context* context = (struct DebugModule_context*) vcontext;
    writeMessage(message, context);

    fprintf(stderr, "<-- ");
    printMessage(message, context->messageCounter);
    context->messageCounter++;
    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    struct DebugModule_context* context = (struct DebugModule_context*) vcontext;
    writeMessage(message, context);

    fprintf(stderr, "--> ");
    printMessage(message, context->messageCounter);
    context->messageCounter++;
    return 0;
}
