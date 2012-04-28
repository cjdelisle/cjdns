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
#ifndef AddrTools_H
#define AddrTools_H

#include "util/Endian.h"
#include "util/Hex.h"

#include <string.h>
#include <stdint.h>

/** Takes the path in host byte order. */
static inline void AddrTools_printPath(uint8_t out[20], uint64_t path)
{
    uint64_t path_be = Endian_hostToBigEndian64(path);
    uint8_t bytes[16];
    Hex_encode(bytes, 16, (uint8_t*) &path_be, 8);
    out[ 0] = bytes[ 0];
    out[ 1] = bytes[ 1];
    out[ 2] = bytes[ 2];
    out[ 3] = bytes[ 3];
    out[ 4] = '.';
    out[ 5] = bytes[ 4];
    out[ 6] = bytes[ 5];
    out[ 7] = bytes[ 6];
    out[ 8] = bytes[ 7];
    out[ 9] = '.';
    out[10] = bytes[ 8];
    out[11] = bytes[ 9];
    out[12] = bytes[10];
    out[13] = bytes[11];
    out[14] = '.';
    out[15] = bytes[12];
    out[16] = bytes[13];
    out[17] = bytes[14];
    out[18] = bytes[15];
    out[19] = '\0';
}

/**
 * Parse out a path.
 *
 * @param out a pointer to a number which will be set to the path in HOST BYTE ORDER.
 * @param netAddr a string representation of the path such as "0000.1111.2222.3333" in Big Endian.
 * @return 0 if successful, -1 if the netAddr is malformed.
 */
static inline int AddrTools_parsePath(uint64_t* out, uint8_t netAddr[20])
{
    if (netAddr[4] != '.' || netAddr[9] != '.' || netAddr[14] != '.') {
        return -1;
    }

    uint8_t hex[16] = {
        netAddr[ 0],
        netAddr[ 1],
        netAddr[ 2],
        netAddr[ 3],

        netAddr[ 5],
        netAddr[ 6],
        netAddr[ 7],
        netAddr[ 8],

        netAddr[10],
        netAddr[11],
        netAddr[12],
        netAddr[13],

        netAddr[15],
        netAddr[16],
        netAddr[17],
        netAddr[18]
    };

    uint8_t numberBytes[8];
    if (Hex_decode(numberBytes, 8, hex, 16) != 8) {
        return -1;
    }
    uint64_t out_be;
    memcpy(&out_be, numberBytes, 8);
    *out = Endian_bigEndianToHost64(out_be);

    return 0;
}

static inline void AddrTools_printIp(uint8_t output[40], uint8_t binIp[16])
{
    uint8_t hex[32];
    Hex_encode(hex, 32, binIp, 16);

    for (int i = 0; i < 8; i++) {
        for (int j = 0; j < 4; j++) {
            output[(i * j) + j] = hex[i * j];
        }
    }
}

/**
 * Parse out an address.
 *
 * @param out a pointer to a byte array which will be set to the bytes of the ipv6 address.
 * @param hexAddr a string representation of the ipv6 address such as:
 *                "fc4f:630d:e499:8f5b:c49f:6e6b:01ae:3120".
 * @return 0 if successful, -1 if the hexAddr is malformed.
 */
static inline int AddrTools_parseIp(uint8_t out[16], uint8_t hexAddr[40])
{
    for (int i = 4; i < 39; i += 5) {
        if (hexAddr[i] != ':') {
            return -1;
        }
    }

    uint8_t hex[32];
    int j = 0;
    for (int i = 0; i < 40; i++) {
        hex[j++] = hexAddr[i++];
        hex[j++] = hexAddr[i++];
        hex[j++] = hexAddr[i++];
        hex[j++] = hexAddr[i++];
    }

    if (Hex_decode(out, 16, hex, 32) != 16) {
        return -1;
    }

    return 0;
}

#endif
