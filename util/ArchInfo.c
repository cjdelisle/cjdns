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

#include "util/Bits.h"
#include "util/ArchInfo.h"
#include "util/CString.h"
#include <stdio.h>

#if !defined(linux) || defined(android)

uint32_t ArchInfo_toAuditArch(enum ArchInfo ai)
{
    return 0xffffffff;
}

#else

#include <linux/audit.h>

// TODO(cjd): understand why some machines are not defining this.
#ifndef EM_ARM
    #define EM_ARM 40
#endif

uint32_t ArchInfo_toAuditArch(enum ArchInfo ai)
{
    switch (ArchInfo_getArch(ai)) {
        case ArchInfo_Arch_AMD64: return AUDIT_ARCH_X86_64;
        case ArchInfo_Arch_I386: return AUDIT_ARCH_I386;
        case ArchInfo_Arch_ARM: {
            if (ArchInfo_isBigEndian(ai)) {
                return AUDIT_ARCH_ARMEB;
            }
            return AUDIT_ARCH_ARM;
        }
        case ArchInfo_Arch_PPC: {
            switch (ArchInfo_getBits(ai)) {
                case 32: return AUDIT_ARCH_PPC;
                case 64: return AUDIT_ARCH_PPC64;
                default:;
            }
            return 0xffffffff;
        }
        case ArchInfo_Arch_SPARC: {
            switch (ArchInfo_getBits(ai)) {
                case 32: return AUDIT_ARCH_SPARC;
                case 64: return AUDIT_ARCH_SPARC64;
                default:;
            }
            return 0xffffffff;
        }
        case ArchInfo_Arch_MIPS: {
            if (ArchInfo_isBigEndian(ai)) {
                switch (ArchInfo_getBits(ai)) {
                    case 32: return AUDIT_ARCH_MIPS;
                    case 64: return AUDIT_ARCH_MIPS64;
                    default:;
                }
                return 0xffffffff;
            } else {
                switch (ArchInfo_getBits(ai)) {
                    case ArchInfo_Bits_32: return AUDIT_ARCH_MIPSEL;
                    case ArchInfo_Bits_64: return AUDIT_ARCH_MIPSEL64;
                    default:;
                }
                return 0xffffffff;
            }
        }
        default: return 0xffffffff;
    }
}

#endif

static char* archStr(enum ArchInfo ai)
{
    switch (ArchInfo_getArch(ai)) {
        case ArchInfo_Arch_AMD64: return "AMD64";
        case ArchInfo_Arch_ARM:   return "ARM";
        case ArchInfo_Arch_PPC:   return "PPC";
        case ArchInfo_Arch_SPARC: return "SPARC";
        case ArchInfo_Arch_MIPS:  return "MIPS";
        case ArchInfo_Arch_I386:  return "i386";
        default: return "UNKNOWN";
    };
}

#define archFlags_BUFF_SZ 512
static void archFlags(enum ArchInfo arch, uint8_t buff[archFlags_BUFF_SZ])
{
    switch (ArchInfo_getArch(arch)) {
        case ArchInfo_Arch_ARM: {
            snprintf(buff, archFlags_BUFF_SZ, "%s%s",
                ((arch & ArchInfo_Arm_SUBTYPE_BITS) == ArchInfo_Arm_EABI) ? " EABI": "",
                (arch & ArchInfo_Arm_NEON) ? " NEON": ""
            );
            break;
        }
        case ArchInfo_Arch_SPARC: {
            switch (arch & ArchInfo_Sparc_SUBTYPE_BITS) {
                case ArchInfo_Sparc_v8: snprintf(buff, archFlags_BUFF_SZ, " sparcv8"); break;
                case ArchInfo_Sparc_v9: snprintf(buff, archFlags_BUFF_SZ, " sparcv9"); break;
            }
            break;
        }
        case ArchInfo_Arch_MIPS: {
            switch (arch & ArchInfo_Mips_SUBTYPE_BITS) {
                case ArchInfo_Mips_OABI: snprintf(buff, archFlags_BUFF_SZ, " OABI"); break;
                case ArchInfo_Mips_NABI: snprintf(buff, archFlags_BUFF_SZ, " NABI"); break;
            }
            break;
        }
        case ArchInfo_Arch_AMD64:
        case ArchInfo_Arch_PPC:
        default: {
            buff[0] = '\0';
            break;
        }
    };
}

