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
#ifndef TAPInterface_H
#define TAPInterface_H

#include "exception/Except.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "interface/Interface.h"

struct Interface* TAPInterface_new(const char* preferredName,
                                   char** assignedName,
                                   struct Except* eh,
                                   struct Log* logger,
                                   struct EventBase* base,
                                   struct Allocator* alloc);

#endif
