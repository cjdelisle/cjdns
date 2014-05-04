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
#ifndef ArchInfo_H
#define ArchInfo_H

#include "memory/Allocator.h"

#include "util/Linker.h"
Linker_require("util/ArchInfo.c")

#include <stdint.h>

enum ArchInfo {
    ArchInfo_UNKNOWN = -1
};

/**
 * ArchInfo enum (not really an enum)
 * NOTE: Value is in host endian order.
 *
 *                  1               2               3
 *  0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7 0 1 2 3 4 5 6 7
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |     Architecture Specific     | Res |B|  Bits |      Arch     |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * Arch: The type of processor (enum ArchInfo_Arch
 * B:    1 if the architecture is Big Endian
 * Bits: Number of bits in the architecture (8 times 2**bits)
 * Res:  Reserved
 * Architecture Specific: see enum Arch_<your architecture> for details.
 */

#define ArchInfo_Arch_SHIFT 0
#define ArchInfo_Arch_BITS (0xff << ArchInfo_Arch_SHIFT)
enum ArchInfo_Arch {
    ArchInfo_Arch_AMD64,
    ArchInfo_Arch_I386,
    ArchInfo_Arch_ARM,
    ArchInfo_Arch_PPC,
    ArchInfo_Arch_SPARC,
    ArchInfo_Arch_MIPS,
    ArchInfo_Arch_UNKNOWN = -1
};

static inline enum ArchInfo_Arch ArchInfo_getArch(enum ArchInfo ai)
{
    return ai & ArchInfo_Arch_BITS;
}

#define ArchInfo_Bits_SHIFT 8
#define ArchInfo_Bits_BITS    (0xf << ArchInfo_Bits_SHIFT)
enum ArchInfo_Bits
{
    ArchInfo_Bits_8 =   (1 << ArchInfo_Bits_SHIFT),
    ArchInfo_Bits_16 =  (2 << ArchInfo_Bits_SHIFT),
    ArchInfo_Bits_32 =  (3 << ArchInfo_Bits_SHIFT),
    ArchInfo_Bits_64 =  (4 << ArchInfo_Bits_SHIFT),
    ArchInfo_Bits_128 = (5 << ArchInfo_Bits_SHIFT),
    ArchInfo_Bits_UNKNOWN = -1
};

static inline int ArchInfo_getBits(enum ArchInfo ai)
{
    switch (ai & ArchInfo_Bits_BITS) {
        case ArchInfo_Bits_16: return 16;
        case ArchInfo_Bits_32: return 32;
        case ArchInfo_Bits_64: return 64;
        case ArchInfo_Bits_128: return 128;
        default: return -1;
    }
}

#define ArchInfo_Endian_SHIFT 12
#define ArchInfo_Endian_BITS (1 << ArchInfo_Endian_SHIFT)

static inline int ArchInfo_isBigEndian(enum ArchInfo ai)
{
    return !!(ai & ArchInfo_Endian_BITS);
}

#define ArchInfo_ARCH_SPECIFIC_SHIFT 16
#define ArchInfo_ARCH_SPECIFIC_BITS (0xffff << ArchInfo_ARCH_SPECIFIC_SHIFT)

#define ArchInfo_Arm_SUBTYPE_BITS (4 << ArchInfo_ARCH_SPECIFIC_SHIFT)
enum ArchInfo_Arm
{
    ArchInfo_Arm_EABI = (1 << ArchInfo_ARCH_SPECIFIC_SHIFT),
    ArchInfo_Arm_NEON = (1 << (ArchInfo_ARCH_SPECIFIC_SHIFT+15))
};

#define ArchInfo_Mips_SUBTYPE_BITS (4 << ArchInfo_ARCH_SPECIFIC_SHIFT)
enum ArchInfo_Mips
{
    ArchInfo_Mips_OABI = (1 << ArchInfo_ARCH_SPECIFIC_SHIFT),
    ArchInfo_Mips_NABI = (2 << ArchInfo_ARCH_SPECIFIC_SHIFT)
};

#define ArchInfo_I386_SUBTYPE_BITS (4 << ArchInfo_ARCH_SPECIFIC_SHIFT)
enum ArchInfo_I386
{
    ArchInfo_I386_SSE2 = (1 << (ArchInfo_ARCH_SPECIFIC_SHIFT+15))
};

#define ArchInfo_Sparc_SUBTYPE_BITS (4 << ArchInfo_ARCH_SPECIFIC_SHIFT)
enum ArchInfo_Sparc
{
    ArchInfo_Sparc_v8 = (1 << ArchInfo_ARCH_SPECIFIC_SHIFT),
    ArchInfo_Sparc_v9 = (2 << ArchInfo_ARCH_SPECIFIC_SHIFT)
};

/**
 * Convert the architecture to a linux/audit.h AUDIT_ARCH_* number.
 * Returns 0xffffffff in case of error or on a non-linux system.
 */
uint32_t ArchInfo_toAuditArch(enum ArchInfo ai);

/** Describe the architecture as a string. */
char* ArchInfo_describe(enum ArchInfo ai, struct Allocator* alloc);

/** Detect the architecture. */
enum ArchInfo ArchInfo_detect();

#endif
