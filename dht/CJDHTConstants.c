#include "dht/CJDHTConstants.h"

// Signifying that this message is a query and defining the query type.
String* const CJDHTConstants_QUERY =      &(String) { .len = 1, .bytes = "q"    };

// Find nodes query. "fn"
String* const CJDHTConstants_QUERY_FN =   &(String) { .len = 2, .bytes = "fn"   };

// Ping query
String* const CJDHTConstants_QUERY_PING = &(String) { .len = 2, .bytes = "pn"   };

// A search target (address)
String* const CJDHTConstants_TARGET =     &(String) { .len = 3, .bytes = "tar"  };

// Response with nodes. "n"
String* const CJDHTConstants_NODES =      &(String) { .len = 1, .bytes = "n"    };

// Transaction id
String* const CJDHTConstants_TXID =       &(String) { .len = 4, .bytes = "txid" };
