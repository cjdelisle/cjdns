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
#include "exception/Er.h"
#include "util/platform/netdev/NetPlatform.h"
#include "util/AddrTools.h"
#include "util/platform/Sockaddr.h"
#include "util/Assert.h"
#include "util/Bits.h"
#include "util/CString.h"
#include "wire/Message.h"

#include <stdbool.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <net/if_dl.h>
#include <net/if_types.h>
#include <string.h>
#include <netdb.h>
#include <net/if_var.h>
#include <netinet/in_var.h>
#include <netinet6/nd6.h>
#include <netinet/in.h>
#include <sys/kern_control.h>
#include <sys/sys_domain.h>
#include <sys/kern_event.h>
#include <net/route.h>
#include <sys/sysctl.h>

struct Prefix {
    struct sockaddr_storage ss;
    int prefix;
};

#define ArrayList_TYPE struct Prefix
#define ArrayList_NAME OfPrefix
#include "util/ArrayList.h"

Assert_compileTime(sizeof(struct in_addr) == 4);

struct RouteMessage4 {
    struct rt_msghdr header;
    struct sockaddr_in dest;
    struct sockaddr_dl link;
    struct sockaddr_in netmask;
};
Assert_compileTime(sizeof(struct rt_msghdr) == 92);
Assert_compileTime(sizeof(struct sockaddr_in) == 16);
Assert_compileTime(sizeof(struct sockaddr_dl) == 20);
Assert_compileTime(sizeof(struct RouteMessage4) == 144);

static Er_DEFUN(void mkRouteMsg(Message_t* msg,
                       struct Prefix* addRoute,
                       int ifIndex,
                       const char* ifName,
                       int seq,
                       bool delete))
{
    if (CString_strlen(ifName) >= 12) {
        Er_raise(Message_getAlloc(msg), "ifName [%s] too long, limit 11 chars", ifName);
    }
    int lengthBegin = Message_getLength(msg);
    bool ipv6 = addRoute->ss.ss_family == AF_INET6;
    if (ipv6) {
        struct sockaddr_in6 mask = {
            .sin6_family = AF_INET6,
            .sin6_len = sizeof(struct sockaddr_in6)
        };
        Bits_memset((void *)&mask.sin6_addr, 0xff, addRoute->prefix >> 3);
        ((uint8_t*)&mask.sin6_addr)[addRoute->prefix >> 3] = 0xff << (8 - (addRoute->prefix % 8));
        Er(Er_fromErr(Message_epush(msg, &mask, sizeof(struct sockaddr_in6))));
    } else {
        struct sockaddr_in mask = {
            .sin_family = AF_INET,
            .sin_len = sizeof(struct sockaddr_in)
        };
        mask.sin_addr.s_addr = Endian_hostToBigEndian32(~0u << (32 - addRoute->prefix));
        Er(Er_fromErr(Message_epush(msg, &mask, sizeof(struct sockaddr_in))));
    }
    if (!delete) {
        struct sockaddr_dl link = {
            .sdl_family = AF_LINK,
            .sdl_len = sizeof(struct sockaddr_dl),
            .sdl_type = IFT_OTHER,
            .sdl_index = ifIndex,
            .sdl_nlen = CString_strlen(ifName)
        };
        CString_safeStrncpy(link.sdl_data, ifName, 12);
        Er(Er_fromErr(Message_epush(msg, &link, sizeof(struct sockaddr_dl))));
    }
    int len = (ipv6) ? sizeof(struct sockaddr_in6) : sizeof(struct sockaddr_in);
    Assert_true(addRoute->ss.ss_len == len);
    Er(Er_fromErr(Message_epush(msg, &addRoute->ss, len)));
    struct rt_msghdr hdr = {
        .rtm_type = (delete) ? RTM_DELETE : RTM_ADD,
        .rtm_flags = RTF_UP | RTF_STATIC,
        .rtm_version = RTM_VERSION,
        .rtm_seq = seq,
        .rtm_pid = getpid(),
        .rtm_addrs = RTA_DST | RTA_NETMASK | ((delete) ? 0 : RTA_GATEWAY),
        .rtm_msglen = sizeof(struct rt_msghdr) + (Message_getLength(msg) - lengthBegin)
    };
    Er(Er_fromErr(Message_epush(msg, &hdr, sizeof(struct rt_msghdr))));
    Er_ret();
}

