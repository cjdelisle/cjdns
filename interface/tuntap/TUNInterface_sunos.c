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
#include "interface/tuntap/TUNInterface.h"
#include "rust/cjdns_sys/RTypes.h"
#include "rust/cjdns_sys/Rffi.h"
#include "util/Identity.h"
#include "wire/Ethernet.h"

#include <errno.h>
#include <stdio.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stddef.h>
#include <net/if.h>
#include <ctype.h>
#include <sys/stropts.h>
#include <sys/sockio.h>
#include <fcntl.h>
#include <net/route.h>

/**
 * Since some illumos distributions (namely SmartOS) don't ship `net/if_tun.h`,
 * define those IOCTL constants here.
 */
#define TUNNEWPPA (('T'<<16) | 0x0001)
#define TUNSETPPA (('T'<<16) | 0x0002)

struct TUNInterface_Illumos_pvt
{
    struct Iface internalIf;
    struct Iface externalIf;
    Identity
};

/**
 * Illumos has no concept of packet info, it only supports IPv4 and IPv6
 * through TUN devices and it detects it by reading the version byte.
 */
static uint16_t ethertypeForPacketType(uint8_t highByte)
{
    return ((highByte >> 4) == 6) ? Ethernet_TYPE_IP6 : Ethernet_TYPE_IP4;
}

static Iface_DEFUN incomingFromWire(Message_t* message, struct Iface* externalIf)
{
    struct TUNInterface_Illumos_pvt* ctx =
        Identity_containerOf(externalIf, struct TUNInterface_Illumos_pvt, externalIf);

    if (Message_getLength(message) < 4) {
        return 0;
    }

    Err(Message_eshift(message, 4));
    ((uint16_t*) Message_bytes(message))[0] = 0;
    ((uint16_t*) Message_bytes(message))[1] = ethertypeForPacketType(Message_bytes(message)[4]);

    return Iface_next(&ctx->internalIf, message);
}

static Iface_DEFUN incomingFromUs(Message_t* message, struct Iface* internalIf)
{
    struct TUNInterface_Illumos_pvt* ctx =
        Identity_containerOf(internalIf, struct TUNInterface_Illumos_pvt, internalIf);

    Err(Message_eshift(message, -4));
    uint16_t ethertype = ((uint16_t*) Message_bytes(message))[-1];
    if (ethertype != Ethernet_TYPE_IP6 && ethertype != Ethernet_TYPE_IP4) {
        Assert_true(!"Unsupported ethertype");
    }

    return Iface_next(&ctx->externalIf, message);
}

Err_DEFUN TUNInterface_newImpl(
    Rffi_SocketIface_t** sout,
    struct Iface** out,
    const char* interfaceName,
    char assignedInterfaceName[TUNInterface_IFNAMSIZ],
    struct Log* logger,
    struct Allocator* alloc)
{
    // Extract the number eg: 0 from tun0
    int ppa = 0;
    if (interfaceName) {
        for (uint32_t i = 0; i < strlen(interfaceName); i++) {
            if (isdigit(interfaceName[i])) {
                ppa = atoi(interfaceName);
            }
        }
    }

    // Open the descriptor
    int tunFd = open("/dev/tun", O_RDWR);

    // Either the name is specified and we use TUNSETPPA,
    // or it's not specified and we just want a TUNNEWPPA
    if (ppa) {
        ppa = ioctl(tunFd, TUNSETPPA, ppa);
    } else {
        ppa = ioctl(tunFd, TUNNEWPPA, -1);
    }

    int ipFd = open("/dev/ip6", O_RDWR, 0);
    int tunFd2 = open("/dev/tun", O_RDWR, 0);

    if (tunFd < 0 || ipFd < 0 || ppa < 0 || tunFd2 < 0) {
        int err = errno;
        close(tunFd);
        close(ipFd);
        close(tunFd2);

        char* error = NULL;
        if (tunFd < 0) {
            error = "open(\"/dev/tun\")";
        } else if (ipFd < 0) {
            error = "open(\"/dev/ip6\")";
        } else if (ppa < 0) {
            error = "ioctl(TUNNEWPPA)";
        } else if (tunFd2 < 0) {
            error = "open(\"/dev/tun\") (opening for plumbing interface)";
        }
        Err_raise(alloc, "%s [%s]", error, strerror(err));
    }

    struct lifreq ifr = {
        .lifr_ppa = ppa,
        .lifr_flags = IFF_IPV6
    };

    // Since devices are numbered rather than named, it's not possible to have tun0 and cjdns0
    // so we'll skip the pretty names and call everything tunX
    int maxNameSize = (LIFNAMSIZ < TUNInterface_IFNAMSIZ) ? LIFNAMSIZ : TUNInterface_IFNAMSIZ;
    if (assignedInterfaceName) {
        snprintf(assignedInterfaceName, maxNameSize, "tun%d", ppa);
    }
    snprintf(ifr.lifr_name, maxNameSize, "tun%d", ppa);

    char* error = NULL;

    if (ioctl(tunFd, I_SRDOPT, RMSGD) < 0) {
        error = "putting tun into message-discard mode";

    } else if (ioctl(tunFd2, I_PUSH, "ip") < 0) {
        // add the ip module
        error = "ioctl(I_PUSH)";

    } else if (ioctl(tunFd2, SIOCSLIFNAME, &ifr) < 0) {
        // set the name of the interface and specify it as ipv6
        error = "ioctl(SIOCSLIFNAME)";

    } else if (ioctl(ipFd, I_LINK, tunFd2) < 0) {
        // link the device to the ipv6 router
        error = "ioctl(I_LINK)";
    }

    if (error) {
        int err = errno;
        close(ipFd);
        close(tunFd2);
        close(tunFd);
        Err_raise(alloc, "%s [%s]", error, strerror(err));
    }

    close(ipFd);

    struct Iface* s = NULL;
    Err(Rffi_socketForFd(&s, sout, tunFd, RTypes_SocketType_Frames, alloc));

    struct TUNInterface_Illumos_pvt* ctx =
        Allocator_clone(alloc, (&(struct TUNInterface_Illumos_pvt) {
            .externalIf = { .send = incomingFromWire },
            .internalIf = { .send = incomingFromUs },
        }));
    Iface_plumb(&ctx->externalIf, s);
    Identity_set(ctx);

    // TODO(cjd): This needs to be tested
    *out = &ctx->internalIf;
    return NULL;
}
