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
#ifndef Hash_H
#define Hash_H

/*
 * SipHash is a family of pseudorandom functions (a.k.a. keyed hash functions)
 * optimized for speed on short messages.
 *
 * Target applications include network traffic authentication and defense against
 * hash-flooding DoS attacks.
 *
 * Solution inspired by code from:
 *     Jean-Philippe Aumasson (https://131002.net/siphash/siphash24.c)
 * */

#include <stdint.h>

#define Hash_ROTL(x,b) (uint64_t)( ((x) << (b)) | ( (x) >> (64 - (b))) )

#define Hash_U8TO64_LE(p) \
    (((uint64_t)((p)[0])      ) | \
     ((uint64_t)((p)[1]) <<  8) | \
     ((uint64_t)((p)[2]) << 16) | \
     ((uint64_t)((p)[3]) << 24) | \
     ((uint64_t)((p)[4]) << 32) | \
     ((uint64_t)((p)[5]) << 40) | \
     ((uint64_t)((p)[6]) << 48) | \
     ((uint64_t)((p)[7]) << 56))

#define Hash_SIPROUND            \
    do {              \
        v0 += v1; v1=Hash_ROTL(v1,13); v1 ^= v0; v0=Hash_ROTL(v0,32); \
        v2 += v3; v3=Hash_ROTL(v3,16); v3 ^= v2;                      \
        v0 += v3; v3=Hash_ROTL(v3,21); v3 ^= v0;                      \
        v2 += v1; v1=Hash_ROTL(v1,17); v1 ^= v2; v2=Hash_ROTL(v2,32); \
    } while (0);

/* SipHash-2-4 */
static uint64_t Hash_compute(const uint8_t* in, int length, uint8_t key[16])
{
    /* "somepseudorandomlygeneratedbytes" */
    uint64_t v0 = 0x736f6d6570736575ULL;
    uint64_t v1 = 0x646f72616e646f6dULL;
    uint64_t v2 = 0x6c7967656e657261ULL;
    uint64_t v3 = 0x7465646279746573ULL;
    uint64_t b;
    uint64_t k0 = Hash_U8TO64_LE( key );
    uint64_t k1 = Hash_U8TO64_LE( key + 8 );
    uint64_t m;
    const uint8_t* end = in + length - ( length % sizeof( uint64_t ) );
    const int left = length & 7;
    b = ( ( uint64_t )length) << 56;
    v3 ^= k1;
    v2 ^= k0;
    v1 ^= k1;
    v0 ^= k0;

    for ( ; in != end; in += 8 ) {
        m = Hash_U8TO64_LE( in );
        v3 ^= m;
        Hash_SIPROUND;
        Hash_SIPROUND;
        v0 ^= m;
    }

    switch( left )
    {
        case 7: b |= ( ( uint64_t )in[ 6] )  << 48;

        case 6: b |= ( ( uint64_t )in[ 5] )  << 40;

        case 5: b |= ( ( uint64_t )in[ 4] )  << 32;

        case 4: b |= ( ( uint64_t )in[ 3] )  << 24;

        case 3: b |= ( ( uint64_t )in[ 2] )  << 16;

        case 2: b |= ( ( uint64_t )in[ 1] )  <<  8;

        case 1: b |= ( ( uint64_t )in[ 0] ); break;

        case 0: break;
    }

    v3 ^= b;
    Hash_SIPROUND;
    Hash_SIPROUND;
    v0 ^= b;
    v2 ^= 0xff;
    Hash_SIPROUND;
    Hash_SIPROUND;
    Hash_SIPROUND;
    Hash_SIPROUND;
    b = v0 ^ v1 ^ v2  ^ v3;
    return b;
}

#endif
