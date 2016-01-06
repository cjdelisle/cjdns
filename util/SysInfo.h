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
#ifndef SysInfo_H
#define SysInfo_H

#include "memory/Allocator.h"
#include "util/Assert.h"

#include "util/Linker.h"
Linker_require("util/SysInfo.c");

#include <stdint.h>

enum SysInfo_Os
{
    SysInfo_Os_UNKNOWN,
    SysInfo_Os_LINUX,
    SysInfo_Os_SUNOS,
    SysInfo_Os_DARWIN,
    SysInfo_Os_FREEBSD,
    SysInfo_Os_WIN32
};

struct SysInfo
{
    int seccomp : 1;
    int os : 4;
};
Assert_compileTime(sizeof(struct SysInfo) == 4);

struct SysInfo SysInfo_detect();

char* SysInfo_describe(struct SysInfo si, struct Allocator* alloc);

#endif
