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
#ifndef GetEntropyRandomSeed_H
#define GetEntropyRandomSeed_H

#include "crypto/random/seed/RandomSeed.h"
#include "crypto/random/seed/RandomSeedProvider.h"
#include "memory/Allocator.h"
#include "util/Linker.h"

#ifndef _WIN32
#include <sys/syscall.h>

#if defined(__OPENBSD__)
    #define GetEntropyRandomSeed_USEIT
#elif defined(SYS_getrandom)
    #if SYS_getrandom == __NR_getrandom && !defined(__NR_getrandom)
        // nope, SYS_getrandom points to __NR_getrandom which is undefined :(
    #else
        #define GetEntropyRandomSeed_USEIT
    #endif
#endif

#ifdef GetEntropyRandomSeed_USEIT
    Linker_require("crypto/random/seed/GetEntropyRandomSeed.c");
    struct RandomSeed* GetEntropyRandomSeed_new(struct Allocator* alloc);
    RandomSeedProvider_register(GetEntropyRandomSeed_new)
#endif

#endif
#endif
