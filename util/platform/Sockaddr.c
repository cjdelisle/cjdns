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
#include "rust/cjdns_sys/Rffi.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/CString.h"
#include "util/Bits.h"
#include "util/Hex.h"
#include "util/Hash.h"
#include "util/Base10.h"

#include <sodium/crypto_hash_sha256.h>

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>
#include <netinet/in.h>

struct Sockaddr_pvt
{
    struct Sockaddr pub;
    struct sockaddr_storage ss;
};
void Sockaddr_assertions(void);
void Sockaddr_assertions() {
    Assert_compileTime(sizeof(struct Sockaddr) == 8);
    Assert_compileTime(sizeof(struct sockaddr_storage) == 128);
    Assert_compileTime(sizeof(struct Sockaddr_pvt) == 128+8);
    Assert_compileTime(sizeof(struct Sockaddr_storage) == 128+8);
}

struct Sockaddr_in_pvt
{
    struct Sockaddr pub;
    struct sockaddr_in si;
};
struct Sockaddr_in6_pvt
{
    struct Sockaddr pub;
    struct sockaddr_in6 si;
};

const struct Sockaddr* const Sockaddr_LOOPBACK_be =
    (const struct Sockaddr*) &((const struct Sockaddr_in_pvt) {
        .pub = { .addrLen = sizeof(struct Sockaddr_in_pvt) },
        .si = {
            .sin_family = AF_INET,
            .sin_addr = { .s_addr = 0x7f000001 }
        }
    });
const struct Sockaddr* const Sockaddr_LOOPBACK_le =
    (const struct Sockaddr*) &((const struct Sockaddr_in_pvt) {
        .pub = { .addrLen = sizeof(struct Sockaddr_in_pvt) },
        .si = {
            .sin_family = AF_INET,
            .sin_addr = { .s_addr = 0x0100007f }
        }
    });
const struct Sockaddr* const Sockaddr_LOOPBACK6 =
    (const struct Sockaddr*) &((const struct Sockaddr_in6_pvt) {
        .pub = { .addrLen = sizeof(struct Sockaddr_in6_pvt) },
        .si = {
            .sin6_family = AF_INET6,
            .sin6_addr = { .s6_addr = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1 }}
        }
    });

int Sockaddr_getPrefix(struct Sockaddr* addr)
{
    if (addr->flags & Sockaddr_flags_PREFIX) {
        return addr->prefix;
    }
    int af = Sockaddr_getFamily(addr);
    if (af == Sockaddr_AF_INET) {
        return 32;
    } else if (af == Sockaddr_AF_INET6) {
        return 128;
    } else {
        return -1;
    }
}

int Sockaddr_parse(const char* input, struct Sockaddr_storage* out)
{
    struct Sockaddr_storage unusedOut;
    if (!out) {
        out = &unusedOut;
    }
    uint8_t buff[64] = {0};
    if (CString_strlen(input) > 63) {
        return -1;
    }
    CString_safeStrncpy(buff, input, 63);

    int64_t port = 0;
    char* lastColon = CString_strrchr(buff, ':');
    char* firstColon = CString_strchr(buff, ':');
    char* bracket = CString_strchr(buff, ']');
    if (!lastColon) {
        // ipv4, no port
    } else if (lastColon != firstColon && (!bracket || lastColon < bracket)) {
        // ipv6, no port
    } else {
        if (bracket && lastColon != &bracket[1]) { return -1; }
        if (Base10_fromString(&lastColon[1], &port)) { return -1; }
        if (port > 65535) { return -1; }
        *lastColon = '\0';
    }
    if (bracket) {
        *bracket = '\0';
        if (buff[0] != '[') { return -1; }
    } else if (buff[0] == '[') { return -1; }

    int64_t prefix = -1;
    char* slash = CString_strchr(buff, '/');
    if (slash) {
        *slash = '\0';
        if (!slash[1]) { return -1; }
        if (Base10_fromString(&slash[1], &prefix)) { return -1; }
    }

    Bits_memset(out, 0, sizeof(struct Sockaddr_storage));
    if (lastColon != firstColon) {
        // ipv6
        struct sockaddr_in6* in6 = (struct sockaddr_in6*) Sockaddr_asNative(&out->addr);
        if (Rffi_inet_pton(1, (char*) ((buff[0] == '[') ? &buff[1] : buff), (uint8_t*) &in6->sin6_addr)) {
            return -1;
        }
        out->addr.addrLen = sizeof(struct sockaddr_in6) + Sockaddr_OVERHEAD;
        in6->sin6_port = Endian_hostToBigEndian16(port);
        in6->sin6_family = AF_INET6;
    } else {
        struct sockaddr_in* in = ((struct sockaddr_in*) Sockaddr_asNative(&out->addr));
        if (Rffi_inet_pton(0, (char*) buff, (uint8_t*) &in->sin_addr)) {
            return -1;
        }
        out->addr.addrLen = sizeof(struct sockaddr_in) + Sockaddr_OVERHEAD;
        in->sin_port = Endian_hostToBigEndian16(port);
        in->sin_family = AF_INET;
    }
    if (prefix != -1) {
        if (prefix < 0 || prefix > 128) { return -1; }
        if (Sockaddr_getFamily(&out->addr) == Sockaddr_AF_INET && prefix > 32) { return -1; }
        out->addr.prefix = prefix;
        out->addr.flags |= Sockaddr_flags_PREFIX;
    }
    return 0;
}

