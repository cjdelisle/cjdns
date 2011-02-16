#include "dht/DHTConstants.h"

/* Cookie which is bounced back so queries aren't confused. */
benc_bstr_t DHTConstants_transactionId = {1, "t"};

/* Key for message type entry. */
benc_bstr_t DHTConstants_messageType = {1, "y"};

/* Possible message types: "q" (query), "r" (reply), "e" (error) */
benc_bstr_t DHTConstants_query = {1, "q"};
benc_bstr_t DHTConstants_reply = {1, "r"};
benc_bstr_t DHTConstants_error = {1, "e"};

/* Query types. */
benc_bstr_t DHTConstants_ping = {4, "ping"};
benc_bstr_t DHTConstants_findNode = {9, "find_node"};
benc_bstr_t DHTConstants_getPeers = {9, "get_peers"};
benc_bstr_t DHTConstants_announcePeer = {13, "announce_peer"};

/* Arguments included in message. */
benc_bstr_t DHTConstants_arguments = {1, "a"};

/* Possible message arguments. */
/* myId is included with all query types. */
benc_bstr_t DHTConstants_myId = {2, "id"};
/* targetId is included with the findNode query only. */
benc_bstr_t DHTConstants_targetId = {6, "target"};
/* a findNode query response contains either nodes/nodes6 or values. */
/* nodes == ids of nodes as a char* made by concatinating the node ids. */
benc_bstr_t DHTConstants_nodes = {5, "nodes"};
/*
 * Nodes who are located in ipv6 space.
 * http://www.pps.jussieu.fr/~jch/software/bittorrent/bep-dht-ipv6.html
 */
benc_bstr_t DHTConstants_nodes6 = {6, "nodes6"};
/*
 * This represents a list of network stacks for which which nodes should be
 * shared. Want is only used with getPeers and findNode requests.
 */
benc_bstr_t DHTConstants_want = {4, "want"};
benc_bstr_t DHTConstants_wantIp4 = {2, "n4"};
benc_bstr_t DHTConstants_wantIp6 = {2, "n6"};
/* values == ip address + ports of nodes, this is a bEncoded list. */
benc_bstr_t DHTConstants_values = {6, "values"};
/* infoHash is included with getPeers and announcePeer query types. */
benc_bstr_t DHTConstants_infoHash = {9, "info_hash"};
/*
 * authToken is included in the response to getPeers queries
 * and queries of announcePeer type.
 */
benc_bstr_t DHTConstants_authToken = {5, "token"};
/* port is included only in announcePeer queries. */
benc_bstr_t DHTConstants_port = {4, "port"};
