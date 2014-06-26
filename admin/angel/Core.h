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
#ifndef Core_H
#define Core_H

#include "benc/String.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "net/Ducttape.h"
#include "util/Linker.h"
Linker_require("admin/angel/Core.c")

void Core_initTunnel(String* desiredDeviceName,
                     struct Sockaddr* addr,
                     uint8_t addressPrefix,
                     struct Ducttape* dt,
                     struct Log* logger,
                     struct IpTunnel* ipTunnel,
                     struct EventBase* eventBase,
                     struct Allocator* alloc,
                     struct Except* eh);

void Core_init(struct Allocator* alloc,
               struct Log* logger,
               struct EventBase* eventBase,
               struct Interface* angelIface,
               struct Random* rand,
               struct Except* eh);

int Core_main(int argc, char** argv);

#endif
