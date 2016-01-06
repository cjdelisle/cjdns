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
#ifndef Process_H
#define Process_H

#include "memory/Allocator.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/Process.c");

#include <stdint.h>

typedef void (* Process_OnExitCallback)(int64_t exit_status, int term_signal);

/**
 * Spawn a new process.
 *
 * @param binaryPath the path to the file to execute.
 * @param args a list of strings representing the arguments to the command followed by NULL.
 * @param base the event base.
 * @param alloc an allocator. The process to be killed when it is freed.
 * @param callback a function to be called when the spawn process exits.
 * @return 0 if all went well, -1 if forking fails.
 */
int Process_spawn(char* binaryPath,
                  char** args,
                  struct EventBase* base,
                  struct Allocator* alloc,
                  Process_OnExitCallback callback);

/**
 * Get the path to the binary of the current process.
 *
 * @param alloc an allocator.
 * @return the binary path to the process or null if there was a failure.
 */
char* Process_getPath(struct Allocator* alloc);

#endif
