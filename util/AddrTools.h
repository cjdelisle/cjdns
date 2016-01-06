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

#include "util/Linker.h"
Linker_require("util/AddrTools.c");

#include <stdint.h>

/** Takes the path in host byte order. */
void AddrTools_printPath(uint8_t out[20], uint64_t path);

/**
 * Parse out a path.
 *
 * @param out a pointer to a number which will be set to the path in HOST BYTE ORDER.
 * @param netAddr a string representation of the path such as "0000.1111.2222.3333" in Big Endian.
 * @return 0 if successful, -1 if the netAddr is malformed.
 */
int AddrTools_parsePath(uint64_t* out, const uint8_t netAddr[20]);

void AddrTools_printIp(uint8_t output[40], const uint8_t binIp[16]);

void AddrTools_printShortIp(uint8_t output[40], const uint8_t binIp[16]);

/**
 * Parse out an address.
 *
 * @param out a pointer to a byte array which will be set to the bytes of the ipv6 address.
 * @param hexAddr a string representation of the ipv6 address such as:
 *                "fc4f:630d:e499:8f5b:c49f:6e6b:01ae:3120".
 * @return 0 if successful, -1 if the hexAddr is malformed.
 */
int AddrTools_parseIp(uint8_t out[16], const uint8_t hexAddr[40]);

/**
 * Parse out an ethernet MAC address.
 *
 * @param out a pointer to a byte array which will be set to the bytes of the MAC address.
 * @param hexAddr a string representation of an ethernet MAC address such as:
 *                "00:11:22:33:44:55"
 * @return 0 if successful, -1 if the hexAddr is malformed.
 */
int AddrTools_parseMac(uint8_t out[6], const uint8_t hexAddr[17]);

void AddrTools_printMac(uint8_t output[18], const uint8_t binMac[6]);

#endif
