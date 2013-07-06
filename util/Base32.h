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
#ifndef Base32_H
#define Base32_H

#include <stdint.h>

/*
 * Base32 encode or decode a byte array using the format defined in:
 * http://dnscurve.org/in-implement.html
 */

/** Returned by Base32_decode() if the input is not valid base32. */
#define Base32_BAD_INPUT -1

/** Returned by Base32_decode() or Base32_encode() if the output buffer is too small. */
#define Base32_TOO_BIG -2

/**
 * Decode a base32 encoded number.
 *
 * @param output the place to put the decoded bytes.
 *               This may be modified even if there is a decoding error.
 * @param outLength the length of the output array, if the decoded output is longer,
 *                  Base32_TOO_BIG will be returned.
 * @param in the buffer holding the base32 encoded number.
 * @param inputLength the length of the input buffer.
 * @return the length of the output if all goes well, Base32_BAD_INPUT if the input
 *         is not valid base32, or Base32_TOO_BIG if the output buffer is not large
 *         enough to handle the output.
 */
static inline int Base32_decode(uint8_t* output,
                                const uint32_t outLength,
                                const uint8_t* in,
                                const uint32_t inputLength)
{
    // Maps ascii character inputs to the numbers
    // Invalid characters are represented by 99
    static const uint8_t numForAscii[] =
    {
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
        99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,99,
         0, 1, 2, 3, 4, 5, 6, 7, 8, 9,99,99,99,99,99,99,
        99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
        21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99,
        99,99,10,11,12,99,13,14,15,99,16,17,18,19,20,99,
        21,22,23,24,25,26,27,28,29,30,31,99,99,99,99,99
    };

    uint32_t outIndex = 0;
    uint32_t inputIndex = 0;
    uint32_t nextByte = 0;
    uint32_t bits = 0;

    while (inputIndex < inputLength) {
        if (in[inputIndex] & 0x80) {
            return Base32_BAD_INPUT;
        }
        const uint8_t b = numForAscii[in[inputIndex++]];
        if (b > 31) {
            return Base32_BAD_INPUT;
        }

        nextByte |= ((unsigned) b) << bits;
        bits += 5;

        if (bits >= 8) {
            if (outIndex >= outLength) {
                return Base32_TOO_BIG;
            }
            output[outIndex++] = nextByte;
            bits -= 8;
            nextByte >>= 8;
        }
    }

    if (bits >= 5 || nextByte) {
        return Base32_BAD_INPUT;
    }

    return outIndex;
}

/**
 * Base32 encode a number.
 *
 * @param output the place to put the base32 encoded output.
 *               This may be modified even if there is a encoding error.
 * @param outLength the length of the output array, if the encoded output is longer,
 *                  Base32_TOO_BIG will be returned.
 * @param in the buffer holding the bytes to encode.
 * @param inputLength the length of the input buffer.
 * @return the length of the output if all goes well,
 *         or Base32_TOO_BIG if the output buffer is not large enough to handle the output.
 */
static inline int Base32_encode(uint8_t* output,
                                const uint32_t outputLength,
                                const uint8_t* in,
                                const uint32_t inputLength)
{
    uint32_t outIndex = 0;
    uint32_t inIndex = 0;
    uint32_t work = 0;
    uint32_t bits = 0;
    static const uint8_t* kChars = (uint8_t*) "0123456789bcdfghjklmnpqrstuvwxyz";

    while (inIndex < inputLength) {
        work |= ((unsigned) in[inIndex++]) << bits;
        bits += 8;

        while (bits >= 5) {
            if (outIndex >= outputLength) {
                return Base32_TOO_BIG;
            }
            output[outIndex++] = kChars[work & 31];
            bits -= 5;
            work >>= 5;
        }
    }

    if (bits) {
        if (outIndex >= outputLength) {
            return Base32_TOO_BIG;
        }
        output[outIndex++] = kChars[work & 31];
        bits -= 5;
        work >>= 5;
    }

    if (outIndex < outputLength) {
        output[outIndex] = '\0';
    }
    return outIndex;
}

#endif
