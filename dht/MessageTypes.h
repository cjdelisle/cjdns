/**
 * These are set on outgoing messages because it is hard to determine statelessly
 * the context of a request. TODO: set messageType on incoming messages as well.
 */
#define MessageTypes_PING                  1
#define MessageTypes_PONG                  2
#define MessageTypes_FIND_NODE             3
#define MessageTypes_GET_PEERS             4
#define MessageTypes_FOUND_NODE            5
#define MessageTypes_NO_PEERS_HERES_NODE   6
#define MessageTypes_GOT_PEERS             7
#define MessageTypes_ANNOUNCE_PEER         8
#define MessageTypes_PEER_ANNOUNCED        9
#define MessageTypes_ERROR                 10
