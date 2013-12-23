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
#ifndef UDPInterface_pvt_H
#define UDPInterface_pvt_H

#include "util/events/EventBase.h"
#include "interface/addressable/AddrInterface.h"
#include "interface/MultiInterface.h"
#include "interface/UDPInterface.h"
#include "interface/InterfaceController.h"
#include "memory/Allocator.h"
#include "util/Identity.h"
#include "util/log/Log.h"
#include "util/platform/Socket.h"
#include "util/events/Event.h"

#define UDPInterface_MAX_PACKET_SIZE 8192

#define UDPInterface_PADDING 512


struct UDPInterface_pvt
{
    struct UDPInterface pub;

    struct AddrInterface* udpBase;

    struct Log* logger;

    struct InterfaceController* ic;

    struct MultiInterface* multiIface;

    struct Allocator* alloc;

    uint8_t messageBuff[UDPInterface_PADDING + UDPInterface_MAX_PACKET_SIZE];

    Identity
};

#endif
