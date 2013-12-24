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
#define string_strerror
#include "exception/Except.h"
#include "util/log/Log.h"
#include "util/Security.h"
#include "util/platform/libc/string.h"

#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <errno.h>
#include <stdlib.h>

int Security_setUser(char* userName, struct Log* logger, struct Except* eh)
{
    struct passwd* pw = getpwnam(userName);
    if (!pw) {
        Except_throw(eh, "Failed to set UID, couldn't find user named [%s].", strerror(errno));
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

static void noFiles(struct Except* eh)
{
    #if !defined(RLIMIT_NOFILE) && defined(RLIMIT_OFILE)
        #define RLIMIT_NOFILE RLIMIT_OFILE
    #endif

    int file = dup(0);
    if (file < 0) {
        Except_throw(eh, "Unable to dupe stdin");
    }
    close(file);
    if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){ 0, 0 })) {
        Except_throw(eh, "Failed to set open file limit to [%s]", strerror(errno));
    }
    file = dup(0);
    close(file);
    if (file >= 0) {
        Except_throw(eh, "Still able to dupe stdin after setting number of files to 0!");
    }
}

static void noForks(struct Except* eh)
{
/* TODO: understand why we are still able to fork after setting NPROC to 0
    int pid = fork();
    if (pid == 0) { exit(0); }
    if (pid < 0) {
        Except_throw(eh, "Unable to fork prior to setting noForks");
    }
    if (setrlimit(RLIMIT_NPROC, &(struct rlimit){ 0, 0 })) {
        Except_throw(eh, "Failed to set fork limit to [%s]", strerror(errno));
    }

    pid = fork();
    if (pid == 0) { exit(0); }
    if (pid > 0) {
        Except_throw(eh, "Still able to fork after calling noForks!");
    }
*/
}

static void maxMemory(unsigned long max, struct Except* eh)
{
    // RLIMIT_DATA doesn't prevent malloc() on linux.
    // see: http://lkml.indiana.edu/hypermail/linux/kernel/0707.1/0675.html
    #if !defined(RLIMIT_AS) && defined(RLIMIT_DATA)
        #define RLIMIT_AS RLIMIT_DATA
    #endif
    if (setrlimit(RLIMIT_AS, &(struct rlimit){ max, max })) {
        Except_throw(eh, "Failed to limit available memory [%s]", strerror(errno));
    }
}

void Security_dropPermissions(struct Except* eh)
{
    maxMemory(100000000, eh);
    noFiles(eh);
return;
    noForks(eh);
}
