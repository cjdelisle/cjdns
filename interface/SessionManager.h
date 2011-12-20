#ifndef SESSION_MANAGER_H
#define SESSION_MANAGER_H

#include "interface/Interface.h"
#include "memory/MemAllocator.h"

#include <stdint.h>
#include <event2/event.h>

/**
 * Wrap an interface with a crypto session manager which will use different crypto
 * depending on information in the message.
 * The typical use case is discriminating between packets by ip address, keyOffset is the number
 * of bytes between the beginning of each message and the beginning of the ip address and keySize
 * is the number of bytes in the address.
 *
 * @param keySize the number of bytes of information to use for discriminating between sessions.
 * @param keyOffset the number of bytes away from the beginning of the message, if the message has
 *                  padding, this may be negative and the padding will be searched.
 * @param toWrap the interface to wrap.
 * @param eventBase the libevent event base.
 * @param cryptoAuth the cryptoauthenticator for the sessions.
 * @param allocator means of getting memory.
 * @return a wrapper interface.
 */
struct Interface* SessionManager_wrapInterface(uint16_t keySize,
                                               int32_t incomingKeyOffset,
                                               int32_t outgoingKeyOffset,
                                               struct Interface* toWrap,
                                               struct event_base* eventBase,
                                               struct CryptoAuth* cryptoAuth,
                                               struct MemAllocator* allocator);

void SessionManager_setKey(struct Message* message,
                           uint8_t key[32],
                           bool isOutgoingMessage,
                           struct Interface* sessionManagerIface);

struct Interface* SessionManager_getSession(struct Message* message,
                                            bool isOutgoingMessage,
                                            struct Interface* sessionManagerIface);

#endif
