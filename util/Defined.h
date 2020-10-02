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
#ifndef Defined_H
#define Defined_H

#include "util/Js.h"

/**
 * Preprocesses to a 1 if the macro is defined, a 0 if it is not.
 * Example:
 *
 * if (Defined(linux)) {
 *     doLinuxSpecificStuff();
 * }
 */
#define Defined(macro) \
    Js_or({ return ( Js_Q macro Js_Q === ' ' + #macro + ' ' ) ? '0' : '1'; }, 1)

#endif