struct Sockaddr* Sockaddr_clone(const struct Sockaddr* addr, struct Allocator* alloc)
{
    struct Sockaddr* out = Allocator_malloc(alloc, addr->addrLen);
    Bits_memcpy(out, addr, addr->addrLen);
    return out;
}

char* Sockaddr_print(struct Sockaddr* sockaddr, struct Allocator* alloc)
{
    if (sockaddr->addrLen < Sockaddr_OVERHEAD
        || sockaddr->addrLen > Sockaddr_MAXSIZE + Sockaddr_OVERHEAD) {
        return "invalid";
    }

    if (sockaddr->type == Sockaddr_HANDLE) {
        // handle address
        uint32_t handle = Sockaddr_addrHandle(sockaddr);
        const char* target = "";
        if (sockaddr->addrLen > Sockaddr_OVERHEAD) {
            struct Sockaddr* sa2 = &sockaddr[1];
            if (sa2->addrLen != sockaddr->addrLen - Sockaddr_OVERHEAD) {
                target = "<invalid length>";
            } else {
                target = Sockaddr_print(sa2, alloc);
            }
        }
        String* out = String_printf(alloc, "Handle:%u/%s", handle, target);
        return out->bytes;
    }

    struct Sockaddr_pvt* addr = (struct Sockaddr_pvt*) sockaddr;

    void* inAddr;
    uint16_t port;
    switch (addr->ss.ss_family) {
        case AF_INET:
            inAddr = &((struct sockaddr_in*) &addr->ss)->sin_addr;
            port = Endian_bigEndianToHost16(((struct sockaddr_in*)&addr->ss)->sin_port);
            break;
        case AF_INET6:
            inAddr = &((struct sockaddr_in6*) &addr->ss)->sin6_addr;
            port = Endian_bigEndianToHost16(((struct sockaddr_in6*)&addr->ss)->sin6_port);
            break;
        default: {
            uint8_t buff[Sockaddr_MAXSIZE * 2 + 1] = {0};
            Hex_encode(buff, sizeof(buff), (uint8_t*)sockaddr, sockaddr->addrLen);
            String* out = String_printf(alloc, "unknown (%s)", buff);
            return out->bytes;
        }
    };

    #define BUFF_SZ 64
    char printedAddr[BUFF_SZ] = {0};
    int ret = Rffi_inet_ntop(addr->ss.ss_family == AF_INET6, inAddr, printedAddr, BUFF_SZ - 1);
    if (ret != 0) {
        return "invalid";
    }

    char printedPrefix[16] = {0};
    if (addr->pub.flags & Sockaddr_flags_PREFIX) {
        snprintf(printedPrefix, 15, "/%u", addr->pub.prefix);
    }

    char printedPort[16] = {0};
    if (port) {
        snprintf(printedPort, 15, ":%u", port);
    }

    char finalAddr[128] = {0};
    const char* format = (port && addr->ss.ss_family == AF_INET6) ? "[%s%s]%s" : "%s%s%s";
    snprintf(finalAddr, 127, format, printedAddr, printedPrefix, printedPort);

    int totalLength = CString_strlen(finalAddr) + 1;
    char* out = Allocator_calloc(alloc, totalLength, 1);
    Bits_memcpy(out, finalAddr, totalLength);
    return out;
}

