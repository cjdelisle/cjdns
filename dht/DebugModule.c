#include "DHTModules.h"
#include "memory/MemAllocator.h"

#include <string.h>
#include <stdio.h>
#include <netinet/in.h>

/*--------------------Prototypes--------------------*/
static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext);
static int handleIncoming(struct DHTMessage* message,
                          void* vcontext);

/*--------------------Interface--------------------*/

struct DHTModule* DebugModule_new(struct MemAllocator* allocator)
{
    struct DHTModule* module = allocator->malloc(sizeof(struct DHTModule), allocator);

    struct DHTModule localModule = {
        .name = "DebugModule",
        .context = NULL,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .compareNodes = NULL,
        .handleIncoming = handleIncoming,
        .handleOutgoing = handleOutgoing
    };
    memcpy(module, &localModule, sizeof(struct DHTModule));

    return module;
}

/*--------------------Internals--------------------*/

static void printMessage(struct DHTMessage* message)
{
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
    fprintf(stderr, "\n");
}

void printPeer(struct DHTMessage* message)
{
    int i;
    for (i = 0; i < 4; i++) {
        fprintf(stderr, "%d", (int) message->peerAddress[i]);
        if (i < 3) {
            fprintf(stderr, ".");
        }
    }
    fprintf(stderr, ":");
    unsigned short port = message->peerAddress[5];
    port <<= 8;
    port |= message->peerAddress[4];
    fprintf(stderr, "%d", (int) ntohs(port));
}

static int handleOutgoing(struct DHTMessage* message,
                          void* vcontext)
{
    /* Avoid unused warning. */
    vcontext = vcontext;

    fprintf(stderr, "SENT TO: ");
    printPeer(message);
    fprintf(stderr, "\nMESSAGE: ");
    printMessage(message);
    fprintf(stderr, "\n");

    return 0;
}

static int handleIncoming(struct DHTMessage* message,
                          void* vcontext)
{
    /* Avoid unused warning. */
    vcontext = vcontext;

    fprintf(stderr, "RCVD FROM: ");
    printPeer(message);
    fprintf(stderr, "\nMESSAGE: ");
    printMessage(message);
    fprintf(stderr, "\n");

    return 0;
}
