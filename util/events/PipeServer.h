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
#ifndef PipeServer_H
#define PipeServer_H

#include "memory/Allocator.h"
#include "exception/Except.h"
#include "interface/addressable/AddrIface.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
Linker_require("util/events/libuv/PipeServer.c");

#include <stdbool.h>

struct PipeServer;
typedef void (* PipeServer_callback)(struct PipeServer* p, struct Sockaddr* client);

struct PipeServer
{
    struct AddrIface iface;

    /** The name of the file eg: "/tmp/cjdns_pipe_foo" */
    const char* const fullName;

    void* userData;

    PipeServer_callback onConnection;
    PipeServer_callback onDisconnection;
};

struct PipeServer* PipeServer_named(const char* fullPath,
                                    struct EventBase* eb,
                                    struct Except* eh,
                                    struct Log* log,
                                    struct Allocator* userAlloc);

#endif
