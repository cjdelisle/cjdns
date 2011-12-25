#include "interface/Interface.h"
#include "wire/Message.h"


static uint8_t transferMessage(struct Message* msg, struct Interface* iface)
{
    struct Interface* other = (struct Interface*) iface->receiverContext;
    return other->sendMessage(msg, other);
}

/**
 * Connect two interfaces together like a double female adapter.
 * Any traffic coming in on one will be forwarded to the other.
 *
 * @param a one interface.
 * @param b another interface.
 */
void InterfaceConnector_connect(struct Interface* a, struct Interface* b)
{
    a->receiveMessage = transferMessage;
    a->receiverContext = b;
    b->receiveMessage = transferMessage;
    b->receiverContext = a;
}
