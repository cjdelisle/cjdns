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
#ifndef NanotimeEntropyProvider_H
#define NanotimeEntropyProvider_H

#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("crypto/random/nanotime/NanotimeEntropyProvider.c")
struct NanotimeEntropyProvider;

void NanotimeEntropyProvider_start(struct Random* provideTo,
                                EventBase_t* base,
                                struct Log* logger,
                                struct Allocator* alloc);

Err_DEFUN NanotimeEntropyProvider_newDefaultRandom(
    struct Random** out,
    EventBase_t* base,
    struct Log* logger,
    struct Allocator* alloc);

#endif
