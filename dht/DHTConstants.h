#include "libbenc/benc.h"

/* Cookie which is bounced back so queries aren't confused. */
benc_bstr_t DHTConstants_transactionId;

/* Key for message type entry. */
benc_bstr_t DHTConstants_messageType;

/* Possible message types: "q" (query), "r" (reply), "e" (error) */
benc_bstr_t DHTConstants_query;
benc_bstr_t DHTConstants_reply;
benc_bstr_t DHTConstants_error;

/* Query types. */
benc_bstr_t DHTConstants_ping;
benc_bstr_t DHTConstants_findNode;
benc_bstr_t DHTConstants_getPeers;
benc_bstr_t DHTConstants_announcePeer;

/* Arguments included in message. */
benc_bstr_t DHTConstants_arguments;

/* Possible message arguments. */
/* myId is included with all query types. */
benc_bstr_t DHTConstants_myId;
/* targetId is included with the findNode query only. */
benc_bstr_t DHTConstants_targetId;
/* a findNode query response contains either nodes/nodes6 or values. */
/* nodes == ids of nodes as a char* made by concatinating the node ids. */
benc_bstr_t DHTConstants_nodes;
/*
 * Nodes who are located in ipv6 space.
 * http://www.pps.jussieu.fr/~jch/software/bittorrent/bep-dht-ipv6.html
 */
benc_bstr_t DHTConstants_nodes6;
/*
 * This represents a list of network stacks for which which nodes should be
 * shared. Want is only used with getPeers and findNode requests.
 */
benc_bstr_t DHTConstants_want;
benc_bstr_t DHTConstants_wantIp4;
benc_bstr_t DHTConstants_wantIp6;
/* values == ip address + ports of nodes, this is a bEncoded list. */
benc_bstr_t DHTConstants_values;
/* infoHash is included with getPeers and announcePeer query types. */
benc_bstr_t DHTConstants_infoHash;
/*
 * authToken is included in the response to getPeers queries
 * and queries of announcePeer type.
 */
benc_bstr_t DHTConstants_authToken;
/* port is included only in announcePeer queries. */
benc_bstr_t DHTConstants_port;

