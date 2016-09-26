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
#ifndef DHTModule_H
#define DHTModule_H

#ifdef SUBNODE
    #error "this file should not be included in subnode"
#endif

#include "dht/DHTMessage.h"

/**
 * This represents a DHT module.
 * Pass one of these to DHTModule_register() and it
 * will handle dht requests and responses.
 */
struct DHTModule;
struct DHTModule {
    /**
     * A user friendly null terminated string which will be used to
     * manipulate the module using the DHTModules API.
     */
    const char* const name;

    /**
     * The module's state.
     */
    void* context;

    /**
     * @param the message which came in from a peer.
     * @param context the module's state.
     * @return 1 if a response should be sent for this message.
     *         -1 if the message is known invalid and should not be passed
     *            to any more handlers.
     */
    int (* handleIncoming)(struct DHTMessage* message, void* context);

    /**
     * @param message the message which will be sent to the peer.
     * @param context the module's state.
     * @return -1 if the message should not be propigated to any more modules.
     *            use with caution as it may be interpreted as network loss.
     */
    int (* handleOutgoing)(struct DHTMessage* message, void* context);
};

#endif
