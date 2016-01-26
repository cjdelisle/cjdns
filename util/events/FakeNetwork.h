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
#ifndef FakeNetwork_H
#define FakeNetwork_H

#include "exception/Except.h"
#include "interface/Iface.h"
#include "interface/addressable/AddrIface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/FakeNetwork.c");

struct FakeNetwork
{
    int unused;
};

struct FakeNetwork_UDPIface
{
    struct AddrIface generic;
};

struct FakeNetwork* FakeNetwork_new(struct EventBase* base,
                                    struct Allocator* allocator,
                                    struct Log* logger);

struct FakeNetwork_UDPIface* FakeNetwork_iface(struct FakeNetwork* net,
                                               struct Sockaddr* bindAddress,
                                               struct Allocator* alloc);
#endif
