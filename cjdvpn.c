#include <stdio.h>
#include <event2/event.h>

#include "crypto/CryptoAuth.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/UDPInterface.h"
#include "memory/MemAllocator.h"
#include "memory/MallocAllocator.h"

static void receiveMessageUDP(struct Message* msg, struct Interface* iface)
{
    iface=iface;
    fprintf(stderr, "UDP message received! %s\n", msg->bytes);
}

static void receiveMessageTUN(struct Message* msg, struct Interface* iface)
{
    iface=iface;
    fprintf(stderr, "TUN message received! %s\n", msg->bytes);
}

int main(int argc, char** argv)
{
argv=argv;
    printf("argc = %d", argc);

    struct event_base* base = event_base_new();
    struct MemAllocator* allocator = MallocAllocator_new(2000000);

    struct UDPInterface* udpCtx = UDPInterface_new(base, "127.0.0.1:3333", allocator);
    struct Interface* udpDefault = UDPInterface_getDefaultInterface(udpCtx);
    
    udpDefault->receiveMessage = receiveMessageUDP;

    struct Interface* tun = TunInterface_new(NULL, base, allocator);
    tun->receiveMessage = receiveMessageTUN;

    event_base_loop(base, 0);
    return 0;
}
