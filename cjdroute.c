#include "crypto/AddressCalc.h"
#include "crypto/Crypto.h"
#include "crypto/CryptoAuth.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dht/SwitchConnectorModule.h"
#include "exception/ExceptionHandler.h"
#include "exception/AbortHandler.h"
#include "interface/Interface.h"
#include "interface/TUNInterface.h"
#include "interface/UDPInterface.h"
#include "memory/MallocAllocator.h"
#include "memory/MemAllocator.h"
#include "switch/SwitchCore.h"
#include "util/Base32.h"


#include <stdint.h>
#include <assert.h>


struct Context
{
    struct event_base* base;

    struct MemAllocator* allocator;

    char* privateKey;

    struct ExceptionHandler* exceptionHandler;

    struct CryptoAuth* ca;

    struct Interface* tun;

    struct UDPInterface* udpContext;

    struct SwitchCore* switchCore;

    struct DHTModuleRegistry* registry;

    struct RouterModule* routerModule;
};

struct User
{
    uint64_t trust;
};

static void serverFirstIncoming(struct Message* msg, struct Interface* iface)
{
    struct Context* ctx = (struct Context*) iface->receiverContext;

    struct Interface* udpDefault = UDPInterface_getDefaultInterface(ctx->udpContext);
    assert(udpDefault);
    UDPInterface_bindToCurrentEndpoint(udpDefault);

    struct User* u = CryptoAuth_getUser(iface);
    assert(u);
    // Add it to the switch.
    uint64_t discard;
    SwitchCore_addInterface(iface, u->trust, &discard, ctx->switchCore);

    // Prepare for the next connection.
    struct Interface* newUdpDefault = UDPInterface_getDefaultInterface(ctx->udpContext);
    struct Interface* newAuthedUdpDefault =
        CryptoAuth_wrapInterface(newUdpDefault, NULL, true, true, ctx->ca);
    newAuthedUdpDefault->receiveMessage = serverFirstIncoming;
    newAuthedUdpDefault->receiverContext = ctx;

    // Send the message on to the switch so the first message isn't lost.
    iface->receiveMessage(msg, iface);
}

static void udpBindTo(const char* bindAddress, struct Context* ctx)
{
    if (ctx->udpContext != NULL) {
        assert(!"udpBindTo() can only be used once "
                "and must be used before any other udp functions.");
    }
    ctx->udpContext =
        UDPInterface_new(ctx->base, bindAddress, ctx->allocator, ctx->exceptionHandler);
    struct Interface* udpDefault = UDPInterface_getDefaultInterface(ctx->udpContext);
    struct Interface* authedUdpDefault =
        CryptoAuth_wrapInterface(udpDefault, NULL, true, true, ctx->ca);
    authedUdpDefault->receiveMessage = serverFirstIncoming;
    authedUdpDefault->receiverContext = ctx;
}

static void authorizedPassword(char* passwd, uint64_t trust, struct Context* ctx)
{
    struct User* u = ctx->allocator->malloc(sizeof(struct User), ctx->allocator);
    u->trust = trust;
    CryptoAuth_addUser(&(String){.bytes=passwd, .len=strlen(passwd)}, 1, u, ctx->ca);
}
/*
static void udpConnectTo(const char* connectToAddress,
                         const char* key,
                         char* password,
                         const uint64_t trust,
                         struct Context* ctx)
{
    struct Interface* udp =
        UDPInterface_addEndpoint(ctx->udpContext, connectToAddress, ctx->exceptionHandler);
    struct Interface* authedUdp =
        CryptoAuth_wrapInterface(udp, (uint8_t*)key, false, true, ctx->ca);
    CryptoAuth_setAuth(&(String) {.bytes=password, .len=strlen(password)}, 1, authedUdp);

    uint64_t switchAddr_be;
    SwitchCore_addInterface(authedUdp, trust, &switchAddr_be, ctx->switchCore);
    RouterModule_addNode((uint8_t*)key, (uint8_t*)&switchAddr_be, ctx->routerModule);
}*/



int main()
{
    struct Context ctx;
    memset(&ctx, 0, sizeof(struct Context));
    ctx.base = event_base_new();

    // ExceptionHandler which simply aborts the program.
    ctx.exceptionHandler = AbortHandler_INSTANCE;

    // Decide how much memory is allowed to be used and how to get it.
    ctx.allocator = MallocAllocator_new(2000000);

    // Set this to the router's private key.
    ctx.privateKey = 
        "\xc9\x86\xfa\x43\xf9\xc3\x5f\x83\x12\x5a\x23\xbe\x0c\xef\x71\x06"
        "\xf3\x81\xe2\x64\xe6\xca\xe3\x10\x49\x96\x99\x6b\x2f\x01\x04\xcb";

    Crypto_init();
    ctx.switchCore = SwitchCore_new(ctx.allocator);
    ctx.ca = CryptoAuth_new(ctx.allocator, (uint8_t*)ctx.privateKey);
    ctx.registry = DHTModules_new(ctx.allocator);
    ReplyModule_register(ctx.registry, ctx.allocator);
    uint8_t myPubKey[32];
    CryptoAuth_getPublicKey(myPubKey, ctx.ca);
    ctx.routerModule = RouterModule_register(ctx.registry, ctx.allocator, myPubKey, ctx.base);
    SerializationModule_register(ctx.registry, ctx.allocator);

    // Which ip and port to bind to, comment out to avoid listening.
    // This line must be before the first udpConnectTo(), it can also only appear once.
    udpBindTo("127.0.0.1:10001", &ctx);

    // Only nodes connecting with passwords added by this function
    // will be able to connect to the bound port.
    // This may be used any number of times.
    authorizedPassword("password1", 5000, &ctx);

    // You can connect to as many of these as you want.
    // udpConnectTo(<ip:port>, <key>, <passwordToAuthWith>, <trustLevel>, &ctx);
    /*udpConnectTo("127.0.0.1:10001",
                 "yjcbnudh3n5wwrl6vuj8lnl4kfx9xfl9zs5znkpjtgfv0f5pqds0.k",
                 "password1",
                 9000,
                 &ctx);*/

    //ctx.tun = TunInterface_new(NULL, ctx.base, ctx.allocator);

    SwitchConnectorModule_register((uint8_t*)ctx.privateKey,
                                   ctx.registry,
                                   ctx.routerModule,
                                   ctx.switchCore,
                                   ctx.base,
                                   ctx.allocator);

    uint8_t address[53];
    Base32_encode(address, 53, myPubKey, 32);
    printf("Your address is: %s.k\n", address);

    event_base_loop(ctx.base, 0);
}
