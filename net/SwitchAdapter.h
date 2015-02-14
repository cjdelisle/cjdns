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
#ifndef SwitchAdapter_H
#define SwitchAdapter_H

#include "interface/Interface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/SwitchAdapter.c")

/**
 * Connects the actual switch interface to the BalingWire and the ControlHandler.
 */
struct SwitchAdapter
{
    /** Sends and handles packets to/from switch. */
    struct Interface switchIf;

    /**
     * Sends forth the packets with control headers (0xffffffff) under the switch header.
     * Directs all input to the switch.
     */
    struct Interface_Two controlIf;

    /**
     * Sends all packets which are not control frames.
     * Directs all input to the switch.
     */
    struct Interface_Two balingWireIf;
};

struct SwitchAdapter* SwitchAdapter_new(struct Allocator* alloc, struct Log* log);

#endif