static Er_DEFUN(void setRoutes(uint32_t ifIndex,
                      const char* ifName,
                      struct ArrayList_OfPrefix* toRemove,
                      struct ArrayList_OfPrefix* toAdd,
                      struct Log* logger,
                      struct Allocator* alloc))
{
    int seq = 0;
    int sock = socket(PF_ROUTE, SOCK_RAW, 0);
    if (sock == -1) {
        int err = errno;
        close(sock);
        Er_raise(alloc, "open route socket [%s]", strerror(err));
    }
    bool err = false;
    ssize_t returnLen = 0;

    for (int i = 0; !err && i < toRemove->length; i++) {
        struct Prefix* pfx = ArrayList_OfPrefix_get(toRemove, i);
        Message_t* msg = Message_new(0, 1024, alloc);
        Er(mkRouteMsg(msg, pfx, ifIndex, ifName, seq++, true));
        //printf("DELETE ROUTE %s\n", Hex_print(msg->bytes, Message_getLength(msg), alloc));
        returnLen = write(sock, Message_bytes(msg), Message_getLength(msg));
        if (returnLen < Message_getLength(msg)) { err = true; break; }
    }
    for (int i = 0; !err && i < toAdd->length; i++) {
        struct Prefix* pfx = ArrayList_OfPrefix_get(toAdd, i);
        Message_t* msg = Message_new(0, 1024, alloc);
        Er(mkRouteMsg(msg, pfx, ifIndex, ifName, seq++, false));
        //printf("ADD ROUTE %s\n", Hex_print(msg->bytes, Message_getLength(msg), alloc));
        returnLen = write(sock, Message_bytes(msg), Message_getLength(msg));
        if (returnLen < Message_getLength(msg)) { err = true; break; }
    }

    if (returnLen < 0) {
        int error = errno;
        close(sock);
        Er_raise(alloc, "setRoutes() [%s]", strerror(error));
    } else if (err) {
        close(sock);
        Er_raise(alloc, "setRoutes() returned short");
    }
    close(sock);
    Er_ret();
}

static int prefixFromWeirdBSDMask(uint8_t* weirdBsdMask, bool ipv6)
{
    uint8_t len = weirdBsdMask[0];
    int out;
    if (ipv6) {
        Assert_true(len >= 8);
        out = (len - 9) * 8;
    } else {
        Assert_true(len >= 4);
        out = (len - 5) * 8;
    }
    for (int i = 1; i < len - 1; i++) {
        Assert_true(weirdBsdMask[i] == 0xff);
    }
    return out + Bits_popCountx32(weirdBsdMask[len - 1]);
}

static Er_DEFUN(struct ArrayList_OfPrefix* getRoutes(uint32_t ifIndex,
                                              struct Log* logger,
                                              struct Allocator* allocator))
{
    size_t needed;
    int mib[] = { CTL_NET, PF_ROUTE, 0, 0, NET_RT_DUMP, 0 };
    if (sysctl(mib, 6, NULL, &needed, NULL, 0) < 0) {
        Er_raise(allocator, "sysctl(net.route.0.0.dump) estimate");
    }
    struct Allocator* tempAlloc = Allocator_child(allocator);
    uint8_t* buf = Allocator_malloc(tempAlloc, needed);
    if (sysctl(mib, 6, buf, &needed, NULL, 0) < 0) {
        Er_raise(allocator, "sysctl(net.route.0.0.dump)");
    }
    struct ArrayList_OfPrefix* addrList = ArrayList_OfPrefix_new(allocator);
    for (int i = 0; i < (int)needed;) {
        struct rt_msghdr* rtm = (struct rt_msghdr*) (&buf[i]);
        i += rtm->rtm_msglen;
        struct sockaddr* sa1 = (struct sockaddr*)(rtm + 1);
        struct sockaddr* sa2 = (struct sockaddr*)(sa1->sa_len + (char *)sa1);
        uint8_t* mask = (uint8_t*) (sa2->sa_len + (char*)sa2);
        Assert_true(&buf[i] >= mask);
        if (sa2->sa_family != AF_LINK) { continue; }
        if (rtm->rtm_flags & RTF_LOCAL) { continue; }
        if (!(rtm->rtm_flags & RTF_STATIC)) { continue; }
        if (((struct sockaddr_dl*) sa2)->sdl_index != ifIndex) { continue; }
        bool ipv6 = (sa1->sa_family == AF_INET6);
        if (!ipv6 && (sa1->sa_family != AF_INET)) { continue; }
        // printf("GOT ROUTE %s\n", Hex_print(rtm, rtm->rtm_msglen, tempAlloc));
        int prefix;
        if (&buf[i] < &mask[mask[0]] || mask[0] == 0) {
            //Assert_true(rtm->rtm_flags & RTF_HOST);
            prefix = (ipv6) ? 128 : 32;
        } else {
            prefix = prefixFromWeirdBSDMask(mask, ipv6);
        }
        struct Prefix* pfx = Allocator_calloc(allocator, sizeof(struct Prefix), 1);
        Assert_true(sizeof(pfx->ss) >= sa1->sa_len);
        Bits_memcpy(&pfx->ss, sa1, sa1->sa_len);
        pfx->prefix = prefix;
        ArrayList_OfPrefix_add(addrList, pfx);
    }
    Allocator_free(tempAlloc);
    Er_ret(addrList);
}

