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

#include "admin/Admin.h"
#include "benc/String.h"
#include "exception/Except.h"
#include "memory/Allocator.h"
#include "tunnel/IpTunnel.h"
#include "util/events/FakeNetwork.h"
#include "util/Linker.h"
Linker_require("admin/angel/Core.c");

void Core_initTunnel(String* desiredDeviceName,
                     struct Sockaddr* addr,
                     uint8_t addressPrefix,
                     struct Log* logger,
                     struct IpTunnel* ipTunnel,
                     struct EventBase* eventBase,
                     struct Allocator* alloc,
                     struct Except* eh);

void Core_init(struct Allocator* alloc,
               struct Log* logger,
               struct EventBase* eventBase,
               uint8_t privateKey[32],
               struct Admin* admin,
               struct Random* rand,
               struct Except* eh,
               struct FakeNetwork* fakeNet,
               bool noSec);

int Core_main(int argc, char** argv);

#endif
