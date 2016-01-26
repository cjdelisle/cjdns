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
#ifndef Security_H
#define Security_H

#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
#ifdef win32
    Linker_require("util/Security_win32.c");
#else
    Linker_require("util/Security.c");
#endif

#include <stdint.h>
#include <stdbool.h>

struct Security_Permissions
{
    int noOpenFiles;
    int seccompExists;
    int seccompEnforcing;
    int uid;
};

struct Security
{
    bool setupComplete;
};

void Security_setUser(int uid,
                      int gid,
                      bool keepNetAdmin,
                      struct Log* logger,
                      struct Except* eh,
                      struct Allocator* alloc);

void Security_nofiles(struct Except* eh);

void Security_noforks(struct Except* eh);

void Security_chroot(char* root, struct Except* eh);

void Security_seccomp(struct Allocator* tempAlloc, struct Log* logger, struct Except* eh);

void Security_setupComplete(struct Security* security);

struct Security* Security_new(struct Allocator* alloc, struct Log* log, struct EventBase* base);

Dict* Security_getUser(char* userName, struct Allocator* retAlloc);

struct Security_Permissions* Security_checkPermissions(struct Allocator* alloc, struct Except* eh);

#endif
