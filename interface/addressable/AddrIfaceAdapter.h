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
#ifndef AddrIfaceAdapter_H
#define AddrIfaceAdapter_H

#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("interface/addressable/AddrIfaceAdapter.c");

struct AddrIfaceAdapter
{
    struct AddrIface generic;
    struct Iface inputIf;
};

struct AddrIfaceAdapter* AddrIfaceAdapter_new(struct Allocator* alloc);

#endif
