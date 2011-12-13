#ifndef CJDHT_CONSTANTS_H
#define CJDHT_CONSTANTS_H

#include "libbenc/benc.h"

// Signifying that this message is a query and defining the query type.
String* const CJDHTConstants_QUERY;

// Find nodes query. "fn"
String* const CJDHTConstants_QUERY_FN;

// Ping query
String* const CJDHTConstants_QUERY_PING;

// A search target (address)
String* const CJDHTConstants_TARGET;

// Response with nodes. "n"
String* const CJDHTConstants_NODES;

// Transaction id
String* const CJDHTConstants_TXID;

#endif
