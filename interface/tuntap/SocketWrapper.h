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
#ifndef SocketWrapper_H
#define SocketWrapper_H

#define SocketWrapper_TYPE_TUN_PACKET 0x0
#define SocketWrapper_TYPE_CONF_ADD_IPV6_ADDRESS 0x1
#define SocketWrapper_TYPE_CONF_SET_MTU 0x2

#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/SocketWrapper.c");

struct SocketWrapper
{
    struct Iface internalIf;
    struct Iface externalIf;
};

struct SocketWrapper* SocketWrapper_new(struct Allocator* alloc, struct Log* log);

void SocketWrapper_addAddress(struct Iface* rawSocketIf,
                                uint8_t* ipv6Addr,
                                struct Log* logger,
                                struct Except* eh,
                                struct Allocator* alloc);

void SocketWrapper_setMTU(struct Iface* rawSocketIf,
                            uint32_t mtu,
                            struct Log* logger,
                            struct Except* eh,
                            struct Allocator* alloc);
#endif
