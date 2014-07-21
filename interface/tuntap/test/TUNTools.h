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
#ifndef TUNTools_H
#define TUNTools_H

#include "interface/addressable/AddrInterface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/platform/Sockaddr.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/test/TUNTools.c")

struct AddrInterface* TUNTools_setupUDP(struct EventBase* base,
                                        struct Sockaddr* bindAddr,
                                        struct Allocator* allocator,
                                        struct Log* logger);

struct Timeout* TUNTools_sendHelloWorld(struct AddrInterface* iface,
                                        struct Sockaddr* dest,
                                        struct EventBase* base,
                                        struct Allocator* alloc);

#endif
