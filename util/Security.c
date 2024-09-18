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
#define _GNU_SOURCE // chroot(), MAP_ANONYMOUS
#include "benc/Dict.h"
#include "benc/String.h"
#include "exception/Err.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "memory/Allocator.h"
#include "util/Setuid.h"
#include "util/events/EventBase.h"
#include "util/events/Timeout.h"

#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <stdio.h>

Dict* Security_getUser(char* userName, struct Allocator* retAlloc)
{
    struct passwd* pw;
    if (userName) {
        pw = getpwnam(userName);
    } else {
        pw = getpwuid(getuid());
    }
    Dict* ret = Dict_new(retAlloc);
    if (!pw) {
        Dict_putString(ret, String_new("error", retAlloc),
                            String_printf(retAlloc, "Could not find user [%s]", strerror(errno)),
                            retAlloc);
        return ret;
    }
    Dict_putString(ret, String_new("error", retAlloc), String_new("none", retAlloc), retAlloc);
    Dict_putString(ret, String_new("name", retAlloc), String_new(pw->pw_name, retAlloc), retAlloc);
    Dict_putInt(ret, String_new("uid", retAlloc), pw->pw_uid, retAlloc);
    Dict_putInt(ret, String_new("gid", retAlloc), pw->pw_gid, retAlloc);
    return ret;
}

Err_DEFUN Security_setUser(int uid,
                      int gid,
                      bool keepNetAdmin,
                      struct Log* logger,
                      struct Allocator* alloc)
{
    int gidErrno = 0;
    int uidErrno = 0;
    if (keepNetAdmin) {
        Err(Setuid_preSetuid(alloc));
    }
    if (gid && setgid(gid)) {
        gidErrno = errno;
    }
    if (setuid(uid)) {
        // errno is global and could get overwritten by Setuid_postSetuid()
        uidErrno = errno;
    }
    if (keepNetAdmin) {
        Err(Setuid_postSetuid(alloc));
    }
    if (uidErrno > 0) {
        Err_raise(alloc, "Failed to set UID [%s]", strerror(uidErrno));
    }
    if (uid != (int) getuid()) {
        Err_raise(alloc, "Failed to set UID but seemed to succeed");
    }
    if (gidErrno > 0) {
        Err_raise(alloc, "Failed to set GID [%s]", strerror(gidErrno));
    }
    if (gid != (int) getgid()) {
        Err_raise(alloc, "Failed to set GID but seemed to succeed");
    }
    return NULL;
}

static int canOpenFiles()
{
    int file = dup(0);
    close(file);
    return file >= 0;
}

Err_DEFUN Security_nofiles(struct Allocator* errAlloc)
{
    #if !defined(RLIMIT_NOFILE) && defined(RLIMIT_OFILE)
        #define RLIMIT_NOFILE RLIMIT_OFILE
    #endif

    if (!canOpenFiles()) {
        Err_raise(errAlloc, "Unable to dupe stdin");
    }
    if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){ 0, 0 })) {
        Err_raise(errAlloc, "Failed to set open file limit to 0 [%s]", strerror(errno));
    }
    if (canOpenFiles()) {
        Err_raise(errAlloc, "Still able to dupe stdin after setting number of files to 0!");
    }
    return NULL;
}

Err_DEFUN Security_noforks(struct Allocator* errAlloc)
{
    if (setrlimit(RLIMIT_NPROC, &(struct rlimit){ 0, 0 })) {
        Err_raise(errAlloc, "Failed to set fork limit to 0 [%s]", strerror(errno));
    }
    return NULL;
}

Err_DEFUN Security_chroot(char* root, struct Allocator* errAlloc)
{
    if (chdir(root)) {
        Err_raise(errAlloc, "chdir(%s) -> [%s]", root, strerror(errno));
    }
    if (chroot(root)) {
        Err_raise(errAlloc, "chroot(%s) -> [%s]", root, strerror(errno));
    }
    return NULL;
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
    exit(232);
}

struct Security* Security_new(struct Allocator* alloc, struct Log* log, EventBase_t* base)
{
    struct Security_pvt* sec = Allocator_calloc(alloc, sizeof(struct Security_pvt), 1);
    Identity_set(sec);
    sec->setupAlloc = Allocator_child(alloc);
    Timeout_setInterval(fail, sec, 20000, base, sec->setupAlloc);
    sec->log = log;
    return &sec->pub;
}

Err_DEFUN Security_checkPermissions(struct Security_Permissions** outP, struct Allocator* alloc)
{
    struct Security_Permissions* out =
        Allocator_calloc(alloc, sizeof(struct Security_Permissions), 1);

    out->noOpenFiles = !canOpenFiles();
    out->uid = getuid();

    *outP = out;
    return NULL;
}
