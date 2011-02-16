#include "dht/DHTConstants.h"
#include "libbenc/benc.h"
#include "libbenc/bencode.h"

/* Definitions taken from dht.c */
#define ERROR 0
#define REPLY 1
#define PING 2
#define FIND_NODE 3
#define GET_PEERS 4
#define ANNOUNCE_PEER 5

#define WANT4 1
#define WANT6 2
/* #end# */

#define DEBUG(m) /* printf(m) */
#define DEBUG2(x, y) /* printf(x, y) */

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
               int *want_return)
{
    /* Temp storage for values returned by dictionary lookups. */
    bobj_t *value = NULL;

    /* Temp storage for values which are converted to benc_str_t. */
    benc_bstr_t *stringValue = NULL;

#define QUERY 6

    /* Get the message and query types. */
    int thisMessageType;
    int thisQueryType;

    if ((value = bobj_dict_lookup(bencodedMessage, &DHTConstants_messageType)) == NULL) {
        thisMessageType = -1;
    } else {
        stringValue = value->as.bstr;

        if (benc_bstr_compare(stringValue, &DHTConstants_reply) == 0) {
            thisMessageType = REPLY;
        } else if (benc_bstr_compare(stringValue, &DHTConstants_error) == 0) {
            thisMessageType = ERROR;
        } else if (benc_bstr_compare(stringValue, &DHTConstants_query) == 0) {
            thisMessageType = QUERY;
        } else {
            thisMessageType = -1;
        }
    }

    /* Get the query type. */
    if (stringValue == NULL
        || (value = bobj_dict_lookup(bencodedMessage, stringValue)) == NULL)
    {
        thisQueryType = -1;
    } else {
        stringValue = value->as.bstr;
        if (benc_bstr_compare(stringValue, &DHTConstants_ping) == 0) {
            thisQueryType = PING;
        } else if (benc_bstr_compare(stringValue, &DHTConstants_findNode) == 0) {
            thisQueryType = FIND_NODE;
        } else if (benc_bstr_compare(stringValue, &DHTConstants_getPeers) == 0) {
            thisQueryType = GET_PEERS;
        } else if (benc_bstr_compare(stringValue, &DHTConstants_announcePeer) == 0) {
            thisQueryType = ANNOUNCE_PEER;
        } else {
            thisQueryType = -1;
        }
    }

    if (thisMessageType == -1) {
        DEBUG("Message which is not a query, response, nor error");
    } else if (thisQueryType == -1) {
        if (thisMessageType == QUERY) {
            DEBUG("Query which is not ping, findNode, getPeers, or announcePeer");
        }
        /* Responses do not have to tell what type of query they are responding to. */
    }

    /* This will be returned. */
    int out = (thisMessageType == QUERY) ? thisQueryType : thisMessageType;
#undef QUERY

/*
 * COPY(bobj_t *bObj,
 *      char *outBuffer,
 *      int *outBufferLength,
 *      benc_str_t *bStr)
 *
 * bStr is just a place to store the bObj after it is converted to a string.
 */
#define COPY(bObj, outBuffer, outBufferLength, bStr)         \
    if (outBuffer && outBufferLength) {                      \
        if (!bObj                                            \
            || (bStr = bObj->as.bstr) == NULL                \
            || (unsigned int) *outBufferLength < bStr->len   \
            || bStr->len < 1)                                \
        {                                                    \
            *outBufferLength = 0;                            \
        } else {                                             \
            memcpy(outBuffer, bStr->bytes, bStr->len);       \
            *outBufferLength = bStr->len;                    \
        }                                                    \
    }

    value = bobj_dict_lookup(bencodedMessage, &DHTConstants_transactionId);
    COPY(value, tid_return, tid_len, stringValue)

    /*
     * If the message is a reply
     * then arguments are filed under "r" rather than "a".
     */
    bobj_t *thisMessageArguments = NULL;
    value = NULL;
    if (thisMessageType == REPLY) {
         thisMessageArguments = bobj_dict_lookup(bencodedMessage, &DHTConstants_reply);
    } else if (thisMessageType != ERROR) {
         thisMessageArguments = bobj_dict_lookup(bencodedMessage, &DHTConstants_arguments);
    }


    if (!thisMessageArguments) {
        if (token_len) { *token_len = 0; }
        if (port_return) { *port_return = 0; }
        if (token_len) { *token_len = 0; }
        if (nodes_len) { *nodes_len = 0; }
        if (nodes6_len) { *nodes6_len = 0; }
        if (values_len) { *values_len = 0; }
        if (values6_len) { *values6_len = 0; }
        if (want_return) { *want_return = 0; }
        return out;
    }

/*
 * COPY_KNOWN_LENGTH(bobj_t *bObj,
 *                   char *outBuffer,
 *                   int length,
 *                   benc_bstr_t *bStr)
 */
#define COPY_KNOWN_LENGTH(bObj, outBuffer, length, bStr)              \
    if (bObj && outBuffer) {                                          \
        bStr = bObj->as.bstr;                                         \
        if (length == bStr->len) {                                    \
            memcpy(outBuffer, bStr->bytes, length);                   \
        } else {                                                      \
            memset(outBuffer, 0, length);                             \
        }                                                             \
    } else if (outBuffer) {                                           \
        memset(outBuffer, 0, length);                                 \
    }

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_myId);
    COPY_KNOWN_LENGTH(value, id_return, 20, stringValue)

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_infoHash);
    COPY_KNOWN_LENGTH(value, info_hash_return, 20, stringValue)

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_targetId);
    COPY_KNOWN_LENGTH(value, target_return, 20, stringValue)

