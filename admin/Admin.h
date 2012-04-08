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
#ifndef Admin_H
#define Admin_H

#include <event2/event.h>

#include "benc/Dict.h"
#include "exception/ExceptionHandler.h"
#include "memory/Allocator.h"

#define Admin_FUNCTION(name) void (* name)(Dict* input, void* context, String* txid)

struct Admin;

void Admin_registerFunction(char* name,
                            Admin_FUNCTION(callback),
                            void* callbackContext,
                            bool needsAuth,
                            struct Admin* admin);

void Admin_sendMessage(Dict* message, String* txid, struct Admin* admin);

struct Admin* Admin_new(struct sockaddr_storage* addr,
                        int addrLen,
                        String* password,
                        char* user,
                        struct event_base* eventBase,
                        struct ExceptionHandler* eh,
                        struct Allocator* allocator);

void Admin_getConnectInfo(struct sockaddr_storage** addrPtr,
                          int* addrLenPtr,
                          String** passwordPtr,
                          struct Admin* admin);
#endif
