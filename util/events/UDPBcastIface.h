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
#ifndef UDPBcastIface_H
#define UDPBcastIface_H

#include "benc/List.h"
#include "exception/Except.h"
#include "interface/Iface.h"
#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/Gcc.h"
#include "util/log/Log.h"
#include "util/Linker.h"

#include <stdbool.h>
Linker_require("util/events/libuv/UDPBcastIface.c");

#define UDPBcastIface_CURRENT_VERSION 0
#define UDPBcastIface_PADDING_AMOUNT 512
#define UDPBcastIface_BUFFER_CAP 3496

/** Maximum number of bytes to hold in queue before dropping packets. */
#define UDPBcastIface_MAX_QUEUE 16384

#define UDPBcastIface_CURRENT_VERSION 0
Gcc_PACKED
struct UDPBcastIface_Header
{
    /** UDPBcastIface_CURRENT_VERSION, no communication is possible with different versions. */
    uint8_t version;

    /** padding and for future use. */
    uint8_t zero;

    /** Length of the content (excluding header) */
    uint16_t length_be;

    uint8_t bcast;
    uint8_t reversed;

    /** Pad to align boundry, also magic. */
    uint16_t magic_be;
};
#define UDPBcastIface_Header_SIZE 8
Assert_compileTime(sizeof(struct UDPBcastIface_Header) == UDPBcastIface_Header_SIZE);

struct UDPBcastIface
{
    struct AddrIface generic;
};

/**
 * @param base the event loop context.
 * @param bindAddr the address/port to bind to.
 * @param allocator the memory allocator for this message.
 * @param exHandler the handler to deal with whatever exception arises.
 * @param logger
 * @return a new UDPBcastInterfaceBase.
 */
struct UDPBcastIface* UDPBcastIface_new(struct EventBase* base,
                                        struct Sockaddr* bindAddr,
                                        const List* bindDevices,
                                        struct Allocator* allocator,
                                        struct Except* exHandler,
                                        struct Log* logger);

int UDPBcastIface_setBroadcast(struct UDPBcastIface* iface, bool enable);
List* UDPBcastIface_listDevices(struct Allocator* alloc, struct Except* eh);

#endif
