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
#ifndef VersionList_H
#define VersionList_H

#include "benc/String.h"
#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/VersionList.c");

#include <stdint.h>

struct VersionList
{
    uint32_t length;
    struct Allocator* alloc;
    uint32_t* versions;
};

struct VersionList* VersionList_parse(String* str, struct Allocator* alloc);

String* VersionList_stringify(struct VersionList* list, struct Allocator* alloc);

struct VersionList* VersionList_new(uint32_t count, struct Allocator* alloc);


#endif
