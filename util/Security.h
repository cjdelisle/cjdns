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

#include "exception/ExceptionHandler.h"
#include "util/Log.h"

#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <errno.h>
#include <unistd.h>

static inline void Security_setUser(char* userName, struct Log* logger, struct ExceptionHandler* eh)
{
    errno = 0;
    struct passwd* pw = getpwnam(userName);
    if (!pw) {
        eh->exception(__FILE__ " couldn't find user to set username to.", errno, eh);
        return;
    }
    if (setuid(pw->pw_uid)) {
        if (errno == EPERM) {
            Log_warn(logger, "You do not have permission to set UID, skipping.\n");
            return;
        }
        eh->exception(__FILE__ " couldn't set UID.", errno, eh);
    }
}

static inline void Security_noFiles(struct ExceptionHandler* eh)
{
    #ifdef BSD
        #define Security_OPEN_FILE_LIMIT RLIMIT_OFILE
    #else
        #define Security_RLIMIT_NOFILE
    #endif
    errno = 0;
    if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){ 0, 0 })) {
        eh->exception(__FILE__ " failed to set open file limit to zero.", errno, eh);
    }
}

#endif
