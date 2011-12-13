#ifndef SESSION_H
#define SESSION_H

#include <stdint.h>
#include <stdbool.h>

struct Session
{
    /** The symmetrical key. */
    uint8_t sharedSecret[32];

    /** the next nonce to use when sending. */
    uint32_t nextNonce;

    /** true if this node initiated the session. */
    bool isInitiator : 1;

    /** true if this is a session and not just empty space. */
    bool exists : 1;
};

#endif
