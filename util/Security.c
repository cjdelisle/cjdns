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
#include "util/Errno.h"
#include "util/Security.h"

#include <sys/resource.h>
#include <sys/types.h>
#include <pwd.h>
#include <unistd.h>
#include <string.h>

void Security_setUser(char* userName, struct Log* logger, struct Except* eh)
{
    struct passwd* pw = getpwnam(userName);
    if (!pw) {
        Except_raise(eh,
                     Security_setUser_NO_SUCH_USER,
                     "Failed to set UID, couldn't find user named [%s] in the system.",
                     Errno_getString());
        return;
    }
    if (setuid(pw->pw_uid)) {
        if (Errno_get() == Errno_EPERM) {
            Except_raise(eh, Security_setUser_PERMISSION,
                         "You do not have permission to set UID.");
            return;
        }
        Except_raise(eh, Security_setUser_INTERNAL, "Failed to set UID [%s]",
                     Errno_getString());
    }
}

void Security_noFiles(struct Except* eh)
{
    #if !defined(RLIMIT_NOFILE) && defined(RLIMIT_OFILE)
        #define RLIMIT_NOFILE RLIMIT_OFILE
    #endif
    if (setrlimit(RLIMIT_NOFILE, &(struct rlimit){ 0, 0 })) {
        Except_raise(eh, -1, "Failed to set open file limit to zero [%s]", Errno_getString());
    }
}
