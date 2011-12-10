#include "exception/ExceptionHandler.h"
#include "libbenc/benc.h"
#include "memory/MemAllocator.h"

String* Hex_encode(const String* input, const struct MemAllocator* allocator);

String* Hex_decode(const String* input,
                   const struct MemAllocator* allocator,
                   struct ExceptionHandler* eHandler);
