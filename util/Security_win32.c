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
#include "benc/Dict.h"
#include "benc/String.h"
#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "memory/Allocator.h"
#include "util/Bits.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"

#include <unistd.h>

Dict* Security_getUser(char* userName, struct Allocator* retAlloc)
{
    Dict* ret = Dict_new(retAlloc);
    Dict_putString(ret, String_new("error", retAlloc),
                        String_new("Not supported on windows", retAlloc),
                        retAlloc);
    return ret;
}

void Security_setUser(int uid,
                      int gid,
                      bool keepNetAdmin,
                      struct Log* logger,
                      struct Except* eh,
                      struct Allocator* alloc)
{
}

void Security_nofiles(struct Except* eh)
{
}

void Security_noforks(struct Except* eh)
{
}

void Security_chroot(char* root, struct Except* eh)
{
}

void Security_seccomp(struct Allocator* tempAlloc, struct Log* logger, struct Except* eh)
{
}

struct Security_pvt
{
    struct Security pub;
    struct Allocator* setupAlloc;
    struct Log* log;
    Identity
};

void Security_setupComplete(struct Security* security)
{
    struct Security_pvt* sec = Identity_check((struct Security_pvt*) security);
    sec->pub.setupComplete = 1;
    Allocator_free(sec->setupAlloc);
}

static void fail(void* vSec)
{
    struct Security_pvt* sec = Identity_check((struct Security_pvt*) vSec);
    Log_critical(sec->log, "Security_setupComplete() not called in time, exiting");
    _exit(232);
}

struct Security* Security_new(struct Allocator* alloc, struct Log* log, struct EventBase* base)
{
    struct Security_pvt* sec = Allocator_calloc(alloc, sizeof(struct Security_pvt), 1);
    Identity_set(sec);
    sec->setupAlloc = Allocator_child(alloc);
    Timeout_setInterval(fail, sec, 20000, base, sec->setupAlloc);
    sec->log = log;
    return &sec->pub;
}

struct Security_Permissions* Security_checkPermissions(struct Allocator* alloc, struct Except* eh)
{
    struct Security_Permissions* out =
        Allocator_calloc(alloc, sizeof(struct Security_Permissions), 1);
    return out;
}
