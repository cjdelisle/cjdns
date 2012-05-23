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
#ifndef NumberCompress_H
#define NumberCompress_H

#include <stdint.h>

/* right now 3 implementations:
 * - fixed 4 bits: 15 peers
 * - fixed 8 bits: 240 peers
 * - dynamically 4-10 bits: 256 peers
 */

#ifndef CJDNS_MAX_PEERS
    #error CJDNS_MAX_PEERS needs to be defined.
#endif

#if CJDNS_MAX_PEERS > 256
    #error "Not more than 256 peers supported"
#endif

/* public interface */
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label);
static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number);
static inline uint64_t NumberCompress_getCompressed(const uint32_t number,
                                                    const uint32_t bitsUsed);
static inline uint32_t NumberCompress_getDecompressed(const uint64_t label,
                                                      const uint32_t bitsUsed);
// also a constant NumberCompress_INTERFACES defining the maximum number of interfaces

/* Number 1 is always encoded as the 4 bits 0001; and each label ending in 0001 is decoded as
 * 4 bit number 1
 * This is required by the protocol. */



/* implementations - select one based on CJDNS_MAX_PEERS
 *
 * they are all accesible with their internal names for unit testing
 */

/**********************
 * Fixed 4 bit scheme: 15 peers + 1 router
 **********************/
# define NumberCompress_4_INTERFACES 16
static inline uint32_t NumberCompress_4_bitsUsedForLabel(const uint64_t label) {
    return 4;
}
static inline uint32_t NumberCompress_4_bitsUsedForNumber(const uint32_t number) {
    return 4;
}
static inline uint64_t NumberCompress_4_getCompressed(const uint32_t number,
                                                      const uint32_t bitsUsed) {
    return number;
}
static inline uint32_t NumberCompress_4_getDecompressed(const uint64_t label,
                                                        const uint32_t bitsUsed) {
    return label & 0xf;
}


/**********************
 * (Fixed) 8 bit scheme: 240 peers + 1 router
 **********************/
// Basic idea: encode number XXXXYYYY as YYYY(XXXX)'; (XXXX)' is XXXX+1 if XXXX != 0, otherwise XXXX
// that way XXXX is never 1
// so map numbers 16-239 -> 32-255, and 240 <-> 1, then swap nibbles
# define NumberCompress_8_INTERFACES 241
static inline uint32_t NumberCompress_8_bitsUsedForLabel(const uint64_t label)
{
    if (1 == (label & 0xf)) return 4;
    return 8;
}

static inline uint32_t NumberCompress_8_bitsUsedForNumber(const uint32_t number)
{
    if (1 == number) return 4;
    return 8;
}

static inline uint64_t NumberCompress_8_getCompressed(const uint32_t number,
                                                      const uint32_t bitsUsed)
{
    if (1 == number) return 1;
    if (240 == number) return 0x10;
    uint32_t low = number & 0xf;
    uint32_t high = (number >> 4) & 0xf;
    if (high > 0) ++high;
    return (low << 4) + high;
}

static inline uint32_t NumberCompress_8_getDecompressed(const uint64_t label,
                                                        const uint32_t bitsUsed)
{
    uint32_t low = label & 0xf;
    if (1 == low) return 1;
    if (0x10 == (label & 0xff)) return 240;
    uint32_t high = (label >> 4) & 0xf;
    if (low > 0) --low; // low != 1
    return (low << 4) + high;
}


/*
 * Dynamic number compression scheme:
 *
 * scheme   data             suffix         range    bits used
 * route 0001                               1            4 (number 1 always encoded as 0001)
 *   0   000-111                 1          0-7          4 (swapped number 0 and 1)
 *   1   00000-11111            10          0-32         7 (skip the number 1)
 *   2   00000000-11111111      00          0-256        10 (skip the number 1)
 */
# define NumberCompress_dyn_INTERFACES 257
static inline uint32_t NumberCompress_dyn_bitsUsedForLabel(const uint64_t label)
{
    if (0 != (label & 0x1)) return 4;
    if (0 != (label & 0x2)) return 7;
    return 10;
}

static inline uint32_t NumberCompress_dyn_bitsUsedForNumber(const uint32_t number)
{
    if (number < 8) {
        return 4;
    } else if (number < 33) {
        return 7;
    } else {
        return 10;
    }
}

static inline uint64_t NumberCompress_dyn_getCompressed(const uint32_t number,
                                                        const uint32_t bitsUsed)
{
    if (1 == number) return 1;
    switch (bitsUsed) {
        case 4:
            if (0 == number) return 3;
            return (number << 1) | 1;
        case 7:
            if (0 == number) return 2;
            // skip the number 1
            return ((number-1) << 2) | 2;
        case 10:
            if (0 == number) return 0;
            // skip the number 1
            return ((number-1) << 2);
        default: return 0;
    }
}

static inline uint32_t NumberCompress_dyn_getDecompressed(const uint64_t label,
                                                          const uint32_t bitsUsed)
{
    uint32_t number;
    switch (bitsUsed) {
        case 4:
            number = (label >> 1) & 0x7u;
            if (0 == number) return 1;
            if (1 == number) return 0;
            return number;
        case 7:
            number = (label >> 2) & 0x1fu;
            if (0 != number) ++number; // skip the number 1
            return number;
        case 10:
            number = (label >> 2) & 0xffu;
            if (0 != number) ++number; // skip the number 1
            return number;
        default: return 0;
    }
}



#if CJDNS_MAX_PEERS < 16

# define NumberCompress_INTERFACES NumberCompress_4_INTERFACES
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label) {
    return NumberCompress_4_bitsUsedForLabel(label);
}
static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number) {
    return NumberCompress_4_bitsUsedForNumber(number);
}
static inline uint64_t NumberCompress_getCompressed(const uint32_t number,
                                                    const uint32_t bitsUsed)
{
    return NumberCompress_4_getCompressed(number, bitsUsed);
}
static inline uint32_t NumberCompress_getDecompressed(const uint64_t label,
                                                      const uint32_t bitsUsed)
{
    return NumberCompress_4_getDecompressed(label, bitsUsed);
}

#elif CJDNS_MAX_PEERS < 241

# define NumberCompress_INTERFACES NumberCompress_8_INTERFACES
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label) {
    return NumberCompress_8_bitsUsedForLabel(label);
}
static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number) {
    return NumberCompress_8_bitsUsedForNumber(number);
}
static inline uint64_t NumberCompress_getCompressed(const uint32_t number,
                                                    const uint32_t bitsUsed)
{
    return NumberCompress_8_getCompressed(number, bitsUsed);
}
static inline uint32_t NumberCompress_getDecompressed(const uint64_t label,
                                                      const uint32_t bitsUsed)
{
    return NumberCompress_8_getDecompressed(label, bitsUsed);
}

#else

# define NumberCompress_INTERFACES NumberCompress_dyn_INTERFACES
static inline uint32_t NumberCompress_bitsUsedForLabel(const uint64_t label) {
    return NumberCompress_dyn_bitsUsedForLabel(label);
}
static inline uint32_t NumberCompress_bitsUsedForNumber(const uint32_t number) {
    return NumberCompress_dyn_bitsUsedForNumber(number);
}
static inline uint64_t NumberCompress_getCompressed(const uint32_t number,
                                                    const uint32_t bitsUsed)
{
    return NumberCompress_dyn_getCompressed(number, bitsUsed);
}
static inline uint32_t NumberCompress_getDecompressed(const uint64_t label,
                                                      const uint32_t bitsUsed)
{
    return NumberCompress_dyn_getDecompressed(label, bitsUsed);
}

#endif

#endif
