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
#ifndef AddrIfaceMuxer_H
#define AddrIfaceMuxer_H

#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/addressable/AddrIfaceMuxer.c")

struct AddrIfaceMuxer
{
    struct AddrIface iface;
};

/**
 * Register an interface with the muxer.
 * After registering, you will have the interface which you can then interact with.
 * To unregister, free the allocator which is provided in this function.
 */
struct Iface* AddrIfaceMuxer_registerIface(struct AddrIfaceMuxer* muxer, struct Allocator* alloc);

struct AddrIfaceMuxer* AddrIfaceMuxer_new(struct Log* log, struct Allocator* alloc);

#endif
