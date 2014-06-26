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
#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "util/Seccomp.h"
#include "memory/Allocator.h"

#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

#define __USE_MISC // for MAP_ANONYMOUS
#include <sys/mman.h>

// Apple
#ifndef MAP_ANONYMOUS
    #define MAP_ANONYMOUS MAP_ANON
#endif

int Security_setUser(char* userName, struct Log* logger, struct Except* eh)
{
    struct passwd* pw = getpwnam(userName);
    if (!pw) {
        Except_throw(eh, "Failed to set UID, couldn't find user named [%s].",
                     strerror(errno));
    }
    if (setuid(pw->pw_uid)) {
        if (errno == EPERM) {
            return Security_setUser_PERMISSION;
        }
        Except_throw(eh, "Failed to set UID [%s]", strerror(errno));
    }
    if (getuid() != pw->pw_uid) {
        Except_throw(eh, "Failed to set UID but seemed to succeed");
    }
    return 0;
}

static int canOpenFiles()
{
    int file = dup(0);
    close(file);
    return file >= 0;
}

static void noFiles(struct Except* eh)
{
    #if !defined(RLIMIT_NOFILE) && defined(RLIMIT_OFILE)
        #define RLIMIT_NOFILE RLIMIT_OFILE
    #endif

    if (!canOpenFiles()) {
        Except_throw(eh, "Unable to dupe stdin");
    }
    if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){ 0, 0 })) {
        Except_throw(eh, "Failed to set open file limit to [%s]", strerror(errno));
    }
    if (canOpenFiles()) {
        Except_throw(eh, "Still able to dupe stdin after setting number of files to 0!");
    }
}

// RLIMIT_DATA doesn't prevent malloc() on linux.
// see: http://lkml.indiana.edu/hypermail/linux/kernel/0707.1/0675.html
#if !defined(RLIMIT_AS) && defined(RLIMIT_DATA)
    #define Security_MEMORY_RLIMIT RLIMIT_DATA
#elif defined(RLIMIT_AS)
    #define Security_MEMORY_RLIMIT RLIMIT_AS
#else
    #error RLIMIT_AS and RLIMIT_DATA are not defined
#endif

static unsigned long getMaxMem(struct Except* eh)
{
    struct rlimit lim = { 0, 0 };
    if (getrlimit(Security_MEMORY_RLIMIT, &lim)) {
        Except_throw(eh, "Failed to get memory limit [%s]", strerror(errno));
    }

    // First time around, we try a very small mapping just to make sure it works.
    size_t tryMapping = 100;
    if (lim.rlim_max > 0) {
        tryMapping = lim.rlim_max * 2l;
    }

    void* ptr = mmap(NULL, tryMapping, PROT_READ | PROT_WRITE, MAP_ANONYMOUS, -1, 0);
    if (ptr != MAP_FAILED) {
        munmap(ptr, tryMapping);
        if (lim.rlim_max > 0) {
            Except_throw(eh, "Memory limit is not enforced, successfully mapped [%zu] bytes",
                         tryMapping);
        }
    } else if (lim.rlim_max == 0) {
        Except_throw(eh, "Testing of memory limit not possible, unable to map memory");
    }

    return lim.rlim_max;
}

static void maxMemory(unsigned long max, struct Except* eh)
{
    unsigned long realMax = getMaxMem(eh);
    if (realMax > 0 && realMax < max) {
        Except_throw(eh, "Failed to limit available memory to [%lu] "
                         "because existing limit is [%lu]", max, realMax);
    }

    if (setrlimit(Security_MEMORY_RLIMIT, &(struct rlimit){ max, max })) {
        Except_throw(eh, "Failed to limit available memory [%s]", strerror(errno));
    }
    if (!setrlimit(Security_MEMORY_RLIMIT, &(struct rlimit){ max+1, max+1 })) {
        Except_throw(eh, "Available memory was modifyable after limiting");
    }

    realMax = getMaxMem(eh);
    if (realMax != max) {
        Except_throw(eh, "Limiting available memory failed");
    }
}

struct Security_Permissions* Security_checkPermissions(struct Allocator* alloc, struct Except* eh)
{
    struct Security_Permissions* out =
        Allocator_calloc(alloc, sizeof(struct Security_Permissions), 1);

    out->noOpenFiles = !canOpenFiles();
    out->seccompExists = Seccomp_exists();
    out->seccompEnforcing = Seccomp_isWorking();
    out->memoryLimitBytes = getMaxMem(eh);

    return out;
}

void Security_dropPermissions(struct Allocator* tempAlloc, struct Log* logger, struct Except* eh)
{
    maxMemory(100000000, eh);
    noFiles(eh);
    Seccomp_dropPermissions(tempAlloc, logger, eh);
}
