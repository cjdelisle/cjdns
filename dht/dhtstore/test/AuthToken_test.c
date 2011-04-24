#include "dht/dhtstore/DHTStoreTools.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"

static int expiredToken()
{
    char buffer[10000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 10000);

    // This token was generated when I wrote the test, after 10 minutes
    // it became invalid and will not be valid until the next rollover of the clock.
    return 0 != DHTStoreTools_isTokenValid(benc_newString("\xF5\x41\x36\x5A\xF2\x9B\xC7\xDA", allocator),
                                           benc_newString("1234567890abcdefghij", allocator),
                                           benc_newString("hijklmnopqrstuvwxyz0", allocator),
                                           "1234567890qwertyui",
                                           "1234567890asdfghjklz");
}

static int invalidToken()
{
    char buffer[10000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 10000);

    return 0 != DHTStoreTools_isTokenValid(benc_newString("00000000", allocator),
                                           benc_newString("1234567890abcdefghij", allocator),
                                           benc_newString("hijklmnopqrstuvwxyz0", allocator),
                                           "1234567890qwertyui",
                                           "1234567890asdfghjklz");
}

static int newToken()
{
    char buffer[10000];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 10000);

    String* token = DHTStoreTools_generateToken(benc_newString("1234567890abcdefghij", allocator),
                                                benc_newString("hijklmnopqrstuvwxyz0", allocator),
                                                "1234567890qwertyui",
                                                "1234567890asdfghjklz",
                                                allocator);

    return 0 == DHTStoreTools_isTokenValid(token,
                                           benc_newString("1234567890abcdefghij", allocator),
                                           benc_newString("hijklmnopqrstuvwxyz0", allocator),
                                           "1234567890qwertyui",
                                           "1234567890asdfghjklz");
}

int main()
{
    return newToken() | expiredToken() | invalidToken();
}
