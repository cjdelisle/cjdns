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
#include "util/platform/netdev/NetPlatform.h"
#include "util/platform/Sockaddr.h"
#include "memory/Allocator.h"
#include "exception/Except.h"
#include "wire/Message.h"
#include "util/AddrTools.h"
#include "util/Assert.h"
#include "util/CString.h"

#include <stdbool.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <arpa/inet.h>
#include <linux/if.h>
#include <linux/route.h>
#if ! defined(Cjdns_android)
    #include <linux/ipv6_route.h>
#endif
#include <linux/netlink.h>
#include <linux/rtnetlink.h>
#include <net/if.h>

// Way to identify our routes as opposed to statically created or otherwise...
#define RTPROT_CJDNS 52

/**
 * This hack exists because linux/in.h and linux/in6.h define
 * the same structures, leading to redefinition errors.
 * For the second operand, we're grateful to android/bionic, platform level 21.
 */
struct Cjdns_in6_ifreq
{
    struct in6_addr ifr6_addr;
    uint32_t ifr6_prefixlen;
    int ifr6_ifindex;
};

/**
 * Get a socket and ifRequest for a given interface by name.
 *
 * @param interfaceName the name of the interface, eg: tun0
 * @param af either AF_INET or AF_INET6
 * @param eg an exception handler in case something goes wrong.
 *           this will send a -1 for all errors.
 * @param ifRequestOut an ifreq which will be populated with the interface index of the interface.
 * @return a socket for interacting with this interface.
 */
static Er_DEFUN(int socketForIfName(const char* interfaceName,
                           int af,
                           struct Allocator* alloc,
                           struct ifreq* ifRequestOut))
{
    int s;

    if ((s = socket(af, SOCK_DGRAM, 0)) < 0) {
        Er_raise(alloc, "socket() [%s]", strerror(errno));
    }

    memset(ifRequestOut, 0, sizeof(struct ifreq));
    CString_safeStrncpy(ifRequestOut->ifr_name, interfaceName, IFNAMSIZ);

    if (ioctl(s, SIOCGIFINDEX, ifRequestOut) < 0) {
        int err = errno;
        close(s);
        Er_raise(alloc, "ioctl(SIOCGIFINDEX) [%s]", strerror(err));
    }
    Er_ret(s);
}

/** don't use if_nametoindex() because it accesses the filesystem. */
static Er_DEFUN(int ifIndexForName(const char* interfaceName, struct Allocator* alloc))
{
    struct ifreq ifRequest;
    int s = Er(socketForIfName(interfaceName, AF_INET, alloc, &ifRequest));
    close(s);
    Er_ret(ifRequest.ifr_ifindex);
}

static Er_DEFUN(void checkInterfaceUp(int socket,
                             struct ifreq* ifRequest,
                             struct Log* logger,
                             struct Allocator* alloc))
{
    if (ioctl(socket, SIOCGIFFLAGS, ifRequest) < 0) {
        int err = errno;
        close(socket);
        Er_raise(alloc, "ioctl(SIOCGIFFLAGS) [%s]", strerror(err));
    }

    if (ifRequest->ifr_flags & IFF_UP & IFF_RUNNING) {
        // already up.
        Er_ret();
    }

    Log_info(logger, "Bringing up interface [%s]", ifRequest->ifr_name);

    ifRequest->ifr_flags |= IFF_UP | IFF_RUNNING;
    if (ioctl(socket, SIOCSIFFLAGS, ifRequest) < 0) {
        int err = errno;
        close(socket);
        Er_raise(alloc, "ioctl(SIOCSIFFLAGS) [%s]", strerror(err));
    }
    Er_ret();
}

