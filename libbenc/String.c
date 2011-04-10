#include <string.h>

#include "memory/MemAllocator.h"
#include "libbenc/benc.h"

/** @see benc.h */
String* benc_newString(const char* bytes, const struct MemAllocator* allocator)
{
    return benc_newBinaryString(bytes, strlen(bytes), allocator);
}

/** @see benc.h */
String* benc_newBinaryString(const char* bytes, size_t length, const struct MemAllocator* allocator)
{
    char* copy = allocator->malloc(length, allocator);
    memcpy(copy, bytes, length);
    benc_bstr_t* string = allocator->malloc(sizeof(benc_bstr_t), allocator);
    string->len = length;
    string->bytes = copy;
    return string;
}

int benc_bstr_compare(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return (a == NULL) - (b == NULL);
    }
    size_t i;
    int d;
    for (i = 0; i < (a->len < b->len ? a->len : b->len); i++)
    {
        d = a->bytes[i] - b->bytes[i];
        if (0 != d) return d;
    }
    return a->len - b->len;
}

int32_t benc_stringEquals(const String* a, const String* b)
{
    if (a == NULL || b == NULL) {
        return a == NULL && b == NULL;
    }
    return a->len == b->len && (memcmp(a->bytes, b->bytes, a->len) == 0);
}
