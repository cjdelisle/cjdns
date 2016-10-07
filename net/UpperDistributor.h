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
#ifndef UpperDistributor_H
#define UpperDistributor_H

#include "dht/Address.h"
#include "memory/Allocator.h"
#include "net/EventEmitter.h"
#include "util/log/Log.h"
#include "util/Linker.h"
Linker_require("net/UpperDistributor.c");

/**
 * Connects the TUN, DHT and IpTunnel (and other?) handlers to the SessionManager.
 * All packets must have SessionManager_UpperHeader on them.
 */
struct UpperDistributor
{
    struct Iface sessionManagerIf;

    struct Iface tunAdapterIf;

    struct Iface ipTunnelIf;
};

struct UpperDistributor_Handler
{
    enum ContentType type;
    int udpPort;
};

/** If the regNum does not corrispond to an existing handler */
#define UpperDistributor_unregisterHandler_NONEXISTANT -1

/** Returns 0 unless there is an error */
int UpperDistributor_unregisterHandler(struct UpperDistributor* ud, int regNum);

/**
 * Returns the number of elements in the list.
 * If there are no elements, outputList is set to NULL.
 */
int UpperDistributor_listHandlers(struct UpperDistributor* ud,
                                  struct UpperDistributor_Handler** outputList,
                                  struct Allocator* alloc);

/** If the port has already been registered to a different contentType */
#define UpperDistributor_registerHandler_PORT_REGISTERED -1

/**
 * Register a handler for receiving messages of a given contentType.
 * @return 0 unless there is an error.
 */
int UpperDistributor_registerHandler(struct UpperDistributor* ud,
                                     enum ContentType ct,
                                     int udpPort);

struct UpperDistributor* UpperDistributor_new(struct Allocator* alloc,
                                              struct Log* log,
                                              struct EventEmitter* ee,
                                              struct Address* myAddress);

#endif
