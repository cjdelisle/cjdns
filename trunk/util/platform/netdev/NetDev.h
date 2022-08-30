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
#ifndef NetDev_H
#define NetDev_H

#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("util/platform/netdev/NetDev.c")

/**
 * Set an address on an interface, if the interface is down (linux) bring it up,
 * and if the interface requires an explicit route (Illumos), configure it.
 *
 * @param ifName the name of the interface to alter.
 * @param sa the ip address to set.
 * @param prefixLen the number of bits netmask to include in the route.
 * @param logger
 * @param alloc allocator for errors.
 */
Er_DEFUN(void NetDev_addAddress(const char* ifName,
                       struct Sockaddr* sa,
                       struct Log* logger,
                       struct Allocator* alloc));

/**
 * Set the MTU of an interface.
 *
 * @param interfaceName the name of the interface to set the MTU for.
 * @param mtu the desired MTU.
 * @param logger where to write information.
 * @param eh an exception handler.
 */
Er_DEFUN(void NetDev_setMTU(const char* interfaceName,
                   uint32_t mtu,
                   struct Log* logger,
                   struct Allocator* alloc));

Er_DEFUN(void NetDev_flushAddresses(const char* deviceName, struct Allocator* alloc));

/**
 * Set a route to an interface.
 */
Er_DEFUN(void NetDev_setRoutes(const char* ifName,
                      struct Sockaddr** prefixSet,
                      int prefixCount,
                      struct Log* logger,
                      struct Allocator* tempAlloc));
#endif
