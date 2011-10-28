#ifndef DHT_STORE_CONSTANTS_H
#define DHT_STORE_CONSTANTS_H

#include "libbenc/benc.h"

// TODO: add header to check for unused first.
#define __unused __attribute__((unused))

static const __unused String* DHTStoreConstants_VALUE     = &(String) {1, "v"};
static const __unused String* DHTStoreConstants_KEY       = &(String) {1, "k"};
static const __unused String* DHTStoreConstants_DATE      = &(String) {4, "date"};
static const __unused String* DHTStoreConstants_SIGNATURE = &(String) {3, "sig"};

#endif
