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
#ifndef Key_H
#define Key_H

#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("crypto/Key.c");

#include <stdint.h>

char* Key_parse_strerror(int error);

#define Key_parse_TOO_SHORT -1
#define Key_parse_MALFORMED -2
#define Key_parse_DECODE_FAILED -3

/** Invalid cjdns key (doesn't hash to an address beginning with FC) */
#define Key_parse_INVALID -4

/**
 * Parse a key.
 *
 * @param key a public key similar to h9xgk0418x538kg2h5yw4n32rkl4wk4wplkzsmltpg20q78bu7q0.k
 * @param keyBytesOut a 32 byte array which will be set to the key.
 * @param ip6Out a 16 byte array which will be set to the IPv6 address, if NULL it will be skipped
 *               and the function will not check if the first byte is FC.
 * @return an error code or 0 if no error.
 */
int Key_parse(String* key, uint8_t keyBytesOut[32], uint8_t ip6Out[16]);

String* Key_stringify(uint8_t key[32], struct Allocator* alloc);

#endif
