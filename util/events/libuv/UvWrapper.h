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
#ifndef UvWrapper_H
#define UvWrapper_H

#include "util/Js.h"

#ifndef _GNU_SOURCE
    #define _GNU_SOURCE // CHECKFILES_IGNORE libuv's fault
#endif
#ifdef win32
    #define _WIN32_WINNT 0x0600 // CHECKFILES_IGNORE
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wpedantic"
#endif

#include <uv.h>

#ifdef win32
    #pragma GCC diagnostic pop
#endif

#endif
