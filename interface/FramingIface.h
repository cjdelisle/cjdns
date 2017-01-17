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
#ifndef FramingIface_H
#define FramingIface_H

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/FramingIface.c");

struct FramingIface
{
    struct Iface messageIf;
    struct Iface streamIf;
};

/**
 * Framed message format:
 * [4 bytes length][ <length> bytes content ....... ]
 * The length is of only the content, not including the beginning 4 bytes
 * which represents the length itself.
 *
 * @param maxMessageSize how large of a framed message to allow
 * @param padding the amount of padding to apply to each full frame when it comes in, for outgoing
 *                messages the message has no added padding, it is just sent across the wire as it
 *                is.
 * @param alloc
 */
struct FramingIface* FramingIface_new(uint32_t maxMessageSize,
                                      uint32_t padding,
                                      struct Allocator* alloc);

#endif