#undef COPY_KNOWN_LENGTH

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_port);
    int portNum = 0;
    if (value) {
        portNum = value->as.int_;
    }
    if (portNum && portNum < 65536 && portNum > 0) {
        *port_return = (unsigned short) portNum;
    } else {
        *port_return = 0;
    }

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_authToken);
    COPY(value, token_return, token_len, stringValue)

    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_nodes);
    COPY(value, nodes_return, nodes_len, stringValue)
    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_nodes6);
    COPY(value, nodes6_return, nodes6_len, stringValue)

#undef COPY

    /* Values which are 6 char long are ip4, 18 char are ip6. */
    value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_values);
    benc_list_entry_t *valueList = NULL;
    if (value) {
        valueList = value->as.list;
    }
    size_t values_return_offset = 0;
    size_t values6_return_offset = 0;
    while (valueList) {
        value = valueList->elem;
        if (value) {
            stringValue = value->as.bstr;
            if (stringValue->len == 6) {
                /* IP4 address. 4 bytes + port# */
                if (values_return_offset + 6 < (unsigned int) *values_len) {
                    memcpy((char*) values_return + values_return_offset,
                           stringValue->bytes, 6);
                    values_return_offset += 6;
                }
            } else if (stringValue->len == 18) {
                /* IP6 address. 16 bytes + port# */
                if (values6_return_offset + 18 < (unsigned int) *values6_len) {
                    memcpy((char*) values6_return + values6_return_offset,
                           stringValue->bytes, 18);
                    values6_return_offset += 18;
                }
            } else {
                DEBUG2("Received weird value -- %d bytes.\n", (int) stringValue->len);
            }
        }
        valueList = valueList->next;
    }
    *values_len = values_return_offset;
    *values6_len = values6_return_offset;


    /* Does the peer want introductions to ip4 nodes or ip6 nodes? */
    if(want_return) {
        value = bobj_dict_lookup(thisMessageArguments, &DHTConstants_want);
        if (!value) {
            *want_return = -1;
        } else {
            benc_list_entry_t *peerWants = value->as.list;
            while (peerWants) {
                value = peerWants->elem;
                if (value) {
                    stringValue = value->as.bstr;
                    if (benc_bstr_compare(stringValue, &DHTConstants_wantIp4) == 0) {
                        *want_return |= WANT4;
                    } else if (benc_bstr_compare(stringValue, &DHTConstants_wantIp6) == 0) {
                        *want_return |= WANT6;
                    }
                }
                peerWants = peerWants->next;
            }
        }
    }

    return out;
}
