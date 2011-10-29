#include <stdio.h>

#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "util/Hex.h"

String* Hex_encode(const String* input, const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, input->len * 2, allocator);
    for(uint32_t i = 0; i < input->len; i++) {
        sprintf(&out->bytes[2 * i], "%02x", input->bytes[i] & 0xFF);
    }
    return out;
}
