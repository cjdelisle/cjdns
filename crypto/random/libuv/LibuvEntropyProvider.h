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
#ifndef LibuvEntropyProvider_H
#define LibuvEntropyProvider_H

#include "crypto/random/Random.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("crypto/random/libuv/LibuvEntropyProvider.c");

struct LibuvEntropyProvider;

void LibuvEntropyProvider_start(struct Random* provideTo,
                                struct EventBase* base,
                                struct Log* logger,
                                struct Allocator* alloc);

struct Random* LibuvEntropyProvider_newDefaultRandom(struct EventBase* base,
                                                     struct Log* logger,
                                                     struct Except* eh,
                                                     struct Allocator* alloc);

#endif
