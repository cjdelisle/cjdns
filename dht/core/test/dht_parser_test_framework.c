#include "test_framework.c"
#include <string.h>
#include <assert.h>

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Reader.h"
#include "io/ArrayReader.h"
#include "io/Writer.h"
#include "io/ArrayWriter.h"
#include "libbenc/benc.h"

#define ERROR 0
#define REPLY 1
#define PING 2
#define FIND_NODE 3
#define GET_PEERS 4
#define ANNOUNCE_PEER 5

#define WANT4 1
#define WANT6 2

int parse_message(const unsigned char *buf, int buflen,
                   unsigned char *tid_return, int *tid_len,
                   unsigned char *id_return,
                   unsigned char *info_hash_return,
                   unsigned char *target_return,
                   unsigned short *port_return,
                   unsigned char *token_return, int *token_len,
                   unsigned char *nodes_return, int *nodes_len,
                   unsigned char *nodes6_return, int *nodes6_len,
                   unsigned char *values_return, int *values_len,
                   unsigned char *values6_return, int *values6_len,
                   int *want_return);

int parse_message1(const unsigned char *buf, int buflen,
                   unsigned char *tid_return, int *tid_len,
                   unsigned char *id_return,
                   unsigned char *info_hash_return,
                   unsigned char *target_return,
                   unsigned short *port_return,
                   unsigned char *token_return, int *token_len,
                   unsigned char *nodes_return, int *nodes_len,
                   unsigned char *nodes6_return, int *nodes6_len,
                   unsigned char *values_return, int *values_len,
                   unsigned char *values6_return, int *values6_len,
                   int *want_return);

int
parse_message2(bobj_t* bencodedMessage,
               unsigned char *tid_return, int *tid_len,
               unsigned char *id_return,
               unsigned char *info_hash_return,
               unsigned char *target_return,
               unsigned short *port_return,
               unsigned char *token_return, int *token_len,
               unsigned char *nodes_return, int *nodes_len,
               unsigned char *nodes6_return, int *nodes6_len,
               unsigned char *values_return, int *values_len,
               unsigned char *values6_return, int *values6_len,
               int *want_return);

struct dht_parser_input {
    unsigned char *buf;
    int buflen;
    unsigned char *tid_return;
    int tid_len;
    /** This is guarenteed to be 20 bytes long. */
    unsigned char *id_return;
    /** This is guarenteed to be 20 bytes long. */
    unsigned char *info_hash_return;
    /** This is guarenteed to be 20 bytes long. */
    unsigned char *target_return;
    unsigned short port_return;
    unsigned char *token_return;
    int token_len;
    unsigned char *nodes_return;
    int nodes_len;
    unsigned char *nodes6_return;
    int nodes6_len;
    unsigned char *values_return;
    int values_len;
    unsigned char *values6_return;
    int values6_len;
    int want_return;

    int parser_return;
};

int generic_test(const char *input, 
                 int(*parser)(const unsigned char *buf, int buflen,
                              unsigned char *tid_return, int *tid_len,
                              unsigned char *id_return,
                              unsigned char *info_hash_return,
                              unsigned char *target_return,
                              unsigned short *port_return,
                              unsigned char *token_return, int *token_len,
                              unsigned char *nodes_return, int *nodes_len,
                              unsigned char *nodes6_return, int *nodes6_len,
                              unsigned char *values_return, int *values_len,
                              unsigned char *values6_return, int *values6_len,
                              int *want_return),
                 void(*modifier)(struct dht_parser_input *input),
                 int(*validator)(struct dht_parser_input *input))
{
    struct dht_parser_input parserInput;

    parserInput.buf = (unsigned char*) input;

    unsigned char tid[16];
    parserInput.tid_return = tid;
    parserInput.tid_len = 16;

    unsigned char id[20];
    parserInput.id_return = id;
    memcpy(id, "XXXXXXXXXXXXXXXXXXXX", 20);

    unsigned char info_hash[20];
    parserInput.info_hash_return = info_hash;
    memcpy(info_hash, "XXXXXXXXXXXXXXXXXXXX", 20);

    unsigned char target[20];
    parserInput.target_return = target;
    memcpy(target, "XXXXXXXXXXXXXXXXXXXX", 20);

    unsigned char nodes[256];
    parserInput.nodes_return = nodes;
    parserInput.nodes_len = 256;

    unsigned char nodes6[1024];
    parserInput.nodes6_return = nodes6;
    parserInput.nodes6_len = 1024;

    unsigned char token[128];
    parserInput.token_return = token;
    parserInput.token_len = 128;

    unsigned char values[2048];
    parserInput.values_return = values;
    parserInput.values_len = 2048;

    unsigned char values6[2048];
    parserInput.values6_return = values6;
    parserInput.values6_len = 2048;

    parserInput.buflen = strlen(input);

    modifier(&parserInput);

    parserInput.parser_return =
        parser(parserInput.buf, parserInput.buflen,
               parserInput.tid_return, &parserInput.tid_len,
               parserInput.id_return,
               parserInput.info_hash_return,
               parserInput.target_return,
               &parserInput.port_return,
               parserInput.token_return, &parserInput.token_len,
               parserInput.nodes_return, &parserInput.nodes_len,
               parserInput.nodes6_return, &parserInput.nodes6_len,
               parserInput.values_return, &parserInput.values_len,
               parserInput.values6_return, &parserInput.values6_len,
               &parserInput.want_return);

    return validator(&parserInput);
}

static void nullModifier(struct dht_parser_input *parserInput)
{
    assert(parserInput != NULL);
}

/**
 * We need a mock serializer so that we have a function
 * which emulates the old parser with the new one.
 */
int
parse_message1(const unsigned char *buf, int buflen,
               unsigned char *tid_return, int *tid_len,
               unsigned char *id_return,
               unsigned char *info_hash_return,
               unsigned char *target_return,
               unsigned short *port_return,
               unsigned char *token_return, int *token_len,
               unsigned char *nodes_return, int *nodes_len,
               unsigned char *nodes6_return, int *nodes6_len,
               unsigned char *values_return, int *values_len,
               unsigned char *values6_return, int *values6_len,
               int *want_return)
{
    char buffer[1600];
    struct MemAllocator* alloc = BufferAllocator_new(buffer, 1600);
    struct Reader* reader = ArrayReader_new(buf, buflen, alloc);
    bobj_t* message;
    if (bobj_parse(reader, alloc, &message) != 0) {
printf("\n\n\nparse failed\n\n\n");
        return -1;
    }
    char testBuff[3200];
    struct Writer* writer = ArrayWriter_new(testBuff, 3200, alloc);
    bobj_print(writer, message);
    writer->write("\0", 1, writer);
    printf("\n\n%s\n\n", testBuff);
/*
    benc_dict_entry_t *messageDict = NULL;
    bbuf_t *buffer = bbuf_new(buflen, NULL);
    memcpy(buffer->base, buf, buflen);
    benc_dict_decode(buffer, &messageDict);
    bbuf_free(buffer);
    buffer = NULL;
    if (messageDict == NULL) {
        debugf("Failed to parse message.");
        return -1;
    }
    bobj_t *message = bobj_new(BENC_DICT);
    message->as.dict = messageDict;
*/
    int out = parse_message2(message,
                             tid_return, tid_len,
                             id_return,
                             info_hash_return,
                             target_return,
                             port_return,
                             token_return, token_len,
                             nodes_return, nodes_len,
                             nodes6_return, nodes6_len,
                             values_return, values_len,
                             values6_return, values6_len,
                             want_return);

    /*bobj_free(message);*/
    return out;
}
