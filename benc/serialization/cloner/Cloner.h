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
#ifndef Cloner_H
#define Cloner_H

#include "memory/Allocator.h"
#include "benc/List.h"
#include "benc/Dict.h"
#include "benc/String.h"
#include "util/Linker.h"
Linker_require("benc/serialization/cloner/Cloner.c");

Dict* Cloner_cloneDict(Dict* orig, struct Allocator* alloc);

List* Cloner_cloneList(List* orig, struct Allocator* alloc);

static inline String* Cloner_cloneString(String* orig, struct Allocator* alloc)
{
    return String_clone(orig, alloc);
}

#endif
