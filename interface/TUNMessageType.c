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
#include "interface/TUNMessageType.h"
#include "util/Assert.h"
#include "wire/Ethernet.h"
#include "wire/Message.h"

#include <stdint.h>
#include <netinet/in.h>


/**
 * Simple and sane, the 4 bytes prior to the message contain flags and the
 * big endian ethertype. Flags only contain a single flag which we don't use.
 */
void TUNMessageType_push_Linux(struct Message* message, uint16_t ethertype)
{
    Message_shift(message, 4);
    ((uint16_t*) message->bytes)[0] = 0;
    ((uint16_t*) message->bytes)[1] = ethertype;
}

uint16_t TUNMessageType_pop_Linux(struct Message* message)
{
    Message_shift(message, -4);
    return ((uint16_t*) message->bytes)[-1];
}


/**
 * OSX and BSD is expect you to send the platform dependent
 * address family type rather than the ethertype.
 */
#define AF_INET6_BE Endian_hostToBigEndian16(AF_INET6)
#define AF_INET_BE Endian_hostToBigEndian16(AF_INET)

void TUNMessageType_push_BSD(struct Message* message, uint16_t ethertype)
{
    uint16_t afType_be = (ethertype == Ethernet_TYPE_IP6) ? AF_INET6_BE : AF_INET_BE;

    Message_shift(message, 4);
    ((uint16_t*) message->bytes)[0] = 0;
    ((uint16_t*) message->bytes)[1] = afType_be;
}
uint16_t TUNMessageType_pop_BSD(struct Message* message)
{
    Message_shift(message, -4);
    uint16_t afType_be = ((uint16_t*) message->bytes)[-1];

    return (afType_be == AF_INET6_BE) ? Ethernet_TYPE_IP6 : Ethernet_TYPE_IP4;
}


/**
 * Illumos has no concept of packet info, it only supports IPv4 and IPv6
 * through TUN devices and it detects it by reading the version byte.
 */
static inline uint16_t ethertypeForPacketType(uint8_t highByte)
{
    return ((highByte >> 4) == 6) ? Ethernet_TYPE_IP6 : Ethernet_TYPE_IP4;
}

void TUNMessageType_push_Illumos(struct Message* message, uint16_t ethertype)
{
    Assert_true(ethertype == ethertypeForPacketType(message->bytes[0]));
}

uint16_t TUNMessageType_pop_Illumos(struct Message* message)
{
    return ethertypeForPacketType(message->bytes[0]);
}