Er_DEFUN(void NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Allocator* tempAlloc))
{
    struct ifreq ifRequest;
    int s = Er(socketForIfName(interfaceName, addrFam, tempAlloc, &ifRequest));
    int ifIndex = ifRequest.ifr_ifindex;

    // checkInterfaceUp() clobbers the ifindex.
    Er(checkInterfaceUp(s, &ifRequest, logger, tempAlloc));

    if (addrFam == AF_INET6) {
        struct Cjdns_in6_ifreq ifr6 = {
            .ifr6_ifindex = ifIndex,
            .ifr6_prefixlen = prefixLen
        };
        memcpy(&ifr6.ifr6_addr, address, 16);

        if (ioctl(s, SIOCSIFADDR, &ifr6) < 0) {
            int err = errno;
            close(s);
            if (err == EPERM) {
                Er_raise(tempAlloc, "ioctl permission denied, Are you root and is ipv6 enabled?");
            } else {
                Er_raise(tempAlloc, "ioctl(SIOCSIFADDR) failed: [%s]", strerror(err));
            }
        }


    } else if (addrFam == AF_INET) {
        struct sockaddr_in sin = { .sin_family = AF_INET, .sin_port = 0 };
        memcpy(&sin.sin_addr.s_addr, address, 4);
        memcpy(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr));

        if (ioctl(s, SIOCSIFADDR, &ifRequest) < 0) {
            int err = errno;
            close(s);
            Er_raise(tempAlloc, "ioctl(SIOCSIFADDR) failed: [%s]", strerror(err));
        }

        uint32_t x = (uint32_t)~0 << (32 - prefixLen);
        x = Endian_hostToBigEndian32(x);
        memcpy(&sin.sin_addr, &x, 4);
        memcpy(&ifRequest.ifr_addr, &sin, sizeof(struct sockaddr_in));

        if (ioctl(s, SIOCSIFNETMASK, &ifRequest) < 0) {
            int err = errno;
            close(s);
            Er_raise(tempAlloc, "ioctl(SIOCSIFNETMASK) failed: [%s]", strerror(err));
        }
    } else {
        Assert_true(0);
    }

    close(s);
    Er_ret();
}

Er_DEFUN(void NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Allocator* alloc))
{
    struct ifreq ifRequest;
    int s = Er(socketForIfName(interfaceName, AF_INET6, alloc, &ifRequest));

    Log_info(logger, "Setting MTU for device [%s] to [%u] bytes.", interfaceName, mtu);

    ifRequest.ifr_mtu = mtu;
    if (ioctl(s, SIOCSIFMTU, &ifRequest) < 0) {
        int err = errno;
        close(s);
        Er_raise(alloc, "ioctl(SIOCSIFMTU) [%s]", strerror(err));
    }

    close(s);
    Er_ret();
}

struct IfIndexAttr {
    struct rtattr rta;
    int ifIndex;
};
Assert_compileTime(sizeof(struct IfIndexAttr) == 8);

struct RouteRequest {
    struct nlmsghdr hdr;
    struct rtmsg route;
};
Assert_compileTime(sizeof(struct nlmsghdr) == 16);
Assert_compileTime(sizeof(struct rtmsg) == 12);
Assert_compileTime(sizeof(struct RouteRequest) == 28);

struct RouteInfo;
struct RouteInfo {
    struct RouteInfo* next;
    int protocol;
    int prefix;
    uint8_t dstAddr[16];
    int ifIndex;
    int af;
};

#define BUFF_SZ 16384

static Er_DEFUN(bool getMoreMessages(struct RouteInfo** rio,
                            int sock,
                            int ifIndex,
                            struct Allocator* alloc))
{
    bool retVal = false;
    struct Allocator* tempAlloc = Allocator_child(alloc);
    struct Message* msg = Message_new(BUFF_SZ, 0, tempAlloc);
    ssize_t sz = recv(sock, msg->msgbytes, BUFF_SZ, MSG_TRUNC);
    if (sz < (ssize_t)sizeof(struct nlmsghdr)) {
        Er_raise(tempAlloc, "recv() -> %s", strerror(errno));
    } else if (sz > BUFF_SZ) {
        Er_raise(tempAlloc, "recv() -> buffer too small");
    }

    Er_assert(Message_truncate(msg, sz));
    //printf("%s\n", Hex_print(msg->bytes, Message_getLength(msg), tempAlloc));

