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
#ifndef Router_H
#define Router_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "dht/dhtcore/Node.h"
#include "util/Linker.h"
Linker_require("dht/dhtcore/Router.c");

/**
 * This is a facad around the other internals of dht/dhtcore.
 * NO OTHER FILE IN dht/dhtcore SHOULD EVER INCLUDE THIS HEADER.
 * Files outside of dht/dhtcore should use this instead of the others.
 */
struct Router
{
    int unused;
};

void Router_sendGetPeers(struct Router* r,
                         struct Address* addr,
                         uint64_t nearbyLabel,
                         uint32_t timeoutMilliseconds,
                         struct Allocator* alloc);

struct Node_Two* Router_lookup(struct Router* r, uint8_t targetAddr[16]);

//void Router_brokenLink(struct Router* r, uint64_t path, uint64_t labelAtErrorHop);

//void Router_disconnectedPeer(struct Router* r, uint64_t path);

struct Node_Link* Router_linkForPath(struct Router* r, uint64_t path);

#if defined(NodeStore_H) && defined(SearchRunner_H) && defined(RouterModule_H)
struct Router* Router_new(struct RouterModule* rm,
                          struct NodeStore* ns,
                          struct SearchRunner* sr,
                          struct Allocator* alloc);
#endif

#endif
