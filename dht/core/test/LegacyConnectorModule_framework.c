#define INCLUDE_CONTEXT
#include "dht/core/LegacyConnectorModuleInternal.h"
#include "dht/core/juliusz/dht.h"
#include "libbenc/bencode.h"

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "net/NetworkTools.h"

#include <string.h>

struct LegacyConnectorModuleTest_context {
    char message[10000];
    char buffer[512];
};

static int handleOutgoing(struct DHTMessage* message, void* vcontext)
{
    struct LegacyConnectorModuleTest_context* testContext =
        (struct LegacyConnectorModuleTest_context*) vcontext;

    memset(testContext->message, 0, 10000);

    struct MemAllocator* alloc = BufferAllocator_new(testContext->buffer, 512);
    struct Writer* writer = ArrayWriter_new(testContext->message, 10000, alloc);
    bobj_serialize(writer, message->bencoded);

    return 0;
}

/* The functions which we are testing. */
int send_ping(struct sockaddr *sa, int salen,
              const unsigned char *tid, int tid_len);

int send_pong(struct sockaddr *sa, int salen,
              const unsigned char *tid, int tid_len);

int send_find_node(struct sockaddr *sa, int salen,
                   const unsigned char *tid, int tid_len,
                   const unsigned char *target,
                   int want,
                   int confirm);

int send_nodes_peers(struct sockaddr *sa, int salen,
                     const unsigned char *tid, int tid_len,
                     const unsigned char *nodes, int nodes_len,
                     const unsigned char *nodes6, int nodes6_len,
                     int af,
                     struct storage *st,
                     const unsigned char *token, int token_len);

int send_get_peers(struct sockaddr *sa, int salen,
                   unsigned char *tid, int tid_len,
                   unsigned char *infohash,
                   int want,
                   int confirm);

int send_announce_peer(struct sockaddr *sa, int salen,
                       unsigned char *tid, int tid_len,
                       unsigned char *infohas,
                       unsigned short port,
                       unsigned char *token,
                       int token_len,
                       int confirm);

int send_peer_announced(struct sockaddr *sa, int salen,
                        unsigned char *tid, int tid_len);

int send_error(struct sockaddr *sa, int salen,
               unsigned char *tid, int tid_len,
               int code,
               const char *message);
