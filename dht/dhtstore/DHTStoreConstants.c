#include <string.h>

#include "libbenc/benc.h"

#define BSTRING_PTR(string) &(benc_bstr_t) {strlen(string), string}
const String* const DHTStoreConstants_VALUE = BSTRING_PTR("v");
const String* const DHTStoreConstants_KEY = BSTRING_PTR("k");
const String* const DHTStoreConstants_DATE = BSTRING_PTR("date");
const String* const DHTStoreConstants_SIGNATURE = BSTRING_PTR("sig");
#undef BSTRING_PTR
