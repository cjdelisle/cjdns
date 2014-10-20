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

#include "util/Bits.h"
#include "util/Endian.h"
#include "util/Hex.h"
#include "util/platform/Sockaddr.h"

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
static inline int AddrTools_parsePath(uint64_t* out, const uint8_t netAddr[20])
{
    if (netAddr[4] != '.' || netAddr[9] != '.' || netAddr[14] != '.' || netAddr[19] != '\0') {
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
    Bits_memcpyConst(&out_be, numberBytes, 8);
    *out = Endian_bigEndianToHost64(out_be);

    return 0;
}

static inline void AddrTools_printIp(uint8_t output[40], const uint8_t binIp[16])
{
    Hex_encode(&output[0], 4, &binIp[0], 2);
    output[4] = ':';
    Hex_encode(&output[5], 4, &binIp[2], 2);
    output[9] = ':';
    Hex_encode(&output[10], 4, &binIp[4], 2);
    output[14] = ':';
    Hex_encode(&output[15], 4, &binIp[6], 2);
    output[19] = ':';
    Hex_encode(&output[20], 4, &binIp[8], 2);
    output[24] = ':';
    Hex_encode(&output[25], 4, &binIp[10], 2);
    output[29] = ':';
    Hex_encode(&output[30], 4, &binIp[12], 2);
    output[34] = ':';
    Hex_encode(&output[35], 4, &binIp[14], 2);
    output[39] = '\0';
}

static inline void AddrTools_printShortIp(uint8_t output[40], const uint8_t binIp[16])
{
    /* The chances of hitting :0:0: and breaking
     * RFC5952 are 1 in (1 / (2^16))^2 * 6.
     * E. Siler
     */

    char *p = output;

    for (int i = 0; i < 16;) {
        if ((size_t)p != (size_t)output) {
            *p++= ':';
        }

        if (binIp[i] > 0x0F) {
            Hex_encode(p, 2, &binIp[i++], 1);
            p += 2;
        } else if (binIp[i] > 0x00) {
            *p++ = Hex_encodeLowNibble(binIp[i++]);
        } else {
            ++i;
            if (binIp[i] > 0x0F) {
                Hex_encode(p, 2, &binIp[i++], 1);
                p += 2;
            } else {
                *p++ = Hex_encodeLowNibble(binIp[i++]);
            }
            continue;
        }
        Hex_encode(p, 2, &binIp[i++], 1);
        p += 2;
    }
    *p = '\0';
}

/**
 * Parse out an address.
 *
 * @param out a pointer to a byte array which will be set to the bytes of the ipv6 address.
 * @param hexAddr a string representation of the ipv6 address such as:
 *                "fc4f:630d:e499:8f5b:c49f:6e6b:01ae:3120".
 * @return 0 if successful, -1 if the hexAddr is malformed.
 */
static inline int AddrTools_parseIp(uint8_t out[16], const uint8_t hexAddr[40])
{
    struct Sockaddr_storage ss;
    if (Sockaddr_parse((const char*) hexAddr, &ss)
        || Sockaddr_getFamily(&ss.addr) != Sockaddr_AF_INET6)
    {
        return -1;
    }
    uint8_t* addr = NULL;
    Sockaddr_getAddress(&ss.addr, &addr);
    Bits_memcpyConst(out, addr, 16);
    return 0;
}

/**
 * Parse out an ethernet MAC address.
 *
 * @param out a pointer to a byte array which will be set to the bytes of the MAC address.
 * @param hexAddr a string representation of an ethernet MAC address such as:
 *                "00:11:22:33:44:55"
 * @return 0 if successful, -1 if the hexAddr is malformed.
 */
static inline int AddrTools_parseMac(uint8_t out[6], const uint8_t hexAddr[17])
{
    for (int i = 2; i < 15; i += 3) {
        if (hexAddr[i] != ':') {
            return -1;
        }
    }

    uint8_t hex[12];
    int j = 0;
    for (int i = 0; i < 18; i++) {
        hex[j++] = hexAddr[i++];
        hex[j++] = hexAddr[i++];
    }

    if (Hex_decode(out, 6, hex, 12) != 6) {
        return -1;
    }

    return 0;
}

static inline void AddrTools_printMac(uint8_t output[18], const uint8_t binMac[6])
{
    uint8_t hex[12];
    Hex_encode(hex, 12, binMac, 6);

    output[ 0] = hex[ 0];
    output[ 1] = hex[ 1];
    output[ 2] = ':';
    output[ 3] = hex[ 2];
    output[ 4] = hex[ 3];
    output[ 5] = ':';
    output[ 6] = hex[ 4];
    output[ 7] = hex[ 5];
    output[ 8] = ':';
    output[ 9] = hex[ 6];
    output[10] = hex[ 7];
    output[11] = ':';
    output[12] = hex[ 8];
    output[13] = hex[ 9];
    output[14] = ':';
    output[15] = hex[10];
    output[16] = hex[11];
    output[17] = '\0';
}

#endif
