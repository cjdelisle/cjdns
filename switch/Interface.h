struct Interface
{
    struct SwitchCore* core;

    uint8_t (* sendMessage)(struct Message* toSend, void* callbackContext);

    void* callbackContext;

    /** How much traffic has flowed down an interface. */
    int64_t buffer;

    /** How congested an interface is. */
    uint32_t congestion;
};
