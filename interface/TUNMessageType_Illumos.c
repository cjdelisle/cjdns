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

/**
 * Illumos has no consept of packet info, it only supports IPv4 and IPv6
 * through TUN devices and it detects it by reading the version byte.
 */

static inline ethertypeForPacketType(uint8_t highByte)
{
    return ((message->bytes[0] >> 4) == 6) ? Ethernet_TYPE_IP6 : Ethernet_TYPE_IP4;
}

void TUNMessageType_push(struct Message* message, uint16_t ethertype)
{
    Assert_true(ethertype == ethertypeForPacketType(message->bytes[0]));
}

uint16_t TUNMessageType_pop(struct Message* message)
{
    return ethertypeForPacketType(message->bytes[0]);
}