static uint16_t* getPortPtr(struct Sockaddr* sockaddr)
{
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)) {
        return NULL;
    }
    struct Sockaddr_pvt* sa = (struct Sockaddr_pvt*) sockaddr;
    switch (sa->ss.ss_family) {
        case AF_INET: return &((struct sockaddr_in*)&sa->ss)->sin_port;
        case AF_INET6: return &((struct sockaddr_in6*)&sa->ss)->sin6_port;
    }
    return NULL;
}

int Sockaddr_getPort(const struct Sockaddr* sockaddr)
{
    const uint16_t* pp = getPortPtr((struct Sockaddr*) sockaddr);
    return (pp) ? Endian_bigEndianToHost16(*pp) : -1;
}
int Sockaddr_getPort_fromRust(const struct Sockaddr* sockaddr)
{
    return Sockaddr_getPort(sockaddr);
}

int Sockaddr_setPort(struct Sockaddr* sockaddr, uint16_t port)
{
    uint16_t* pp = getPortPtr(sockaddr);
    if (pp) {
        *pp = Endian_hostToBigEndian16(port);
        return 0;
    }
    return -1;
}
int Sockaddr_setPort_fromRust(struct Sockaddr* sockaddr, uint16_t port)
{
    return Sockaddr_setPort(sockaddr, port);
}

int Sockaddr_getAddress(struct Sockaddr* sockaddr, void* addrPtr)
{
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)) {
        return -1;
    }
    struct Sockaddr_pvt* sa = (struct Sockaddr_pvt*) sockaddr;
    if (addrPtr) {
        void** ap = (void**) addrPtr;
        switch (sa->ss.ss_family) {
            case AF_INET: *ap = &((struct sockaddr_in*)&sa->ss)->sin_addr; break;
            case AF_INET6: *ap = &((struct sockaddr_in6*)&sa->ss)->sin6_addr; break;
        }
    }
    switch (sa->ss.ss_family) {
        case AF_INET: return 4;
        case AF_INET6: return 16;
    }
    return -1;
}

const int Sockaddr_AF_INET = AF_INET;
const int Sockaddr_AF_INET6 = AF_INET6;
int Sockaddr_getFamily(const struct Sockaddr* sockaddr)
{
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)) {
        return -1;
    }
    const struct Sockaddr_pvt* sa = (const struct Sockaddr_pvt*) sockaddr;
    return sa->ss.ss_family;
}

int Sockaddr_getFamily_fromRust(const struct Sockaddr* sockaddr)
{
    return Sockaddr_getFamily(sockaddr);
}

struct Sockaddr* Sockaddr_initFromBytes(struct Sockaddr_storage* out, const uint8_t* bytes, int addrFamily)
{
    switch (addrFamily) {
        case AF_INET: {
            struct Sockaddr_in_pvt* in = (struct Sockaddr_in_pvt*) out;
            Bits_memset(in, 0, sizeof *in);
            in->si.sin_family = AF_INET;
            Bits_memcpy(&in->si.sin_addr, bytes, 4);
            in->pub.addrLen = sizeof(struct Sockaddr_in_pvt);
            break;
        }
        case AF_INET6: {
            struct Sockaddr_in6_pvt* in = (struct Sockaddr_in6_pvt*) out;
            Bits_memset(in, 0, sizeof *in);
            in->si.sin6_family = AF_INET6;
            Bits_memcpy(&in->si.sin6_addr, bytes, 16);
            in->pub.addrLen = sizeof(struct Sockaddr_in6_pvt);
            break;
        }
        default: Assert_failure("unrecognized address type [%d]", addrFamily);
    }
    return &out->addr;
}

