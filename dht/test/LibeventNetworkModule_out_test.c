#include <net/NetworkTools.h>
#include "dht/LibeventNetworkModule.c"

struct LibeventNetworkModuleTest_context {
    struct event_base* eventBase;
    char message[50];
};

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    struct LibeventNetworkModuleTest_context* context =
        (struct LibeventNetworkModuleTest_context*) vcontext;

    memcpy(context->message, message->bytes, message->length);
    /* A success will cause libevent to be stopped. */
    event_base_loopbreak(context->eventBase);
    return 0;
}

static int testOutgoing()
{
    struct LibeventNetworkModuleTest_context* context =
        calloc(sizeof(struct LibeventNetworkModuleTest_context), 1);

    const char* passThis = "Hello world, I was passed through a socket.";

    struct DHTModule* receiver = calloc(sizeof(struct DHTModule), 1);
    struct DHTModule localReceiver = {
        .name = "TestModule",
        .context = context,
        .handleIncoming = handleIncoming
    };
    memcpy(receiver, &localReceiver, sizeof(struct DHTModule));

    struct DHTModuleRegistry* reg = DHTModules_new();

    DHTModules_register(receiver, reg);

    struct event_base* base = event_base_new();
    context->eventBase = base;

    evutil_socket_t socket = NetworkTools_bindSocket("127.0.0.1:7891");
    LibeventNetworkModule_register(base, socket, 6, reg);

    struct DHTModuleRegistry* reg2 = DHTModules_new();
    evutil_socket_t socket2 = NetworkTools_bindSocket("127.0.0.1:7890");
    LibeventNetworkModule_register(base, socket2, 6, reg2);

    struct DHTMessage message;
    memcpy(message.peerAddress, "\x7F\x00\x00\x01\x1E\xD3", 6);
    message.addressLength = 6;
    memcpy(message.bytes, passThis, strlen(passThis));
    message.length = strlen(passThis);

    DHTModules_handleOutgoing(&message, reg2);

    /** If the test fails, this should break eventually. */
    struct timeval twoSec = {2, 0};
    event_base_loopexit(base, &twoSec);

    event_base_dispatch(base);


    DHTModules_free(reg);
    DHTModules_free(reg2);
    EVUTIL_CLOSESOCKET(socket);
    EVUTIL_CLOSESOCKET(socket2);

    return memcmp(context->message, passThis, strlen(passThis));
}

int main()
{
    return testOutgoing();
}