    while (Message_getLength(msg)) {
        struct RouteInfo ri = { .protocol = 0 };
        int initMsgLen = Message_getLength(msg);
        struct nlmsghdr hdr;
        Er(Message_epop(msg, &hdr, sizeof(struct nlmsghdr)));
        //printf("\nHEADER %04x %04x\n", hdr.nlmsg_type, hdr.nlmsg_flags);
        if (hdr.nlmsg_flags & NLM_F_MULTI) { retVal = true; }
        if (hdr.nlmsg_type == NLMSG_DONE) {
            Allocator_free(tempAlloc);
            Er_ret(false);
        }
        struct rtmsg rtm;
        Er(Message_epop(msg, &rtm, sizeof(struct rtmsg)));
        ri.prefix = rtm.rtm_dst_len;
        ri.af = rtm.rtm_family;
        ri.protocol = rtm.rtm_protocol;
        for (;;) {
            int remainingLen = hdr.nlmsg_len - (initMsgLen - Message_getLength(msg));
            if (remainingLen <= (int)sizeof(struct rtattr)) { break; }
            struct rtattr attrHead;
            //printf(">%s %d\n", Hex_print(msg->bytes, Message_getLength(msg), tempAlloc), remainingLen);
            Er(Message_epop(msg, &attrHead, sizeof(struct rtattr)));
            switch (attrHead.rta_type) {
                case RTA_OIF: {
                    if (attrHead.rta_len != 8) {
                        Er_raise(alloc, "unexpected rta_len for ifIndex");
                    }
                    Er(Message_epop(msg, &ri.ifIndex, 4));
                    break;
                }
                case RTA_DST: {
                    if (rtm.rtm_family == AF_INET6) {
                        if (attrHead.rta_len != 20) {
                            Er_raise(alloc, "unexpected rta_len for RTA_DST (ipv6)");
                        }
                        Er(Message_epop(msg, ri.dstAddr, 16));
                    } else if (rtm.rtm_family == AF_INET) {
                        if (attrHead.rta_len != 8) {
                            Er_raise(alloc, "unexpected rta_len for RTA_DST (ipv4)");
                        }
                        Er(Message_epop(msg, ri.dstAddr, 4));
                    } else {
                        Er_raise(alloc, "unexpected af %d", rtm.rtm_family);
                    }
                    break;
                }
                default: {
                    int effectiveLen = RTA_ALIGN(attrHead.rta_len);
                    //printf("unrecognized %d (length %d)\n", attrHead.rta_type, effectiveLen);
                    Er(Message_epop(msg, NULL, effectiveLen - sizeof(struct rtattr)));
                    break;
                }
            }
        }

        if (rtm.rtm_table != RT_TABLE_MAIN) { continue; }
        if (rtm.rtm_type != RTN_UNICAST) { continue; }

        if (ri.ifIndex != ifIndex) { continue; }
        if (ri.protocol != RTPROT_CJDNS) { continue; }
        struct RouteInfo* outRi = Allocator_clone(alloc, &ri);
        outRi->next = *rio;
        *rio = outRi;
    }
    Allocator_free(tempAlloc);
    Er_ret(retVal);
}

static Er_DEFUN(struct RouteInfo* getRoutes(int sock,
                                   int ifIndex,
                                   struct Allocator* alloc))
{
    struct RouteRequest req = {
        .hdr = {
            .nlmsg_len = sizeof(struct RouteRequest),
            .nlmsg_type = RTM_GETROUTE,
            .nlmsg_flags = NLM_F_REQUEST | NLM_F_ROOT | NLM_F_MULTI
        },
        .route = {
            .rtm_family = AF_UNSPEC
        }
    };
    ssize_t sz = send(sock, &req, req.hdr.nlmsg_len, 0);
    if (sz < 0) {
        Er_raise(alloc, "send() -> %s", strerror(errno));
    }
    struct RouteInfo* ri = NULL;
    while (Er(getMoreMessages(&ri, sock, ifIndex, alloc))) ;
    Er_ret(ri);
}

static void bitShave(uint8_t* address, int prefix, int af)
{
    int top;
    if (af == AF_INET) {
        top = 4;
    } else if (af == AF_INET6) {
        top = 16;
    } else {
        Assert_failure("bad af");
    }
    if (prefix < (8 * top)) {
        address[prefix >> 3] &= ( 0xff << (8 - (prefix % 8)) );
        for (int i = (prefix >> 3) + 1; i < top; i++) {
            address[i] = 0;
        }
    }
}

