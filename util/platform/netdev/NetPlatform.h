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
#ifndef NetPlatform_H
#define NetPlatform_H

#include "memory/Allocator.h"
#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/platform/Sockaddr.h"
#include "util/Linker.h"
Linker_require("util/platform/netdev/NetPlatform_" + builder.config.systemName + ".c");

#include <stdint.h>

void NetPlatform_flushAddresses(const char* deviceName, struct Except* eh);

void NetPlatform_addAddress(const char* interfaceName,
                            const uint8_t* address,
                            int prefixLen,
                            int addrFam,
                            struct Log* logger,
                            struct Allocator* tempAlloc,
                            struct Except* eh);

void NetPlatform_setMTU(const char* interfaceName,
                        uint32_t mtu,
                        struct Log* logger,
                        struct Except* eh);

void NetPlatform_setRoutes(const char* ifName,
                           struct Sockaddr** prefixSet,
                           int prefixCount,
                           struct Log* logger,
                           struct Allocator* tempAlloc,
                           struct Except* eh);
#endif
