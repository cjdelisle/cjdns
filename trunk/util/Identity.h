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
#ifndef Identity_H
#define Identity_H

#include "util/Assert.h"
#include "util/Constant.h"
#include "util/Js.h"

#include <stdint.h>

#if defined(Identity_CHECK)

    Js({ this.Identity_hash = "0x" + Constant_randHexString(16) + "ull"; })

    #define Identity_MAGIC ((uintptr_t) Js_or({ return this.Identity_hash }, 1))

    /** This goes in each structure which will be checked. */
    #define Identity uintptr_t Identity_verifier;

    #define Identity_set(pointer) \
        (pointer)->Identity_verifier = Identity_MAGIC

    #define Identity_check(pointer) \
        (__extension__ ({                                                      \
            __typeof__(pointer) Identity_ptr = (pointer);                      \
            Assert_true(Identity_ptr->Identity_verifier == Identity_MAGIC);  \
            Identity_ptr;                                                      \
        }))

    #define Identity_ncheck(pointer) \
        (__extension__ ({                                                                       \
            __typeof__(pointer) Identity_ptr = (pointer);                                       \
            Assert_true(!Identity_ptr || Identity_ptr->Identity_verifier == Identity_MAGIC);  \
            Identity_ptr;                                                                       \
        }))

#else
    #define Identity
    #define Identity_set(pointer)

    #define Identity_check(pointer) \
        (__extension__ ({                                                      \
            (pointer);                                                         \
        }))

    #define Identity_ncheck(pointer) Identity_check(pointer)
#endif

#define Identity_containerOf(ptr, type, member) \
    (__extension__ ({                                                          \
        const __typeof__(((type*)0)->member)*__mptr = (ptr);                   \
        Identity_check( (type*)((char*)__mptr - offsetof(type, member)) );     \
    }))

#endif