static Er_DEFUN(void addDeleteRoutes(int sock,
                            bool delete,
                            struct RouteInfo* ri,
                            struct Allocator* tempAlloc))
{
    struct Message* msg = Message_new(0, 512, tempAlloc);
    for (;ri;ri = ri->next) {
        struct IfIndexAttr ifa = {
            .rta = {
                .rta_len = sizeof(struct IfIndexAttr),
                .rta_type = RTA_OIF
            },
            .ifIndex = ri->ifIndex
        };
        Er(Message_epush(msg, &ifa, sizeof(struct IfIndexAttr)));
        int addrLen = (ri->af == AF_INET6) ? 16 : 4;
        Er(Message_epush(msg, ri->dstAddr, addrLen));
        bitShave(msg->msgbytes, ri->prefix, ri->af);
        struct rtattr rta = { .rta_len = sizeof(struct rtattr) + addrLen, .rta_type = RTA_DST };
        Er(Message_epush(msg, &rta, sizeof(struct rtattr)));
        struct rtmsg route = {
            .rtm_family = ri->af,
            .rtm_dst_len = ri->prefix,
            .rtm_table = RT_TABLE_MAIN,
            .rtm_scope = (delete) ? RT_SCOPE_NOWHERE : RT_SCOPE_LINK,
            .rtm_protocol = (delete) ? RTPROT_UNSPEC : ri->protocol,
            .rtm_type = (delete) ? RTN_UNSPEC : RTN_UNICAST
        };
        Er(Message_epush(msg, &route, sizeof(struct rtmsg)));
        struct nlmsghdr hdr = {
            .nlmsg_len = Message_getLength(msg) + sizeof(struct nlmsghdr),
            .nlmsg_type = (delete) ? RTM_DELROUTE : RTM_NEWROUTE,
            .nlmsg_flags = NLM_F_REQUEST | ((delete) ? 0 : NLM_F_CREATE) // | NLM_F_ACK,
        };
        Er(Message_epush(msg, &hdr, sizeof(struct nlmsghdr)));
        ssize_t sz = send(sock, msg->msgbytes, Message_getLength(msg), 0);
        if (sz < 0) {
            Er_raise(tempAlloc, "send() -> %s", strerror(errno));
        }
        Message_reset(msg);
    }
    Er_ret();
}

static int closeSocket(struct Allocator_OnFreeJob* job)
{
    long sock = (long) job->userData;
    close((int)sock);
    return 0;
}

static Er_DEFUN(int mkSocket(struct Allocator* alloc))
{
    int sock = socket(PF_NETLINK, SOCK_RAW, NETLINK_ROUTE);
    if (sock < 0) {
        Er_raise(alloc, "socket(PF_NETLINK) -> %s", strerror(errno));
    }
    Allocator_onFree(alloc, closeSocket, (void*) ((long) sock));
    Er_ret(sock);
}

static struct RouteInfo* riForSockaddrs(struct Sockaddr** prefixSet,
                                        int prefixCount,
                                        int ifIndex,
                                        struct Allocator* alloc)
{
    struct RouteInfo* out = NULL;
    for (int i = 0; i < prefixCount; i++) {
        struct RouteInfo* ri = Allocator_calloc(alloc, sizeof(struct RouteInfo), 1);
        ri->protocol = RTPROT_CJDNS;
        ri->prefix = Sockaddr_getPrefix(prefixSet[i]);
        ri->af = Sockaddr_getFamily(prefixSet[i]);
        ri->ifIndex = ifIndex;
        uint8_t* addr;
        int len = Sockaddr_getAddress(prefixSet[i], &addr);
        Assert_true(len == 4 || len == 16);
        Bits_memcpy(ri->dstAddr, addr, len);
        ri->next = out;
        out = ri;
    }
    return out;
}

static void logRis(struct RouteInfo* ri, struct Log* logger, char* msg)
{
    for (; ri; ri = ri->next) {
        uint8_t addrBuff[40] = {0};
        if (ri->af == AF_INET6) {
            AddrTools_printIp(addrBuff, ri->dstAddr);
        } else if (ri->af == AF_INET) {
            snprintf(addrBuff, 40, "%u.%u.%u.%u",
                ri->dstAddr[0], ri->dstAddr[1], ri->dstAddr[2], ri->dstAddr[3]);
        }
        Log_debug(logger, "%s %s/%u", msg, addrBuff, ri->prefix);
    }
}

Er_DEFUN(void NetPlatform_setRoutes(const char* ifName,
                           struct Sockaddr** prefixSet,
                           int prefixCount,
                           struct Log* logger,
                           struct Allocator* tempAlloc))
{
    int ifIndex = Er(ifIndexForName(ifName, tempAlloc));
    struct RouteInfo* newRi = riForSockaddrs(prefixSet, prefixCount, ifIndex, tempAlloc);
    int sock = Er(mkSocket(tempAlloc));
    struct RouteInfo* oldRi = Er(getRoutes(sock, ifIndex, tempAlloc));
    logRis(oldRi, logger, "DELETE ROUTE");
    Er(addDeleteRoutes(sock, true, oldRi, tempAlloc));
    logRis(newRi, logger, "ADD ROUTE");
    Er(addDeleteRoutes(sock, false, newRi, tempAlloc));
    Er_ret();
}
