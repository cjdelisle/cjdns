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
#ifndef Identity_H
#define Identity_H

#include "util/Assert.h"

#ifdef Identity_CHECK

    /** This is unique to each file. */
    static char Identity_ID;

    /** This goes in each structure which will be checked. */
    #define Identity \
        char* Identity_verifier;

    #define Identity_set(pointer) \
        pointer->Identity_verifier = &Identity_ID

    #define Identity_check(pointer) \
        Assert_always((pointer)->Identity_verifier == &Identity_ID)

    #define Identity_cast(pointer) \
        (pointer); Identity_check(pointer)

#else
    #define Identity
    #define Identity_set(pointer)
    #define Identity_check(pointer)
    #define Identity_cast(pointer) (pointer)
#endif

#endif