char* ArchInfo_describe(enum ArchInfo ai, struct Allocator* alloc)
{
    uint8_t flagBuff[archFlags_BUFF_SZ];
    archFlags(ai, flagBuff);

    uint8_t buff[1024];
    snprintf(buff, 1024, "%s %d-bit %sEndian %s",
        archStr(ai),
        ArchInfo_getBits(ai),
        ArchInfo_isBigEndian(ai) ? "Big" : "Little",
        flagBuff);

    int len = CString_strlen(buff);
    Assert_true(len < 1024);
    if (buff[len-1] == ' ') { buff[--len] = '\0'; }
    char* out = Allocator_malloc(alloc, len+1);
    Bits_memcpy(out, buff, len+1);
    return out;
}

static enum ArchInfo_Bits detectBits()
{
    switch (sizeof(long)) {
        case 4: return ArchInfo_Bits_32;
        case 8: return ArchInfo_Bits_64;
        default: return ArchInfo_Bits_UNKNOWN;
    }
}

static enum ArchInfo_Bits assertBits(enum ArchInfo_Bits bits)
{
    enum ArchInfo_Bits detected = detectBits();
    if (bits != detected) { return ArchInfo_Bits_UNKNOWN; }
    return bits;
}

enum ArchInfo_Endian
{
    ArchInfo_Endian_LITTLE =  (0 << ArchInfo_Endian_SHIFT),
    ArchInfo_Endian_BIG =     (1 << ArchInfo_Endian_SHIFT),
    ArchInfo_Endian_UNKNOWN = -1,
};

static enum ArchInfo_Endian detectEndian()
{
    union {
        uint32_t i;
        uint8_t c[4];
    } bint = { .i = 0xff0000ee };

    if (bint.c[0] == 0xff) {
        return ArchInfo_Endian_BIG;
    } else if (bint.c[0] == 0xee) {
        return ArchInfo_Endian_LITTLE;
    }
    // 2+2=5
    Assert_true(0);
    return -1;
}

static enum ArchInfo_Endian assertEndian(enum ArchInfo_Endian end)
{
    enum ArchInfo_Endian detected = detectEndian();
    if (end != detected) { return ArchInfo_Endian_UNKNOWN; }
    return end;
}

enum ArchInfo ArchInfo_detect()
{
    enum ArchInfo arch = 0;

    #if defined(__amd64__) || defined(__x86_64__) || defined(__AMD64__) || \
        defined(_M_X64) || defined(__amd64)
        arch |= ArchInfo_Arch_AMD64;
    #elif defined(__i386__) || defined(__x86__) || defined(__X86__) || \
          defined(_M_IX86) || defined(__i386)
        arch |= ArchInfo_Arch_I386;
    #elif defined(__ARM_EABI__) || defined(__arm__)
        arch |= ArchInfo_Arch_ARM;
    #elif defined(__powerpc64__) || defined(__ppc64__) || defined(__PPC64__) || defined(_ARCH_PPC64)
        arch |= ArchInfo_Arch_PPC | ArchInfo_Bits_64;
    #elif defined(__powerpc__) || defined(__ppc__) || defined(__PPC__) || defined(_ARCH_PPC)
        arch |= ArchInfo_Arch_PPC | ArchInfo_Bits_32;
    #elif defined(__sparcv9__) || defined(__sparcv9)
        arch |= ArchInfo_Arch_SPARC | ArchInfo_Sparc_v9;
    #elif defined(__sparc_v8__)
        arch |= ArchInfo_Arch_SPARC | ArchInfo_Sparc_v8;
    #elif defined(__sparc__) || defined(__sparc)
        arch |= ArchInfo_Arch_SPARC;
    #elif defined(__mips__) || defined(__mips) || defined(__MIPS__)
        arch |= ArchInfo_Arch_MIPS;
    #else
        arch |= ArchInfo_UNKNOWN;
    #endif

