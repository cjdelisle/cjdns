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
#ifndef FileNo_H
#define FileNo_H

#include "memory/Allocator.h"
#include "exception/Except.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"
#include "util/log/Log.h"
Linker_require("util/events/libuv/FileNo.c");

struct FileNo;

enum FileNo_Type {
    /** Normal tunfd type, no need other wrapper */
    FileNo_Type_NORMAL = 0,

    /** Android tunfd, need AndroidWrapper */
    FileNo_Type_ANDROID
};

typedef void (* FileNo_callback)(void* context, enum FileNo_Type type, int fileno);

struct FileNo
{
    /** The name of the file eg: "/tmp/cjdns_fileno_foo" */
    const char* const pipePath;

    void* userData;

    enum FileNo_Type type;

    struct EventBase* const base;

    FileNo_callback onFileNoReceived;

    struct Log* logger;
};

#define FileNo_PADDING_AMOUNT   512
#define FileNo_BUFFER_CAP       4000

struct FileNo* FileNo_new(const char* path,
                          struct EventBase* eb,
                          struct Except* eh,
                          struct Log* logger,
                          struct Allocator* userAlloc,
                          FileNo_callback recv_cb);

#endif