static Err_DEFUN addIp4Address(const char* interfaceName,
                          const uint8_t address[4],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* tempAlloc)
{
    struct ifaliasreq ifarted;
    Bits_memset(&ifarted, 0, sizeof(struct ifaliasreq));
    CString_safeStrncpy(ifarted.ifra_name, interfaceName, IFNAMSIZ);

    struct sockaddr_in sin = { .sin_family = AF_INET, .sin_len = sizeof(struct sockaddr_in) };
    Bits_memcpy(&sin.sin_addr.s_addr, address, 4);
    Bits_memcpy(&ifarted.ifra_addr, &sin, sizeof(struct sockaddr_in));
    sin.sin_addr.s_addr = Endian_hostToBigEndian32(~0u << (32 - prefixLen));
    Bits_memcpy(&ifarted.ifra_mask, &sin, sizeof(struct sockaddr_in));

    int s = socket(AF_INET, SOCK_DGRAM, 0);
    if (s < 0) {
        Err_raise(tempAlloc, "socket() [%s]", strerror(errno));
    }

    // will probably fail, ignore result.
    struct ifreq ifr = { .ifr_flags = 0 };
    CString_safeStrncpy(ifr.ifr_name, interfaceName, IFNAMSIZ);
    ioctl(s, SIOCDIFADDR, &ifr);

    if (ioctl(s, SIOCSIFADDR, &ifarted) < 0) {
        int err = errno;
        close(s);
        Err_raise(tempAlloc, "ioctl(SIOCSIFADDR) [%s]", strerror(err));
    }

    //setupRoute4(address, prefixLen, interfaceName, logger, tempAlloc, eh);

    Log_info(logger, "Configured IPv4 [%u.%u.%u.%u/%i] for [%s]",
        address[0], address[1], address[2], address[3], prefixLen, interfaceName);

    close(s);
    return NULL;
}

static Err_DEFUN addIp6Address(const char* interfaceName,
                          const uint8_t address[16],
                          int prefixLen,
                          struct Log* logger,
                          struct Allocator* errAlloc)
{
    /* stringify our IP address */
    char myIp[40];
    AddrTools_printIp((uint8_t*)myIp, address);

    /* set up the interface ip assignment request */
    struct in6_aliasreq in6_addreq;
    memset(&in6_addreq, 0, sizeof(in6_addreq));
    in6_addreq.ifra_lifetime.ia6t_vltime = ND6_INFINITE_LIFETIME;
    in6_addreq.ifra_lifetime.ia6t_pltime = ND6_INFINITE_LIFETIME;

    /* parse the IPv6 address and add it to the request */
    struct addrinfo hints, *result;

    bzero(&hints, sizeof(struct addrinfo));
    hints.ai_family = AF_INET6;
    int err = getaddrinfo((const char *)myIp, NULL, &hints, &result);
    if (err) {
        // Should never happen since the address is specified as binary.
        Err_raise(errAlloc, "bad IPv6 address [%s]", gai_strerror(err));
    }

    bcopy(result->ai_addr, &in6_addreq.ifra_addr, result->ai_addrlen);

    /* turn the prefixlen into a mask, and add it to the request */
    struct sockaddr_in6* mask = &in6_addreq.ifra_prefixmask;

