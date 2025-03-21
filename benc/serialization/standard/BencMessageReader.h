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
#ifndef BencMessageReader_H
#define BencMessageReader_H

#include "benc/Object.h"
#include "exception/Err.h"
#include "memory/Allocator.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("benc/serialization/standard/BencMessageReader.c")

Err_DEFUN BencMessageReader_read(Dict** outP, Message_t* msg, struct Allocator* alloc);

const char* BencMessageReader_readNoExcept(
    Message_t* msg, struct Allocator* alloc, Dict** outPtr);

#endif
