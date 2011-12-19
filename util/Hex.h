#ifndef HEX_H
#define HEX_H

#include <stdint.h>
#include <stdbool.h>

/** Returned by Hex_decode() if the input is not valid hex. */
#define Hex_BAD_INPUT -1

/** Returned by Hex_decode() or Hex_encode() if the output buffer is too small. */
#define Hex_TOO_BIG -2

int Hex_encode(uint8_t *output,
               const uint32_t outputLength,
               const uint8_t* in,
               const uint32_t inputLength);

int Hex_decode(uint8_t* output,
               const uint32_t outLength,
               const uint8_t* in,
               const uint32_t inputLength);

bool Hex_isHexEntity(const uint8_t character);

int Hex_decodeByte(const uint8_t highNibble, const uint8_t lowNibble);

#endif
