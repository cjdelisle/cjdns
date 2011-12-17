#ifndef HEX_H
#define HEX_H

#include "exception/ExceptionHandler.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"

#include <stdbool.h>

String* Hex_encode(const String* input, const struct MemAllocator* allocator);

String* Hex_decode(const String* input,
                   const struct MemAllocator* allocator,
                   struct ExceptionHandler* eHandler);

bool Hex_isHexEntity(const uint8_t character);

int Hex_decodeByte(const uint8_t highNibble, const uint8_t lowNibble);

#endif
