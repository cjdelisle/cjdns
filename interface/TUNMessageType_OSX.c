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
#include "wire/Message.h"
#include "interface/TUNMessageType.h"
#include "wire/Ethernet.h"

#include <stdint.h>
#include <netinet/in.h>

#define AF_INET6_BE Endian_hostToBigEndian16(AF_INET6)
#define AF_INET_BE Endian_hostToBigEndian16(AF_INET)

/**
 * OSX is broken and expects you to send the platform dependent
 * address family type rather than the ethertype.
 */

void TUNMessageType_push(struct Message* message, uint16_t ethertype)
{
    uint16_t afType_be = (ethertype == Ethernet_TYPE_IP6) ? AF_INET6_BE : AF_INET_BE;

    Message_shift(message, 4);
    ((uint16_t*) message->bytes)[0] = 0;
    ((uint16_t*) message->bytes)[1] = afType_be;
}

uint16_t TUNMessageType_pop(struct Message* message)
{
    Message_shift(message, -4);
    uint16_t afType_be = ((uint16_t*) message->bytes)[-1];

    return (afType_be == AF_INET6_BE) ? Ethernet_TYPE_IP6 : Ethernet_TYPE_IP4;
}
