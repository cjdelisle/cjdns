#include "libbenc/benc.h"

#define BSTRING_PTR(string) &(benc_bstr_t) {strlen(string), string}
const benc_bstr_t* const DHTStoreConstants_VALUE = BSTRING_PTR("v");
const benc_bstr_t* const DHTStoreConstants_KEY = BSTRING_PTR("k");
const benc_bstr_t* const DHTStoreConstants_DATE = BSTRING_PTR("date");
const benc_bstr_t* const DHTStoreConstants_SIGNATURE = BSTRING_PTR("sig");
#undef BSTRING_PTR
