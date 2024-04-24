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
#ifndef Pipe_H
#define Pipe_H

#include "exception/Except.h"
#include "interface/Iface.h"
#include "memory/Allocator.h"
#include "util/events/EventBase.h"
#include "util/log/Log.h"
#include "util/Linker.h"

#define Pipe_PADDING_AMOUNT 512
#define Pipe_BUFFER_CAP 4000

#ifdef win32
    #define Pipe_PATH_SEP "\\"
#else
    #define Pipe_PATH_SEP "/"
#endif

#ifndef Pipe_PATH
    #ifdef win32
        #define Pipe_PATH "\\\\.\\pipe"
    #elif defined(Cjdns_android)
        #define Pipe_PATH "/data/local/tmp"
    #else
        #define Pipe_PATH "/tmp"
    #endif
#endif

#endif
