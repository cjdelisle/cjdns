



struct Context
{
    struct event_base* base;

    struct MemAllocator* allocator;

    char* privateKey;

    struct ExceptionHandler* exceptionHandler;

    struct CryptoAuth* ca;

    struct Interface* tun;

    struct UDPInterface* udpContext;
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
    SwitchCore_addInterface(ctx->switchCore, u, iface);

    // Prepare for the next connection.
    struct Interface* newUdpDefault = UDPInterface_getDefaultInterface(ctx->udpContext);
    struct Interface* newAuthedUdpDefault =
        CryptoAuth_wrapInterface(newUdpDefault, NULL, true, true, ca);
    newAuthedUdpDefault->receiveMessage = serverFirstIncoming;
    newAuthedUdpDefault->receiverContext = ctx;

    // Send the message on to the switch so the first message isn't lost.
    iface->receiveMessage(msg, iface);
}

static void udpBindTo(const char* bindAddress, struct Context* ctx)
{
    if (context->udpContext != NULL) {
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

static void checkUdp(struct Context* ctx)
{
    if (ctx->udpContext == NULL) {
        ctx->udpContext =
            UDPInterface_new(ctx->base, NULL, ctx->allocator, ctx->exceptionHandler);
    }
}

static void checkSwitch()
{
    if (ctx.switchCore == NULL) {
        ctx.switchCore = SwitchCore_new(ctx.allocator);
    }
}

static void checkCa(struct Context* context)
{
    if (context->ca == NULL) {
        context->ca = CryptoAuth_new(context->allocator, contrext->privateKey);
    }
}

static void authorizedPassword(const char* password, uint64_t trust, struct Context* ctx)
{
    checkCa(context);
    struct User* u = ctx->allocator->malloc(sizeof(User), ctx->allocator);
    u->trust = trust;
    CryptoAuth_addUser(&(String){.bytes=passwd, .len=strlen(passwd)}, 1, u, ctx->ca);
}

static void udpConnectTo(const char* connectToAddress,
                         const char* key,
                         const char* password,
                         const uint64_t trust,
                         struct Context* ctx)
{
    checkUdp(ctx);
    struct Interface* udp =
        UDPInterface_addEndpoint(ctx->udpContext, connectToAddress, ctx->exceptionHandler);
    struct Interface* authedUdp = CryptoAuth_wrapInterface(udp, key, false, true, ctx->ca);
    CryptoAuth_setAuth(&(String) {.bytes=password, .len=strlen(password)}, 1, authedUdp);
    checkSwitch(ctx);
    SwitchCore_addInterface(authedUdp, trust, ctx->switchCore);
}

static int start(struct Context* ctx)
{
    struct DHTModuleRegistry* registry = DHTModules_new(ctx->allocator);
    ReplyModule_register(registry, ctx->allocator);
    struct RouterModule* router =
        RouterModule_register(registry, ctx->allocator, (uint8_t*) id, eventBase);
    SwitchCore_setRouterInterface(struct Interface* iface, struct SwitchCore* core)
}







int main()
{
    struct Context ctx = {NULL};
    ctx.base = event_base_new();

    // ExceptionHandler which simply aborts the program.
    ctx.exceptionHandler = AbortHandler_INSTANCE;

    // Decide how much memory is allowed to be used and how to get it.
    ctx.allocator = MallocAllocator_new(2000000);

    // Set this to the router's private key.
    ctx.privateKey = 
        "\xc9\x86\xfa\x43\xf9\xc3\x5f\x83\x12\x5a\x23\xbe\x0c\xef\x71\x06"
        "\xf3\x81\xe2\x64\xe6\xca\xe3\x10\x49\x96\x99\x6b\x2f\x01\x04\xca";

    // Which ip and port to bind to, comment out to avoid listening.
    // This line must be before the first udpConnectTo(), it can also only appear once.
    udpBindTo("192.168.0.1:10000", &ctx);

    // Only nodes connecting with passwords added by this function
    // will be able to connect to the bound port.
    // This may be used any number of times.
    authorizedPassword("reallyreallysecret", 5000, &ctx);

    // You can connect to as many of these as you want.
    // udpConnectTo(<ip:port>, <key>, <passwordToAuthWith>, <trustLevel>, &ctx);
    udpConnectTo("192.168.0.2:3300",
                 "fssswyd31rgrugvppm1l7l5532bzhm4kfccgdj26zhpyxk29dnm0.k",
                 "password1",
                 9000,
                 &ctx);

    ctx.tun = TunInterface_new(NULL, ctx.base, ctx.allocator);


    return start(&ctx);
}
