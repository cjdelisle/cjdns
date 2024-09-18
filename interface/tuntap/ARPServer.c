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
#include "interface/tuntap/TUNMessageType.h"
#include "interface/tuntap/ARPServer.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/log/Log.h"
#include "util/Identity.h"
#include "util/Endian.h"
#include "wire/ARPHeader.h"
#include "wire/Ethernet.h"

#include <stdbool.h>

struct ARPServer_pvt
{
    struct ARPServer pub;
    struct Iface external;
    struct Log* log;
    uint8_t localMac[Ethernet_ADDRLEN];
    Identity
};

static bool isValidARP(Message_t* msg)
{
    struct ARPHeader_6_4 arp;
    Err_assert(Message_epop(msg, &arp, ARPHeader_6_4_SIZE));
    Err_assert(Message_eshift(msg, ARPHeader_6_4_SIZE)); // Get copy and restore.
    if (!ARPHeader_isEthIP4(&arp.prefix)) {
        return false; // not ARP.
    }
    if (arp.prefix.operation != ARPHeader_OP_Q) {
        return false; // Not question.
    }
    if (Bits_memcmp(arp.spa, arp.tpa, 4) == 0) {
        return false; // not really ARP question.
    }
    if (Bits_isZero(arp.spa, 4)) {
        return false; // "I am just checking if this IP is not taken."
    }
    return true;
}

static Iface_DEFUN answerARP(Message_t* msg, struct ARPServer_pvt* as)
{
    struct ARPHeader_6_4 arp;
    Err(Message_epop(msg, &arp, ARPHeader_6_4_SIZE));
    if (Message_getLength(msg)) {
        Log_warn(as->log, "%d extra bytes in ARP, weird", Message_getLength(msg));
    }
    // Swap sender with target.
    // 10 = Eth_Len + IP4_Len
    uint8_t tmp[10];
    Bits_memcpy(tmp, arp.sha, 10);
    Bits_memcpy(arp.sha, arp.tha, 10);
    Bits_memcpy(arp.tha, tmp, 10);
    // Set our MAC as source
    Bits_memcpy(arp.sha, as->localMac, Ethernet_ADDRLEN);
    // Set answer opcode.
    arp.prefix.operation = ARPHeader_OP_A;

    Err(Message_epush(msg, &arp, ARPHeader_6_4_SIZE));
    Er_assert(TUNMessageType_push(msg, Ethernet_TYPE_ARP));

    Log_debug(as->log, "Sending ARP answer.");

    return Iface_next(&as->external, msg);
}

static Iface_DEFUN receiveMessage(Message_t* msg, struct Iface* external)
{
    struct ARPServer_pvt* as = Identity_containerOf(external, struct ARPServer_pvt, external);
    // Length should be ARP + Ethertype
    if (Message_getLength(msg) >= ARPHeader_6_4_SIZE + 4) {
        uint16_t ethertype = Er_assert(TUNMessageType_pop(msg));
        if (ethertype == Ethernet_TYPE_ARP) {
            if (isValidARP(msg)) {
                return answerARP(msg, as);
            }
        }
        Er_assert(TUNMessageType_push(msg, ethertype));
    }
    return Iface_next(&as->pub.internal, msg);
}

static Iface_DEFUN sendMessage(Message_t* msg, struct Iface* internal)
{
    struct ARPServer_pvt* as = Identity_containerOf(internal, struct ARPServer_pvt, pub.internal);
    return Iface_next(&as->external, msg);
}

struct ARPServer* ARPServer_new(struct Iface* external,
                                struct Log* log,
                                uint8_t localMac[Ethernet_ADDRLEN],
                                struct Allocator* alloc)
{
    struct ARPServer_pvt* out = Allocator_calloc(alloc, sizeof(struct ARPServer_pvt), 1);
    Identity_set(out);
    out->external.send = receiveMessage;
    Iface_plumb(&out->external, external);
    out->pub.internal.send = sendMessage;
    out->log = log;
    Bits_memcpy(out->localMac, localMac, Ethernet_ADDRLEN);
    return &out->pub;
}

