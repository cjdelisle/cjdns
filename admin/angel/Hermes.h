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
#ifndef Hermes_H
#define Hermes_H

#include "benc/Dict.h"
#include "memory/Allocator.h"
#include "exception/Except.h"
#include "interface/Interface.h"
#include "util/log/Log.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("admin/angel/Hermes.c")

/**
 * Hermes is the god of transitions and boundaries, moving freely between
 * the worlds of the mortal and divine, as messenger of the gods.
 * IE: The communications pipeline between the core and the angel.
 * Hermes is used by the core to make admin API calls against the angel.
 */
struct Hermes;

typedef void (* Hermes_onResponse)(Dict* responseMessage, void* context);

/**
 * Make a call to the angel and get the response.
 *
 * @param message the message to send, the txid key is reserved and will
 *                be overwritten.
 * @param onResponse a callback which will be called with the response
 *                   from the angel. This callback will also be called if
 *                   there is no response and the request times out.
 * @param onResponseContext a pointer which will be provided to onResponse.
 * @param alloc an allocator for the request, if this allocator is freed
 *              before the response happens, the onResponse will not be
 *              called, this prevents a race condition when the structure
 *              pointed to be context is freed after a call. The allocator
 *              need not be freed after the request is complete.
 * @param eh an exception handler.
 * @param hermes the Hermes.
 */
void Hermes_callAngel(Dict* message,
                      Hermes_onResponse onResponse,
                      void* onResponseContext,
                      struct Allocator* alloc,
                      struct Except* eh,
                      struct Hermes* hermes);


struct Hermes* Hermes_new(struct Interface* angelIface,
                          struct EventBase* eventBase,
                          struct Log* logger,
                          struct Allocator* alloc);

#endif
