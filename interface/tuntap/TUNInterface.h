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
#ifndef TUNInterface_H
#define TUNInterface_H

#include "exception/Err.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("interface/tuntap/TUNInterface.c")

/**
 * This is the maximum size that will be accepted as an interface name.
 * If this runs on a system where IFNAMSIZ is less then this, names larger
 * than that will also be rejected.
 */
#ifdef win32
    #define TUNInterface_IFNAMSIZ 256
#else
    #define TUNInterface_IFNAMSIZ 16
#endif

typedef struct TUNInterface {
    Iface_t* iface;
} TUNInterface_t;

/**
 * Create a new TUNInterface.
 *
 * @param interfaceName the interface name you *want* to use or NULL to let the kernel decide.
 * @param assignedInterfaceName an empty buffer which will be filled in with the interface
 *                              name that is assigned.
 * @param logger for logging messages about the tun device.
 * @param allocator a means of getting memory.
 * @return a Interface.
 */
Err_DEFUN TUNInterface_new(
    TUNInterface_t** out,
    const char* interfaceName,
    char assignedInterfaceName[TUNInterface_IFNAMSIZ],
    struct Log* logger,
    struct Allocator* alloc);

Err_DEFUN TUNInterface_workerStates(
    Object_t** out,
    TUNInterface_t* tt,
    Allocator_t* alloc);

Err_DEFUN TUNInterface_forFd(
    TUNInterface_t** out,
    int fd,
    struct Allocator* alloc);

#endif
