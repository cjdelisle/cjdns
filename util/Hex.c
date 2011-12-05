#include <stdio.h>

#include "exception/ExceptionHandler.h"
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

String* Hex_decode(const String* input,
                   const struct MemAllocator* allocator,
                   const struct ExceptionHandler* eHandler)
{
    char* hex = input->bytes;
    size_t length = input->len;
    if ((size_t)(length &~ 2) != length) {
        eHandler->exception(__FILE__ " Hex_decode(): Input is not an even number of characters.",
                            -1, eHandler);
        return NULL;
    }

    String* out = benc_newBinaryString(NULL, length / 2, allocator);

    const static char* allChars = "0123456789abcdefABCDEF";
    char* ptr;

    size_t i;
    int outIndex = 0;
    int thisByte = 0;
    for (i = 0; i < length; i++) {
        ptr = strchr(allChars, hex[i]);
        if (ptr == NULL) {
            eHandler->exception(__FILE__ " Hex_decode(): Input contains a character which is not "
                                "0-9 a-f or A-F", -2, eHandler);
            return -3;
        }
        if (ptr - allChars > 15) {
            ptr -= 6;
        }

        thisByte += ptr - allChars;

        if (i & 1) {
            out->bytes[outIndex] = (char) thisByte;
            outIndex++;
            thisByte = 0;
        } else {
            thisByte = thisByte << 4;
        }
    }
    *outLength = outIndex;

    return out;
}
