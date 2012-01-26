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
#ifndef ADMIN_H
#define ADMIN_H

#include <event2/event.h>

#include "benc/Dict.h"
#include "exception/ExceptionHandler.h"
#include "memory/Allocator.h"

#define Admin_FUNCTION(name) void (* name)(Dict* input, void* context, struct Allocator* alloc)

struct Admin;

void Admin_registerFunction(char* name,
                            Admin_FUNCTION(callback),
                            void* callbackContext,
                            struct Admin* admin);

void Admin_sendMessage(Dict* message, struct Admin* admin);

struct Admin* Admin_new(Dict* config,
                        struct event_base** eventBase,
                        struct ExceptionHandler* eh,
                        struct Allocator* allocator);
#endif
