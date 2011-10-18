#ifndef ADDR_PREFIX_H
#define ADDR_PREFIX_H

#include <netinet/in.h>

#define AddrPrefix_get(address) ntohl(*((uint32_t*) address))

#endif
