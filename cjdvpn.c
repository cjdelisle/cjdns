#include "crypto/CryptoAuth.h"
#include "interface/Interface.h"
#include "interface/InterfaceConnector.h"
#include "interface/TUNInterface.h"
#include "interface/UDPInterface.h"
#include "memory/MemAllocator.h"
#include "memory/MallocAllocator.h"
#include "util/Base32.h"

#include <assert.h>
#include <stdio.h>
#include <event2/event.h>

int usage()
{
    printf(
        "NOTE: this is really experimental, it's for testing only\n"
        "      the error handling method is to segfault, you have been warned.\n"
        "cjdvpn s bindToAddress authorizedPassword\n"
        "    Become a server and listen for clients to connect.\n"
        "cjdvpn c connectToAddress serverKey:password\n"
        "    Connect to the specified server and offer the password given.\n"
    );
    return 0;
}

static void serverFirstIncoming(struct Message* msg, struct Interface* iface)
{
    struct Interface** interfaces = iface->receiverContext;
    UDPInterface_bindToCurrentEndpoint(interfaces[0]);
    InterfaceConnector_connect(interfaces[1], interfaces[2]);
    interfaces[2]->sendMessage(msg, interfaces[2]);
}

int startServer(char* bindToAddress,
                char* passwd,
                struct event_base* base,
                struct MemAllocator* allocator,
                struct CryptoAuth* ca)
{
    CryptoAuth_addUser(&(String){.bytes=passwd, .len=strlen(passwd)}, 1, (void*)1, ca);
    struct UDPInterface* udpContext = UDPInterface_new(base, bindToAddress, allocator);
    struct Interface* udpDefault = UDPInterface_getDefaultInterface(udpContext);
    struct Interface* authedUdp = CryptoAuth_wrapInterface(udpDefault, NULL, true, true, ca);
    struct Interface* tun = TunInterface_new(NULL, base, allocator);

    uint8_t pubKey[32];
    CryptoAuth_getPublicKey(pubKey, ca);
    uint8_t base32PubKey[53];
    assert(52 == Base32_encode(base32PubKey, 53, pubKey, 32));
    fprintf(stderr,
            "To connect to this server, use:  cjdvpn c <ip addr> %s:%s",
            base32PubKey, passwd);

    struct Interface* interfaces[3] = {udpDefault, authedUdp, tun};
    authedUdp->receiveMessage = serverFirstIncoming;
    authedUdp->receiverContext = &interfaces;

    event_base_loop(base, 0);
    return 0;
}

int startClient(char* connectToAddress,
                char* keyAndPassword,
                struct event_base* base,
                struct MemAllocator* allocator,
                struct CryptoAuth* ca)
{
    char* passwd = strchr(keyAndPassword, ':');
    if (passwd == NULL) {
        return usage();
    }
    *passwd = '\0';
    passwd += 1;

    uint8_t srvrKey[32];
    if (Base32_decode(srvrKey, 32, (uint8_t*)keyAndPassword, strlen(keyAndPassword)) != 32) {
        fprintf(stderr, "Unparsable server key.");
        return usage();
    }

    struct UDPInterface* udpContext = UDPInterface_new(base, NULL, allocator);
    struct Interface* udp = UDPInterface_addEndpoint(udpContext, connectToAddress);
    struct Interface* authedUdp = CryptoAuth_wrapInterface(udp, srvrKey, true, true, ca);
    CryptoAuth_setAuth(&(String) {.bytes=passwd, .len=strlen(passwd)}, 1, authedUdp);
    struct Interface* tun = TunInterface_new(NULL, base, allocator);
    InterfaceConnector_connect(authedUdp, tun);

    event_base_loop(base, 0);
    return 0;
}

int main(int argc, char** argv)
{
    if (argc < 4) {
        return usage();
    }

    struct event_base* base = event_base_new();
    struct MemAllocator* allocator = MallocAllocator_new(2000000);
    struct CryptoAuth* ca = CryptoAuth_new(allocator, NULL);

    switch (argv[1][0]) {
        case 's': return startServer(argv[2], argv[3], base, allocator, ca);
        case 'c': return startClient(argv[2], argv[3], base, allocator, ca);
        default: return usage();
    };

    return 0;
}
