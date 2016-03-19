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

enum FileNo_Type {
    /** Normal tunfd type, no need other wrapper */
    FileNo_Type_NORMAL = 0,

    /** Android tunfd, need AndroidWrapper */
    FileNo_Type_ANDROID
};

struct FileNo_Promise;
struct FileNo_Promise
{
    void (* callback)(struct FileNo_Promise* promise,
                      int fileno);
    void* userData;
    struct Allocator* alloc;
};

#define FileNo_PADDING_AMOUNT   512
#define FileNo_BUFFER_CAP       4000

struct FileNo_Promise* FileNo_import(const char* path,
                                     struct EventBase* eb,
                                     struct Except* eh,
                                     struct Log* logger,
                                     struct Allocator* alloc);

#endif
