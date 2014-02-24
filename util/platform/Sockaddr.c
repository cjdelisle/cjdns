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
#include "util/events/libuv/UvWrapper.h"
#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/platform/Sockaddr.h"
#include "util/CString.h"
#include "util/Bits.h"
#include "util/Hex.h"

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

struct Sockaddr_pvt
{
    struct Sockaddr pub;
    struct sockaddr_storage ss;
};

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

struct Sockaddr* Sockaddr_fromNative(const void* ss, int addrLen, struct Allocator* alloc)
{
    struct Sockaddr_pvt* out = Allocator_malloc(alloc, addrLen + Sockaddr_OVERHEAD);
    Bits_memcpy(&out->ss, ss, addrLen);
    out->pub.addrLen = addrLen + Sockaddr_OVERHEAD;
    Sockaddr_normalizeNative(&out->ss);
    return &out->pub;
}

int Sockaddr_parse(const char* str, struct Sockaddr_storage* out)
{
    struct Sockaddr_storage unusedOut;
    if (!out) {
        out = &unusedOut;
    }
    Bits_memset(out, 0, sizeof(struct Sockaddr_storage));
    char* lastColon = CString_strrchr(str, ':');
    if (!lastColon || lastColon == CString_strchr(str, ':')) {
        // IPv4
        int port = 0;
        int addrLen;
        if (lastColon) {
            addrLen = (lastColon - str);
            port = atoi(lastColon+1);
            if (port > 65535 || port < 0) {
                return -1;
            }
        } else {
            addrLen = CString_strlen(str);
        }
        uint8_t addr[16] = {0};
        if (addrLen > 15 || addrLen < 7) {
            return -1;
        }
        Bits_memcpy(addr, str, addrLen);
        struct sockaddr_in* in = ((struct sockaddr_in*) Sockaddr_asNative(&out->addr));
        if (uv_inet_pton(AF_INET, (char*) addr, &in->sin_addr) != 0) {
            return -1;
        }
        out->addr.addrLen = sizeof(struct sockaddr_in) + Sockaddr_OVERHEAD;
        in->sin_port = Endian_hostToBigEndian16(port);
        in->sin_family = AF_INET;
    } else {
        // IPv6
        int port = 0;
        int addrLen;
        if (*str == '[') {
            str++;
            {
                char* endBracket = CString_strchr(str, ']');
                if (!endBracket) {
                    return -1;
                }
                addrLen = (endBracket - str);
            }
            if (str[addrLen+1] == ':') {
                port = atoi(&str[addrLen+2]);
                if (port > 65535 || port < 0) {
                    return -1;
                }
            }
        } else {
            addrLen = CString_strlen(str);
        }
        uint8_t addr[40] = {0};
        if (addrLen > 39 || addrLen < 2) {
            return -1;
        }
        Bits_memcpy(addr, str, addrLen);
        struct sockaddr_in6* in6 = (struct sockaddr_in6*) Sockaddr_asNative(&out->addr);
        int ret = uv_inet_pton(AF_INET6, (char*) addr, &in6->sin6_addr);
        if (ret != 0) {
            return -1;
        }
        out->addr.addrLen = sizeof(struct sockaddr_in6) + Sockaddr_OVERHEAD;
        in6->sin6_port = Endian_hostToBigEndian16(port);
        in6->sin6_family = AF_INET6;
    }
    return 0;
}

struct Sockaddr* Sockaddr_clone(const struct Sockaddr* addr, struct Allocator* alloc)
{
    return Sockaddr_fromNative(Sockaddr_asNativeConst(addr),
                               addr->addrLen - Sockaddr_OVERHEAD,
                               alloc);
}

char* Sockaddr_print(struct Sockaddr* sockaddr, struct Allocator* alloc)
{
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)
        || sockaddr->addrLen > Sockaddr_MAXSIZE + Sockaddr_OVERHEAD) {
        return "invalid";
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
            Hex_encode(buff, sizeof(buff), (uint8_t*)&addr->ss, sockaddr->addrLen);
            String* out = String_printf(alloc, "unknown (%s)", buff);
            return out->bytes;
        }
    };

    #define BUFF_SZ 64
    char printedAddr[BUFF_SZ] = {0};
    int ret = uv_inet_ntop(addr->ss.ss_family, inAddr, printedAddr, BUFF_SZ - 1);
    if (ret != 0) {
        return "invalid";
    }

    if (port) {
        int totalLength = CString_strlen(printedAddr) + CString_strlen("[]:65535") + 1;
        char* out = Allocator_malloc(alloc, totalLength);
        const char* format = (addr->ss.ss_family == AF_INET6) ? "[%s]:%u" : "%s:%u";
        snprintf(out, totalLength, format, printedAddr, port);
        return out;
    }

    int totalLength = CString_strlen(printedAddr) + 1;
    char* out = Allocator_calloc(alloc, totalLength, 1);
    Bits_memcpy(out, printedAddr, totalLength);
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

int Sockaddr_getPort(struct Sockaddr* sockaddr)
{
    uint16_t* pp = getPortPtr(sockaddr);
    return (pp) ? Endian_bigEndianToHost16(*pp) : -1;
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
int Sockaddr_getFamily(struct Sockaddr* sockaddr)
{
    if (sockaddr->addrLen < (2 + Sockaddr_OVERHEAD)) {
        return -1;
    }
    struct Sockaddr_pvt* sa = (struct Sockaddr_pvt*) sockaddr;
    return sa->ss.ss_family;
}

struct Sockaddr* Sockaddr_fromBytes(const uint8_t* bytes, int addrFamily, struct Allocator* alloc)
{
    struct sockaddr_storage ss;
    Bits_memset(&ss, 0, sizeof(struct sockaddr_storage));
    int addrLen;
    switch (addrFamily) {
        case AF_INET: {
            ss.ss_family = AF_INET;
            Bits_memcpyConst(&((struct sockaddr_in*)&ss)->sin_addr, bytes, 4);
            addrLen = sizeof(struct sockaddr_in);
            break;
        }
        case AF_INET6: {
            ss.ss_family = AF_INET6;
            Bits_memcpyConst(&((struct sockaddr_in6*)&ss)->sin6_addr, bytes, 16);
            addrLen = sizeof(struct sockaddr_in6);
            break;
        }
        default: return NULL;
    }

    struct Sockaddr_pvt* out = Allocator_malloc(alloc, addrLen + Sockaddr_OVERHEAD);
    Bits_memcpy(&out->ss, &ss, addrLen);
    out->pub.addrLen = addrLen + Sockaddr_OVERHEAD;
    return &out->pub;
}

void Sockaddr_normalizeNative(void* nativeSockaddr)
{
#if defined(freebsd) || defined(openbsd) || defined(darwin)
    ((struct sockaddr*)nativeSockaddr)->sa_len = 0;
#endif
}

struct Sockaddr* Sockaddr_fromName(char* name, struct Allocator* alloc)
{
    struct addrinfo* servinfo;
    if (getaddrinfo(name, 0, NULL, &servinfo) == 0) {
        struct Sockaddr* adr;
        adr = Sockaddr_fromNative(servinfo->ai_addr, servinfo->ai_addrlen, alloc);
        freeaddrinfo(servinfo);
        return adr;
    }
    return NULL;
}
