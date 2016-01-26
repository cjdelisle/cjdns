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
#ifndef ETHInterface_H
#define ETHInterface_H

#include "interface/addressable/AddrIface.h"
#include "benc/List.h"
#include "util/events/EventBase.h"
#include "net/InterfaceController.h"
#include "util/Gcc.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/ETHInterface_" + builder.config.systemName + ".c");

Gcc_PACKED
struct ETHInterface_Header
{
    /** ETHInterface_CURRENT_VERSION, no communication is possible with different versions. */
    uint8_t version;

    /** padding and for future use. */
    uint8_t zero;

    /** Length of the content (excluding header) */
    uint16_t length_be;

    /** Pad to align boundry, also magic. */
    uint16_t fc00_be;
};
#define ETHInterface_Header_SIZE 6
Assert_compileTime(sizeof(struct ETHInterface_Header) == ETHInterface_Header_SIZE);

/** The content of a Sockaddr emitted from ETHInterface. */
struct ETHInterface_Sockaddr
{
    struct Sockaddr generic;
    /*
     * We need to make the first byte following the Sockaddr be 0 because
     * Sockaddr_normalizeNative will zero it.
     */
    uint16_t zero;
    uint8_t mac[6];
};
#define ETHInterface_Sockaddr_SIZE 16
Assert_compileTime(sizeof(struct ETHInterface_Sockaddr) == ETHInterface_Sockaddr_SIZE);

#define ETHInterface_CURRENT_VERSION 0

struct ETHInterface
{
    struct AddrIface generic;
};

struct ETHInterface* ETHInterface_new(struct EventBase* eventBase,
                                      const char* bindDevice,
                                      struct Allocator* alloc,
                                      struct Except* exHandler,
                                      struct Log* logger);

List* ETHInterface_listDevices(struct Allocator* alloc, struct Except* eh);

#endif
