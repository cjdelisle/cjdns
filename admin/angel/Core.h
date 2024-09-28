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
#ifndef Core_H
#define Core_H

#include "admin/Admin.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "util/Linker.h"
Linker_require("admin/angel/Core.c")

Err_DEFUN Core_initTunnel(String* desiredDeviceName,
                     struct Sockaddr* addr,
                     uint8_t addressPrefix,
                     struct Log* logger,
                     struct IpTunnel* ipTunnel,
                     EventBase_t* eventBase,
                     struct Allocator* alloc);

Err_DEFUN Core_init(struct Allocator* alloc,
               struct Log* logger,
               EventBase_t* eventBase,
               uint8_t privateKey[32],
               struct Admin* admin,
               struct Random* rand,
               bool noSec);

int Core_main(int argc, char** argv);

#endif
