#ifndef REPLAY_PROTECTOR_H
#define REPLAY_PROTECTOR_H

#include <stdint.h>
#include <stdbool.h>

struct ReplayProtector
{
    uint32_t baseOffset;
    uint32_t bitfield;
};

/**
 * Check a nonce and file it as being seen.
 * Don't call this until the packet has been authenticated or else forged packets will
 * make legit ones appear to be duplicates.
 *
 * @param nonce the number to check, this should be a counter nonce as numbers less than 20 minus
 *              the highest seen nonce will be dropped erroniously.
 * @param context the context
 */
static inline bool ReplayProtector_checkNonce(const uint32_t nonce, struct ReplayProtector* context)
{
    if (nonce < context->baseOffset) {
        return false;
    }

    uint32_t offset = nonce - context->baseOffset;

    if (offset > 20) {
        context->baseOffset += offset - 20;
        context->bitfield = context->bitfield >> (offset - 20) | (1 << 20);
        return true;
    }

    if (context->bitfield & (1 << offset)) {
        return false;
    }
    context->bitfield |= (1 << offset);
    return true;
}

#endif