    mask->sin6_len = sizeof(*mask);
    if (prefixLen >= 128 || prefixLen <= 0) {
        memset(&mask->sin6_addr, 0xff, sizeof(struct in6_addr));
    } else {
        memset((void *)&mask->sin6_addr, 0x00, sizeof(mask->sin6_addr));
        memset((void *)&mask->sin6_addr, 0xff, prefixLen>>3);
        ((uint8_t*)&mask->sin6_addr)[prefixLen>>3] = 0xff << (8 - (prefixLen%8));
    }

    CString_safeStrncpy(in6_addreq.ifra_name, interfaceName, sizeof(in6_addreq.ifra_name));

    /* do the actual assignment ioctl */
    int s = socket(AF_INET6, SOCK_DGRAM, 0);
    if (s < 0) {
        Err_raise(errAlloc, "socket() [%s]", strerror(errno));
    }

    if (ioctl(s, SIOCAIFADDR_IN6, &in6_addreq) < 0) {
        int err = errno;
        close(s);
        Err_raise(errAlloc, "ioctl(SIOCAIFADDR) [%s] for [%s]", strerror(err), interfaceName);
    }

    Log_info(logger, "Configured IPv6 [%s/%i] for [%s]", myIp, prefixLen, interfaceName);

    close(s);
    return NULL;
}

Err_DEFUN NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Allocator* tempAlloc)
{
    if (addrFam == Sockaddr_AF_INET6) {
        return addIp6Address(interfaceName, address, prefixLen, logger, tempAlloc);
    } else if (addrFam == Sockaddr_AF_INET) {
        return addIp4Address(interfaceName, address, prefixLen, logger, tempAlloc);
    } else {
        Err_raise(tempAlloc, "Invalid address family [%d]", addrFam);
    }
}

Err_DEFUN NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Allocator* errAlloc)
{
    int s = socket(AF_INET6, SOCK_DGRAM, 0);

    if (s < 0) {
        Err_raise(errAlloc, "socket() [%s]", strerror(errno));
    }

    struct ifreq ifRequest;

    CString_safeStrncpy(ifRequest.ifr_name, interfaceName, IFNAMSIZ);
    ifRequest.ifr_mtu = mtu;

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
       int err = errno;
       close(s);
       Err_raise(errAlloc, "ioctl(SIOCSIFMTU) [%s]", strerror(err));
    }
    close(s);
    return NULL;
}

Er_DEFUN(void NetPlatform_setRoutes(const char* ifName,
                           struct Sockaddr** prefixSet,
                           int prefixCount,
                           struct Log* logger,
                           struct Allocator* tempAlloc))
{
    struct ArrayList_OfPrefix* newRoutes = ArrayList_OfPrefix_new(tempAlloc);
    for (int i = 0; i < prefixCount; i++) {
        struct Prefix* pfx = Allocator_calloc(tempAlloc, sizeof(struct Prefix), 1);
        //Sockaddr_getAddress

        int addrFam = Sockaddr_getFamily(prefixSet[i]);
        if (addrFam == Sockaddr_AF_INET) {
            // OK
            struct sockaddr_in* in = (struct sockaddr_in*) &pfx->ss;
            in->sin_family = AF_INET;
            Assert_compileTime(sizeof(in->sin_addr) == 4);
            Assert_true(Sockaddr_getAddress(prefixSet[i], &in->sin_addr) == AF_INET);
        } else if (addrFam == Sockaddr_AF_INET6) {
            // OK
            struct sockaddr_in6* in6 = (struct sockaddr_in6*) &pfx->ss;
            in6->sin6_family = AF_INET6;
            Assert_compileTime(sizeof(in6->sin6_addr) == 16);
            Assert_true(Sockaddr_getAddress(prefixSet[i], &in6->sin6_addr) == AF_INET6);
        } else {
            Er_raise(tempAlloc, "Unrecognized address type %d", addrFam);
        }
        ArrayList_OfPrefix_add(newRoutes, prefixSet[i]);
    }
    uint32_t ifIndex = if_nametoindex(ifName);
    if (!ifIndex) {
        Er_raise(tempAlloc, "tunName not recognized");
    }
    struct ArrayList_OfPrefix* oldRoutes = Er(getRoutes(ifIndex, logger, tempAlloc));
    Er(setRoutes(ifIndex, ifName, oldRoutes, newRoutes, logger, tempAlloc));
    Er_ret();
}
