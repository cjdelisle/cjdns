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
#ifndef BencMessageWriter_H
#define BencMessageWriter_H

#include "benc/Dict.h"
#include "exception/Except.h"
#include "wire/Message.h"
#include "util/Linker.h"
Linker_require("benc/serialization/standard/BencMessageWriter.c");

void BencMessageWriter_write(Dict* toWrite, struct Message* msg, struct Except* eh);

#endif
