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
#include "memory/Allocator.h"
#include "util/SysInfo.h"
#include "util/Seccomp.h"
#include "util/CString.h"
#include "util/Bits.h"
#include "util/Defined.h"

#include <stdio.h>

struct SysInfo SysInfo_detect()
{
    struct SysInfo out = { .os = 0 };
    if (Defined(linux)) {
        out.os = SysInfo_Os_LINUX;

    } else if (Defined(sunos)) {
        out.os = SysInfo_Os_SUNOS;

    } else if (Defined(darwin)) {
        out.os = SysInfo_Os_DARWIN;

    } else if (Defined(freebsd)) {
        out.os = SysInfo_Os_FREEBSD;

    } else if (Defined(win32)) {
        out.os = SysInfo_Os_WIN32;

    } else {
        out.os = SysInfo_Os_UNKNOWN;
    }

    out.seccomp = Seccomp_exists();

    return out;
}

char* getName(enum SysInfo_Os os)
{
    switch (os) {
        case SysInfo_Os_LINUX: return "linux";
        case SysInfo_Os_SUNOS: return "sunos";
        case SysInfo_Os_DARWIN: return "darwin";
        case SysInfo_Os_FREEBSD: return "freebsd";
        case SysInfo_Os_WIN32: return "win32";
        default: return "os_unknown";
    }
}

#define BUFF_SZ 1024
char* SysInfo_describe(struct SysInfo si, struct Allocator* alloc)
{
    uint8_t buff[BUFF_SZ];
    snprintf(buff, BUFF_SZ, "%s%s",
             getName(si.os),
             (si.seccomp) ? " +seccomp" : "");

    int len = CString_strlen(buff)+1;
    char* out = Allocator_malloc(alloc, len);
    Bits_memcpy(out, buff, len);
    return out;
}
