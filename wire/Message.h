struct Message
{
    /** The length of the message. */
    uint16_t length;

    /** The content. */
    uint8_t* bytes;

    struct MemAllocator* allocator;
};
