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
typedef uint64_t u64;
typedef uint32_t u32;
typedef uint8_t   u8;

#define Hash_ROTL(x,b) (u64)( ((x) << (b)) | ( (x) >> (64 - (b))) )

#define Hash_U8TO64_LE(p) \
    (((u64)((p)[0])      ) | \
     ((u64)((p)[1]) <<  8) | \
     ((u64)((p)[2]) << 16) | \
     ((u64)((p)[3]) << 24) | \
     ((u64)((p)[4]) << 32) | \
     ((u64)((p)[5]) << 40) | \
     ((u64)((p)[6]) << 48) | \
     ((u64)((p)[7]) << 56))

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
    u64 v0 = 0x736f6d6570736575ULL;
    u64 v1 = 0x646f72616e646f6dULL;
    u64 v2 = 0x6c7967656e657261ULL;
    u64 v3 = 0x7465646279746573ULL;
    u64 b;
    u64 k0 = Hash_U8TO64_LE( key );
    u64 k1 = Hash_U8TO64_LE( key + 8 );
    u64 m;
    const u8 *end = in + length - ( length % sizeof( u64 ) );
    const int left = length & 7;
    b = ( ( u64 )length) << 56;
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
        case 7: b |= ( ( u64 )in[ 6] )  << 48;

        case 6: b |= ( ( u64 )in[ 5] )  << 40;

        case 5: b |= ( ( u64 )in[ 4] )  << 32;

        case 4: b |= ( ( u64 )in[ 3] )  << 24;

        case 3: b |= ( ( u64 )in[ 2] )  << 16;

        case 2: b |= ( ( u64 )in[ 1] )  <<  8;

        case 1: b |= ( ( u64 )in[ 0] ); break;

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