    switch ((int)arch) {
        case ArchInfo_Arch_AMD64:
            return arch | assertBits(ArchInfo_Bits_64) | assertEndian(ArchInfo_Endian_LITTLE);
        case ArchInfo_Arch_I386: {
            #define HAS_SSE2 0
            #ifdef __SSE2__
                #undef HAS_SSE2
                #define HAS_SSE2 1
            #endif
            if (HAS_SSE2) {
                arch |= ArchInfo_I386_SSE2;
            }
            return arch | assertBits(ArchInfo_Bits_32) | assertEndian(ArchInfo_Endian_LITTLE);
        }
        case ArchInfo_Arch_ARM: {
            #define HAS_EABI 0
            #ifdef __ARM_EABI__
                #undef HAS_EABI
                #define HAS_EABI 1
            #endif
            if (HAS_EABI) {
                arch |= ArchInfo_Arm_EABI;
            }
            #define HAS_NEON 0
            #ifdef __ARM_NEON__
                #undef HAS_NEON
                #define HAS_NEON 1
            #endif
            if (HAS_NEON) {
                arch |= ArchInfo_Arm_NEON;
            }
            return arch | detectBits() | detectEndian();
        }
        case (ArchInfo_Arch_PPC | ArchInfo_Bits_64): {
            return arch | assertBits(ArchInfo_Bits_64) | assertEndian(ArchInfo_Endian_BIG);
        }
        case (ArchInfo_Arch_PPC | ArchInfo_Bits_32): {
            return arch | assertBits(ArchInfo_Bits_32) | assertEndian(ArchInfo_Endian_BIG);
        }
        case (ArchInfo_Arch_SPARC | ArchInfo_Sparc_v9): {
            return arch | assertBits(ArchInfo_Bits_64) | assertEndian(ArchInfo_Endian_BIG);
        }
        case (ArchInfo_Arch_SPARC | ArchInfo_Sparc_v8): {
            return arch | assertBits(ArchInfo_Bits_32) | assertEndian(ArchInfo_Endian_BIG);
        }
        case ArchInfo_Arch_SPARC: {
            arch |= detectBits();
            switch (ArchInfo_getBits(arch)) {
                case ArchInfo_Bits_32: arch |= ArchInfo_Sparc_v8; break;
                case ArchInfo_Bits_64: arch |= ArchInfo_Sparc_v9; break;
                default: return ArchInfo_UNKNOWN;
            }
            return arch | assertEndian(ArchInfo_Endian_BIG);
        }
        case ArchInfo_Arch_MIPS: {
            #define IS_OABI 0
            #if defined(_ABIO32)
                #undef IS_OABI
                #define IS_OABI 1
            #endif
            if (IS_OABI) {
                return arch | ArchInfo_Mips_OABI | assertBits(ArchInfo_Bits_32);
            }

            #define IS_NABI 0
            #if defined(_ABIN32)
                #undef IS_NABI
                #define IS_NABI 1
            #endif
            if (IS_NABI) {
                if (IS_OABI) { return ArchInfo_UNKNOWN; }
                return arch | ArchInfo_Mips_NABI | assertBits(ArchInfo_Bits_32);
            }

            // If it's not oabi or nabi and it's not mips64 then it's UNKNOWN.
            return arch | assertBits(ArchInfo_Bits_64);
        }
        default: return ArchInfo_UNKNOWN;
    }
}
