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
#ifndef Base10_H
#define Base10_H

#include "exception/Except.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("util/Base10.c");

#include <stdint.h>

void Base10_write(struct Message* msg, int64_t num, struct Except* eh);
int64_t Base10_read(struct Message* msg, struct Except* eh);
int Base10_fromString(uint8_t* str, int64_t* numOut);

#endif
