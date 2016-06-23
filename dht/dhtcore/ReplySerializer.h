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
#ifndef ReplySerializer_H
#define ReplySerializer_H

#include "benc/Dict.h"
#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/ReplySerializer.c");

/**
 * For serializing and parsing responses to getPeers and search requests.
 */
struct Address_List* ReplySerializer_parse(struct Address* fromNode,
                                           Dict* result,
                                           struct Log* log,
                                           bool splicePath,
                                           struct Allocator* alloc);

void ReplySerializer_serialize(struct Address_List* addrs,
                               Dict* out,
                               struct Address* convertDirectorFor,
                               struct Allocator* alloc);

#endif
