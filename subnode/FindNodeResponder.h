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
#ifndef FindNodeResponder_H
#define FindNodeResponder_H

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "subnode/MsgCore.h"
#include "subnode/BoilerplateResponder.h"
#include "subnode/NodeCache.h"
#include "util/events/EventBase.h"
#include "util/Linker.h"

Linker_require("subnode/FindNodeResponder.c");

struct FindNodeResponder
{
    int unused;
};

struct FindNodeResponder* FindNodeResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct MsgCore* msgCore,
                                                struct EventBase* base,
                                                struct BoilerplateResponder* br,
                                                struct NodeCache* nc);

#endif
