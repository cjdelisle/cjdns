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
#ifndef ENDIAN_H
#define ENDIAN_H

#include <stdint.h>

// ntohs() ntohl()
#ifdef WIN32
    #include <Winsock2.h>
#else
    #include <arpa/inet.h>
#endif

#if defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
    #if (__BYTE_ORDER == __BIG_ENDIAN)
        #define Endian_isBigEndian() 1
    #else
        #define Endian_isBigEndian() 0
    #endif
#else
    static uint32_t Endian_isBigEndian()
    {
        union {
            uint32_t i;
            char c[4];
        } bint = {0x01020304};

        return bint.c[0] == 1; 
    }
#endif


#define Endian_hostToBigEndian16(x) htons(x)
#define Endian_bigEndianToHost16(x) Endian_hostToBigEndian16(x)


#define Endian_hostToBigEndian32(x) htonl(x)
#define Endian_bigEndianToHost32(x) Endian_hostToBigEndian32(x)



#ifdef htobe64
    #define Endian_hostToBigEndian64(x) htobe64(x)
#else
    #if defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && defined(__bswap_64)
        #define Endian_hostToBigEndian64(x) __bswap_64(x)
    #elif defined(__BYTE_ORDER) && defined(__BIG_ENDIAN)
        #define Endian_hostToBigEndian64(x) (x)
    #else
        static inline uint64_t Endian_hostToBigEndian64(uint64_t input)
        {
            if (Endian_isBigEndian()) {
                return htonl(input >> 32) | htonl(input << 32);
            } else {
                return input;
            }
        }
    #endif
#endif
#define Endian_bigEndianToHost64(x) Endian_hostToBigEndian64(x)


#endif
