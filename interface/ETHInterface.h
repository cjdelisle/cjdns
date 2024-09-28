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
#ifndef ETHInterface_H
#define ETHInterface_H

#include "interface/addressable/AddrIface.h"
#include "exception/Err.h"
#include "util/events/EventBase.h"
#include "util/Gcc.h"
#include "util/Assert.h"
#include "util/log/Log.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/ETHInterface_" + builder.config.systemName + ".c")

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
} Gcc_PACKED;
#define ETHInterface_Header_SIZE 6
Assert_compileTime(sizeof(struct ETHInterface_Header) == ETHInterface_Header_SIZE);

#define ETHInterface_CURRENT_VERSION 0

struct ETHInterface
{
    AddrIface_t generic;
};

Err_DEFUN ETHInterface_new(
    struct ETHInterface** out,
    EventBase_t* eventBase,
    const char* bindDevice,
    struct Allocator* alloc,
    struct Log* logger);

Err_DEFUN ETHInterface_listDevices(List** out, struct Allocator* alloc);

#endif
