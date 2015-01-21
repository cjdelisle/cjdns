/* vim: set expandtab ts=4 sw=4: */
/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "util/Hex.h"

#include <stdint.h>
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
};

static const char* hexEntities = "0123456789abcdef";

int Hex_encode(uint8_t* output,
               const uint32_t outputLength,
               const uint8_t* in,
               const uint32_t inputLength)
{
    if (outputLength < inputLength * 2) {
        return Hex_TOO_BIG;
    } else if (outputLength > inputLength * 2) {
        output[inputLength * 2] = '\0';
    }

    for (uint32_t i = 0; i < inputLength; i++) {
        output[i * 2] = hexEntities[in[i] >> 4];
        output[i * 2 + 1] = hexEntities[in[i] & 15];
    }

    return inputLength * 2;
}

int Hex_decodeByte(const uint8_t highNibble, const uint8_t lowNibble)
{
    if ((highNibble & 0x80) | (lowNibble & 0x80)) {
        return -1;
    }
    int high = numForAscii[highNibble];
    int low = numForAscii[lowNibble];
    return (high + low < 31) ? (high << 4) | low : -1;
}

bool Hex_isHexEntity(const uint8_t character)
{
    return !(character & 0x80) && (numForAscii[character] < 99);
}

int Hex_decode(uint8_t* output,
               const uint32_t outLength,
               const uint8_t* hex,
               const uint32_t length)
{
    if (length % 1) {
        return Hex_BAD_INPUT;
    } else if (outLength < (length / 2)) {
        return Hex_TOO_BIG;
    } else if (outLength > (length / 2)) {
        output[length / 2] = '\0';
    }

    for (uint32_t i = 0; i < length; i += 2) {
        int byte = Hex_decodeByte(hex[i], hex[i + 1]);
        if (byte == -1) {
            return Hex_BAD_INPUT;
        }
        output[i / 2] = (uint8_t) byte;
    }

    return length / 2;
}

uint8_t Hex_encodeLowNibble(const uint8_t nibble)
{
    return hexEntities[nibble & 15];
}

char* Hex_print(void* bytes, uint32_t length, struct Allocator* alloc)
{
    int outLen = length * 2;
    char* outBuf = Allocator_malloc(alloc, outLen+1);
    Assert_true(Hex_encode(outBuf, outLen, (uint8_t*) bytes, length) == outLen);
    outBuf[outLen] = '\0';
    return outBuf;
}
