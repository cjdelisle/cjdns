#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

#include "dht/DHTConstants.h"
#include "dht/MessageTypes.h"
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
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .compareNodes = NULL,
        .handleIncoming = handleIncoming,
        .handleOutgoing = NULL
    };
    struct DHTModule outModule = {
        .name = "DebugModule",
        .context = context,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .compareNodes = NULL,
        .handleIncoming = NULL,
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
static bobj_t* getArguments(struct DHTMessage* message)
{
    /* If query then arguments under 'a', otherwise under 'r' */
    bobj_t* args = bobj_dict_lookup(message->bencoded, &DHTConstants_arguments);
    if (args == NULL) {
        args = bobj_dict_lookup(message->bencoded, &DHTConstants_reply);
    }
    if (args == NULL || args->type != BENC_DICT) {
        return NULL;
    }
    return args;
}

/** @return 0 on success -1 on failure. */
static int getTargetHex(struct DHTMessage* message, char hexOut[41])
{
    bobj_t* args = getArguments(message);
    bobj_t* id = bobj_dict_lookup(args, &DHTConstants_targetId);
    if (id == NULL) {
        id = bobj_dict_lookup(args, &DHTConstants_infoHash);
    }
    if (id != NULL
        && id->type == BENC_BSTR
        && id->as.bstr->len == 20)
    {
        int i;
        unsigned char* idStr = (unsigned char*) id->as.bstr->bytes;
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
    bobj_t* args = getArguments(message);
    bobj_t* id = bobj_dict_lookup(args, &DHTConstants_myId);
    if (id != NULL
        && id->type == BENC_BSTR
        && id->as.bstr->len == 20)
    {
        int i;
        unsigned char* idStr = (unsigned char*) id->as.bstr->bytes;
        for (i = 0; i < 20; i++) {
            sprintf(hexOut + (2 * i), "%02X", idStr[i]);
        }
        return 0;
    }
    return -1;
}

static void printPeer(struct DHTMessage* message)
{
    char buffer[64];
    memset(buffer, ' ', 63);
    buffer[63] = '\0';

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
    sprintf(buffer, "%d.%d.%d.%d:%d             ",
            addr[0], addr[1], addr[2], addr[3], addr[4]);

    buffer[21] = '\0';
    fprintf(stderr, buffer);
}

static void printError(struct DHTMessage* message)
{
    bobj_t* error = bobj_dict_lookup(message->bencoded, &DHTConstants_error);
    if (error != NULL && error->type == BENC_LIST && error->as.list->next != NULL)
    {
        bobj_t* errorNum = error->as.list->elem;
        bobj_t* errorMessage = error->as.list->next->elem;
        if (errorNum != NULL
            || errorNum->type == BENC_INT
            || errorMessage != NULL
            || errorMessage->type == BENC_BSTR)
        {
            fprintf(stderr,
                    "Error #%d - \"%s\"\n",
                    (int) errorNum->as.int_,
                    errorMessage->as.bstr->bytes);
            return;
        }
    }
    unparsable(message);
}

static void printMessage(struct DHTMessage* message, uint64_t counter)
{
    printPeer(message);
    fprintf(stderr, " ");
    if ((message->messageType & MessageTypes_CLASS) == MessageTypes_ERROR) {
        fprintf(stderr, " ");
        printError(message);
        return;
    }

    switch (message->messageType) {
        case MessageTypes_PING : ;
            fprintf(stderr, "Ping      ");
            break;
        case MessageTypes_PONG : ;
            fprintf(stderr, "Pong      ");
            break;
        case MessageTypes_FIND_NODE : ;
            fprintf(stderr, "FindNode  ");
            break;
        case MessageTypes_FOUND_NODE : ;
            fprintf(stderr, "FoundNode ");
            break;
        case MessageTypes_GET_PEERS : ;
            fprintf(stderr, "GetPeers  ");
            break;
        case MessageTypes_GOT_PEERS : ;
            fprintf(stderr, "GotPeers  ");
            break;
        case MessageTypes_ANNOUNCE_PEER : ;
            fprintf(stderr, "Announce  ");
            break;
        case MessageTypes_PEER_ANNOUNCED : ;
            fprintf(stderr, "Announced ");
            break;
        default :
            return unparsable(message);
    };

    char hex[41];
    if (getNodeIdHex(message, hex) != 0) {
        unparsable(message);
        return;
    } else {
        fprintf(stderr, "%s ", hex);
    }

    if (message->messageType & MessageTypes_QUERY && getTargetHex(message, hex) == 0) {
        fprintf(stderr, "Target: %s ", hex);
    }

    bobj_t* version = bobj_dict_lookup(message->bencoded, &DHTConstants_version);
    if (version == NULL) {
        fprintf(stderr, "No version.");
    } else if (version->type != BENC_BSTR) {
        fprintf(stderr, "Version tag not a string!");
    } else if (version->as.bstr->len < 2) {
        fprintf(stderr, "Version length is %d", (int) version->as.bstr->len);
    } else {
        benc_bstr_t* vs = version->as.bstr;
        char v[3];
        memcpy(v, vs->bytes, 2);
        v[2] = '\0';
        fprintf(stderr, "%s:", v);
        if (vs->len < 4) {
            fprintf(stderr, " No version given.");
        } else {
            /* TODO: This is undocumented, it must be either big endian or little endian
             *       right now it is machine dependent since using htons produced numbers which
             *       looked quite wrong.
             */
            unsigned short num = (unsigned short) *(((char*)vs->bytes) + 2);
            fprintf(stderr, ":%d", num);
        }
    }
    fprintf(stderr, (sizeof(long int) == 8) ? " #%ld " : " #%lld ", counter);

    fprintf(stderr, "\n");
}

static void writeMessage(struct DHTMessage* message,
                         struct DebugModule_context* context)
{
    fprintf(context->log, (sizeof(long int) == 8) ? "\n%d %ld " : "\n%d %lld ",
            message->length, context->messageCounter);
    fwrite(message->bytes, 1, message->length, context->log);
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
