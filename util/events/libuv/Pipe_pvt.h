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
#ifndef Pipe_pvt_H
#define Pipe_pvt_H

#include "util/events/libuv/UvWrapper.h"
#include "util/events/Pipe.h"
#include "exception/Er.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/Pipe.c");

#include <stdbool.h>

Er_DEFUN(struct Pipe* Pipe_serverAccept(uv_pipe_t* server,
                               const char* pipeName,
                               struct EventBase* eb,
                               struct Log* log,
                               struct Allocator* userAlloc));
#endif