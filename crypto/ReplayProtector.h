struct ReplayProtector
{
    uint32_t baseOffset;
    uint32_t bitfield;
};

static inline bool ReplayProtector_checkNonce(const uint32_t nonce, struct ReplayProtector* context)
{
    if (nonce < context->baseOffset) {
        return false;
    }

    uint32_t offset = nonce - context->baseOffset;

    if (offset > 20) {
        // Roll it if it's over 19
        offset = 20;
        context->baseOffset += offset - 20;
        context->bitfield <<= offset - 20;
    }

    return ~((context->bitfield &= (1 << offset)) >> offset) & 1;
}
