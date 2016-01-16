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
#ifndef RouteGen_H
#define RouteGen_H

#include "util/platform/Sockaddr.h"
#include "exception/Except.h"
#include "benc/List.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("tunnel/RouteGen.c");

#include <stdbool.h>

struct RouteGen
{
    bool hasUncommittedChanges;
};

void RouteGen_addException(struct RouteGen* rg, struct Sockaddr* exempt);

void RouteGen_addPrefix(struct RouteGen* rg, struct Sockaddr* destination);

void RouteGen_addLocalPrefix(struct RouteGen* rg, struct Sockaddr* destination);

void RouteGen_commit(struct RouteGen* rg,
                     const char* tunName,
                     struct Allocator* tempAlloc,
                     struct Except* eh);

Dict* RouteGen_getPrefixes(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getLocalPrefixes(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getExceptions(struct RouteGen* rg, struct Allocator* alloc);

Dict* RouteGen_getGeneratedRoutes(struct RouteGen* rg, struct Allocator* alloc);

bool RouteGen_removePrefix(struct RouteGen* rg, struct Sockaddr* toRemove);

bool RouteGen_removeLocalPrefix(struct RouteGen* rg, struct Sockaddr* toRemove);

bool RouteGen_removeException(struct RouteGen* rg, struct Sockaddr* toRemove);

struct RouteGen* RouteGen_new(struct Allocator* allocator, struct Log* log);

#endif
