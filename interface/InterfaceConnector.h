#ifndef INTERFACE_CONNECTOR_H
#define INTERFACE_CONNECTOR_H

/**
 * Connect two interfaces together like a double female adapter.
 * Any traffic coming in on one will be forwarded to the other.
 *
 * @param a one interface.
 * @param b another interface.
 */
void InterfaceConnector_connect(struct Interface* a, struct Interface* b);

#endif
