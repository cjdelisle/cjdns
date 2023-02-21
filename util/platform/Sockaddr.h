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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#ifndef Sockaddr_H
#define Sockaddr_H

#include "memory/Allocator.h"
#include "util/Endian.h"
#include "util/Linker.h"
Linker_require("util/platform/Sockaddr.c")

#include <stdint.h>

struct Sockaddr
{
    /** the length of this sockaddr, this field is included in the length. */
    uint16_t addrLen;

    #define Sockaddr_flags_BCAST  1
    #define Sockaddr_flags_PREFIX (1<<1)
    uint8_t flags;

    #define Sockaddr_PLATFORM 0
    #define Sockaddr_HANDLE 1
    uint8_t type;

    /** Only applies if flags & Sockaddr_flags_PREFIX is true. */
    uint8_t prefix;

    uint8_t pad1;
    uint16_t pad2;
};

/** The number of bytes of space taken for representing the addrLen at the beginning. */
#define Sockaddr_OVERHEAD 8

/** The maximum possible size for the native sockaddr (not including Sockaddr_OVERHEAD) */
#define Sockaddr_MAXSIZE 128
struct Sockaddr_storage
{
    struct Sockaddr addr;
    uint64_t nativeAddr[Sockaddr_MAXSIZE / 8];
};

/** 127.0.0.1 and ::1 addresses for building from. */
extern const struct Sockaddr* const Sockaddr_LOOPBACK_be;
extern const struct Sockaddr* const Sockaddr_LOOPBACK_le;
#define Sockaddr_LOOPBACK (Endian_isBigEndian() ? Sockaddr_LOOPBACK_be : Sockaddr_LOOPBACK_le)

extern const struct Sockaddr* const Sockaddr_LOOPBACK6;

/**
 * Parse an internal form of Sockaddr which is used to represent a uint32_t handle.
 * If the length of the address is not equal to sizeof(struct Sockaddr) or the type is not
 * Sockaddr_HANDLE then this returns Sockaddr_addrHandle_INVALID.
 */
#define Sockaddr_addrHandle_INVALID 0xffffffffu
uint32_t Sockaddr_addrHandle(const struct Sockaddr* addr);

/**
 * Create a handle sockaddr from a numeric handle, if handle is equal to
 * Sockaddr_addrHandle_INVALID then this will trigger an assertion.
 */
void Sockaddr_addrFromHandle(struct Sockaddr* addr, uint32_t handle);


int Sockaddr_getPrefix(struct Sockaddr* addr);

/**
 * Parse a sockaddr from a string, may be IP4 or IP6.
 *
 * @param str a string representation of the sockaddr.
 * @param output a sockaddr_storage to populate, if null then the validity of the string will be
 *               checked only.
 * @return 0 if all goes well, -1 if there is an error.
 */
int Sockaddr_parse(const char* str, struct Sockaddr_storage* out);

/**
 * Convert a sockaddr to a printable string.
 */
char* Sockaddr_print(struct Sockaddr* addr, struct Allocator* alloc);

/**
 * Get the port from a sockaddr if applicable.
 *
 * @param a sockaddr.
 * @return the port number or -1 if not applicable to this sockaddr.
 */
int Sockaddr_getPort(const struct Sockaddr* sa);

/**
 * Set the port for a sockaddr if applicable.
 *
 * @param sa a sockaddr.
 * @param port the port number to set.
 * @return 0 if all goes well, -1 if not applicable to this sockaddr.
 */
int Sockaddr_setPort(struct Sockaddr* sa, uint16_t port);

/**
 * Get the address family for the address.
 *
 * @param a sockaddr.
 * @return the AF number for this sockaddr.
 */
extern const int Sockaddr_AF_INET;
extern const int Sockaddr_AF_INET6;
int Sockaddr_getFamily(const struct Sockaddr* sa);

/**
 * Get the address stored in a sockaddr.
 *
 * @param sa a sockaddr.
 * @param addrPtr a pointer which will be set to the actual address component of the sockaddr.
 *                If NULL, the length will be returned only.
 * @return the length of the address component in bytes, -1 if failed to parse.
 */
int Sockaddr_getAddress(struct Sockaddr* sa, void* addrPtr);

/**
 * Whatever the address is, attempt to represent it as an IPv6 address.
 * IPv6 addresses are easy.
 * IPv4 are represented as ::ffff::aabb::ccdd
 * Other addresses smaller or equal than 14 bytes are represented as fffe::<address bytes>
 * Other addresses larger than 14 bytes are represented as ffff::<14 bytes of sha256 hash of address>
 */
void Sockaddr_asIp6(uint8_t addrOut[static 16], const struct Sockaddr* sockaddr);

/**
 * Output the native form of a sockaddr.
 */
static inline void* Sockaddr_asNative(struct Sockaddr* sa)
{
    return (void*)(&sa[1]);
}

/**
 * @brief Initialize a Sockaddr in place from bytes.
 * 
 * @param out The location in memory to place the sockaddr data
 * @param bytes The bytes of the address, e.g. {127,0,0,1} (port not included)
 * @param addrFamily The address family (Sockaddr_AF_INET / Sockaddr_AF_INET6)
 * @return struct Sockaddr* which points to the memory of `out`
 */
struct Sockaddr* Sockaddr_initFromBytes(struct Sockaddr_storage* out, const uint8_t* bytes, int addrFamily);

/**
 * Sockaddr_fromBytes() takes
 * input as the bytes of the address eg: Sockaddr_fromBytes({127,0,0,1}, Sockaddr_AF_INET, alloc)
 */
struct Sockaddr* Sockaddr_fromBytes(const uint8_t* bytes, int addrFamily, struct Allocator* alloc);

/**
 * Clone the sockaddr, the clone will use only as much memory as the type of sockaddr requires.
 */
struct Sockaddr* Sockaddr_clone(const struct Sockaddr* addr, struct Allocator* alloc);

/**
 * Normalize inconsistent native sockaddr implementations
 */
void Sockaddr_normalizeNative(void* nativeSockaddr);

/**
 * Get a hash for hashtable lookup.
 */
uint32_t Sockaddr_hash(const struct Sockaddr* addr);

/**
 * Compare two sockaddrs for sorting, comparison does not put them in any specific order.
 */
int Sockaddr_compare(const struct Sockaddr* a, const struct Sockaddr* b);

#endif
