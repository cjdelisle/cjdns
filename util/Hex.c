#include <stdio.h>
#include <string.h>

#include "exception/ExceptionHandler.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "util/Hex.h"

#include <stdbool.h>

static const uint8_t numForAscii[] =
{
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
     0, 1, 2, 3, 4, 5, 6, 7, 8, 9,99,99,99,99,99,99,
    99,10,11,12,13,14,15,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,10,11,12,13,14,15,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
    99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
};

String* Hex_encode(const String* input, const struct MemAllocator* allocator)
{
    String* out = benc_newBinaryString(NULL, input->len * 2, allocator);
    for(uint32_t i = 0; i < input->len; i++) {
        sprintf(&out->bytes[2 * i], "%02x", input->bytes[i] & 0xFF);
    }
    return out;
}

int Hex_decodeByte(const uint8_t highNibble, const uint8_t lowNibble)
{
    int high = numForAscii[highNibble];
    int low = numForAscii[lowNibble];
    return (high + low < 31) ? (high << 4) | low : -1;
}

String* Hex_decode(const String* input,
                   const struct MemAllocator* allocator,
                   struct ExceptionHandler* eHandler)
{
    uint8_t* hex = (uint8_t*) input->bytes;
    size_t length = input->len;
    if ((size_t)(length &~ 2) != length) {
        eHandler->exception(__FILE__ " Hex_decode(): Input is not an even number of characters.",
                            -1, eHandler);
        return NULL;
    }

    String* out = benc_newBinaryString(NULL, length / 2, allocator);

    for (uint32_t i = 0; i < length; i += 2) {
        int byte = Hex_decodeByte(hex[i], hex[i + 1]);
        if (byte == -1) {
            eHandler->exception(__FILE__ " Hex_decode(): Input contains a character which is not "
                                "0-9 a-f or A-F", -2, eHandler);
            return NULL;
        }
        out->bytes[i / 2] = (uint8_t) byte;
    }

    return out;
}

bool Hex_isHexEntity(const uint8_t character)
{
    return numForAscii[character] < 99;
}
