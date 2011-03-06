/**
 * These are set on outgoing messages because it is hard to
 * determine statelessly the context of a request.
 * TODO: set messageType on incoming messages as well.
 *
 * The protocol defines only 2 classifications of messages,
 * either a message is a query or a reply. For any message
 * (messageType & MessageTypes_QUERY || messageType & MessageType_REPLY)
 * must be true.
 * Also, for every message
 * ((messageType & MessageTypes_CLASS) == MessageTypes_QUERY
 *  || (messageType & MessageTypes_CLASS) == MessageTypes_REPLY)
 * must be true.
 *
 * More classifications of messages could be added by halving the
 * number of possible message types or expanding the data type.
 */

/** Utility for removing the message type leaving only the message class. */
#define MessageTypes_CLASS                      0xC000

/** A message class for messages which are sent unsolicited to get a response. */
#define MessageTypes_QUERY                      0x4000

/** A message class for messages which are in response to solicitations. */
#define MessageTypes_REPLY                      0x8000

/**
 * An ERROR message is a subset of a REPLY message.
 * An ERROR still keeps it's type IE an ERROR in response to a ANNOUNCE_PEER
 * is an ERROR and also a PEER_ANNOUNCED message.
 * (MessageTypes_ERROR & MessageTypes_CLASS) == MessageTypes_REPLY is true.
 */
#define MessageTypes_ERROR                      0xA000

/**
 * Check the type of message without checking the class
 * MessageTypes_IS_TYPE(message->messageType, MessageTypes_PING)
 * will return true for both PING and PONG messages.
 */
#define MessageTypes_IS_TYPE(messageType, checkAgainst) \
    ((messageType &~ MessageTypes_CLASS) & (checkAgainst &~ MessageTypes_CLASS))

/*------------------------Query Messages------------------------*/

/** A message for which the type information could not be determined. */
#define MessageTypes_UNPARSABLE           0

/** Typical ping message to see if a node is alive. */
#define MessageTypes_PING                (1 | MessageTypes_QUERY)

/** find_node message which is looking for nodes near a given id. */
#define MessageTypes_FIND_NODE           (2 | MessageTypes_QUERY)

/** get_peers message soliciting a list of peers for a torrent. */
#define MessageTypes_GET_PEERS           (3 | MessageTypes_QUERY)

/** announce_peer message which is asking for the node's id to be stored. */
#define MessageTypes_ANNOUNCE_PEER       (4 | MessageTypes_QUERY)


/*------------------------Reply Messages------------------------*/

/** Typical response to a ping message, indicates the node is alive. */
#define MessageTypes_PONG                (1 | MessageTypes_REPLY)

/**
 * Response to a find_node message which provides 8 nearest node
 * ids, ip addresses and ports.
 */
#define MessageTypes_FOUND_NODE          (2 | MessageTypes_REPLY)

/**
 * Response to a get_peers request. This will return a list of peer ip
 * addresses and ports and a set of node ids and ip addresses of nearby nodes.
 * If it doesn't know of any peers, it will (obviously) only send nodes.
 */
#define MessageTypes_GOT_PEERS           (3 | MessageTypes_REPLY)

/** Small response to an announce peer request just to tell the node that it's done. */
#define MessageTypes_PEER_ANNOUNCED      (4 | MessageTypes_REPLY)

/** Allow these definitions to be imported without namespace. */
#ifdef STATIC_IMPORT
#undef STATIC_IMPORT

#define CLASS          MessageTypes_CLASS
#define QUERY          MessageTypes_QUERY
#define REPLY          MessageTypes_REPLY
#define ERROR          MessageTypes_ERROR
#define IS_TYPE(messageType, checkAgainst) \
    MessageTypes_ IS_TYPE(messageType, checkAgainst)
#define UNPARSABLE     MessageTypes_UNPARSABLE
#define PING           MessageTypes_PING
#define FIND_NODE      MessageTypes_FIND_NODE
#define GET_PEERS      MessageTypes_GET_PEERS
#define ANNOUNCE_PEER  MessageTypes_ANNOUNCE_PEER
#define PONG           MessageTypes_PONG
#define FOUND_NODE     MessageTypes_FOUND_NODE
#define GOT_PEERS      MessageTypes_GOT_PEERS
#define PEER_ANNOUNCED MessageTypes_PEER_ANNOUNCED

#endif
