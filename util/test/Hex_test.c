#include "crypto/Crypto.h"
#include "exception/AbortHandler.h"
#include "memory/BufferAllocator.h"
#include "memory/MemAllocator.h"
#include "util/Hex.h"

#include <stdio.h>
#include <assert.h>

int main()
{
    uint8_t buff[1024];
    struct MemAllocator* allocator = BufferAllocator_new(buff, 1024);

    uint8_t bytes[32];
    randombytes(bytes, 32);

    String* hex = Hex_encode(&(String) {.bytes=(char*)bytes, .len=32}, allocator);
    assert(hex && hex->len == 64);

    printf("hex encoded: %s\n", hex->bytes);

    String* bytes2 = Hex_decode(hex, allocator, AbortHandler_INSTANCE);
    assert(bytes2 && bytes2->len == 32);
    assert(memcmp(bytes, bytes2->bytes, 32) == 0);
}
