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
#ifndef GetPeersResponder_H
#define GetPeersResponder_H

#include "memory/Allocator.h"
#include "util/log/Log.h"
#include "subnode/AddrSet.h"
#include "subnode/MsgCore.h"
#include "subnode/BoilerplateResponder.h"
#include "switch/EncodingScheme.h"
#include "util/Linker.h"

Linker_require("subnode/GetPeersResponder.c");

struct GetPeersResponder
{
    int unused;
};

struct GetPeersResponder* GetPeersResponder_new(struct Allocator* allocator,
                                                struct Log* log,
                                                struct AddrSet* peers,
                                                struct Address* selfAddr,
                                                struct MsgCore* msgCore,
                                                struct BoilerplateResponder* br,
                                                struct EncodingScheme* myScheme);

#endif