Sockaddr_t* Sockaddr_initFromBytes_fromRust(
    struct Sockaddr_storage* out,
    const uint8_t* bytes,
    int addrFamily
) {
    return Sockaddr_initFromBytes(out, bytes, addrFamily);
}

struct Sockaddr* Sockaddr_fromBytes(const uint8_t* bytes, int addrFamily, struct Allocator* alloc)
{
    struct Sockaddr_storage ss;
    struct Sockaddr* sa = Sockaddr_initFromBytes(&ss, bytes, addrFamily);
    return Sockaddr_clone(sa, alloc);
}

void Sockaddr_normalizeNative(void* nativeSockaddr)
{
#if defined(freebsd) || defined(openbsd) || defined(netbsd) || defined(darwin)
    ((struct sockaddr*)nativeSockaddr)->sa_len = 0;
#endif
}

uint32_t Sockaddr_hash(const struct Sockaddr* addr)
{
    return Hash_compute((uint8_t*)addr, addr->addrLen);
}

void Sockaddr_asIp6(uint8_t addrOut[static 16], const struct Sockaddr* sockaddr)
{
    Bits_memset(addrOut, 0, 16);
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)) {
        // Corrupt sockaddr, whatever dude
        addrOut[0] = 0xff;
        addrOut[1] = 0xfc;
        int len = sockaddr->addrLen;
        Bits_memcpy(&addrOut[16-len], &sockaddr, len);
    }
    struct Sockaddr_pvt* sa = (struct Sockaddr_pvt*) sockaddr;
    Bits_memset(addrOut, 0, 16);
    switch (sa->ss.ss_family) {
        case AF_INET: {
            // IPv4 in 6
            addrOut[10] = 0xff;
            addrOut[11] = 0xff;
            Bits_memcpy(&addrOut[12], &((struct sockaddr_in*)&sa->ss)->sin_addr, 4);
            break;
        }
        case AF_INET6: {
            // Normal IPv6
            Bits_memcpy(addrOut, &((struct sockaddr_in6*)&sa->ss)->sin6_addr, 16);
            break;
        }
        default: {
            uint16_t len = sa->pub.addrLen - Sockaddr_OVERHEAD;
            if (len <= 14) {
                addrOut[0] = 0xff;
                addrOut[1] = 0xfe;
                Bits_memcpy(&addrOut[16-len], &sa->ss, len);
            } else {
                uint8_t hash[32];
                crypto_hash_sha256(hash, (uint8_t*) &sa->ss, len);
                addrOut[0] = 0xff;
                addrOut[1] = 0xff;
                Bits_memcpy(&addrOut[2], hash, 14);
            }
        }
    }
}
void Sockaddr_asIp6_fromRust(uint8_t addrOut[static 16], const struct Sockaddr* sockaddr)
{
    Sockaddr_asIp6(addrOut, sockaddr);
}

int Sockaddr_compare(const struct Sockaddr* a, const struct Sockaddr* b)
{
    return Bits_memcmp(a, b, a->addrLen);
}

uint32_t Sockaddr_addrHandle(const struct Sockaddr* addr)
{
    if (addr->addrLen != sizeof(struct Sockaddr) || addr->type != Sockaddr_HANDLE) {
        return Sockaddr_addrHandle_INVALID;
    }
    uint32_t handle;
    Bits_memcpy(&handle, &((uint8_t*)addr)[4], 4);
    return handle;
}
uint32_t Sockaddr_addrHandle_fromRust(const Sockaddr_t* addr)
{
    return Sockaddr_addrHandle(addr);
}

void Sockaddr_addrFromHandle(struct Sockaddr* addr, uint32_t handle)
{
    Assert_true(handle != Sockaddr_addrHandle_INVALID);
    Bits_memset(addr, 0, sizeof(struct Sockaddr));
    addr->type = Sockaddr_HANDLE;
    addr->addrLen = sizeof(struct Sockaddr);
    Bits_memcpy(&((uint8_t*)addr)[4], &handle, 4);
}
