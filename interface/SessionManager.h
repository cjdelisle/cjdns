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
#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "interface/Interface.h"
#include "memory/MemAllocator.h"

#include <stdint.h>
#include <event2/event.h>

struct SessionManager;

/**
 * Create a new session manager for keeping track of and expiring crypto sessions.
 * The typical use case is discriminating between packets by ip address, keyOffset is the number
 * of bytes between the beginning of each message and the beginning of the ip address and keySize
 * is the number of bytes in the address.
 *
 * @param keySize the number of bytes of information to use for discriminating between sessions.
 * @param decryptedIncoming the callback to call with incoming data after it has been decrypted.
 * @param encryptedOutgoing the callback to call with outgoing data after it has been encrypted.
 * @param interfaceContext the context which will become the senderContext and receiverContext for
 *                         encryptedOutgoing and decryptedIncoming respectively.
 * @param eventBase the libevent event base.
 * @param cryptoAuth the cryptoauthenticator for the sessions.
 * @param allocator means of getting memory.
 * @return a session manager.
 */
struct SessionManager* SessionManager_new(uint16_t keySize,
                                          Interface_CALLBACK(decryptedIncoming),
                                          Interface_CALLBACK(encryptedOutgoing),
                                          void* interfaceContext,
                                          struct event_base* eventBase,
                                          struct CryptoAuth* cryptoAuth,
                                          struct MemAllocator* allocator);

/**
 * Get a session from the session manager.
 * If there is no session for the lookup key, it will be created.
 *
 * @param lookupKey this must be the size given by keySize in SessionManager_new().
 * @param cryptoKey optional encryption key if it is known, otherwise NULL.
 * @param sm the session manager.
 */
struct Interface* SessionManager_getSession(uint8_t* lookupKey,
                                            uint8_t cryptoKey[32],
                                            struct SessionManager* sm);

#endif
