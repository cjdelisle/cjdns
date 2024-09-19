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
#ifndef JsonBencMessageReader_H
#define JsonBencMessageReader_H

#include "benc/Object.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("benc/serialization/json/JsonBencMessageReader.c")

#include <stdbool.h>

Err_DEFUN JsonBencMessageReader_read(
    Dict_t** out,
    Message_t* msg,
    struct Allocator* alloc,
    bool lax
);

Err_DEFUN JsonBencMessageReader_write(
    Dict_t* input,
    Message_t* msg,
    struct Allocator* alloc
);

const char* JsonBencMessageReader_readNoExcept(
    Message_t* msg,
    struct Allocator* alloc,
    Dict** out,
    bool lax);

#endif
