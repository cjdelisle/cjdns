#include "dht/DHTModules.h"
#include <stdio.h>

struct Context
{
    struct DHTMessage* theMessage;
    int ret;
};

static int handleIncoming(struct DHTMessage* message, void* vcontext)
{
    struct Context* context = (struct Context*) vcontext;
    if (message == context->theMessage) {
        context->ret = 0;
    } else {
        context->ret = -2;
    }
    return 0;
}

int testInputHandler()
{
    struct DHTMessage theMessage;

    struct Context context =
    {
        .theMessage = &theMessage,
        .ret = -1
    };

    struct Context context2 =
    {
        .theMessage = &theMessage,
        .ret = -1
    };

    struct DHTModule module = {
        .name = "TestModule",
        .context = &context,
        .handleIncoming = handleIncoming
    };
    struct DHTModule module2 = {
        .name = "TestModule2",
        .context = &context2,
        .handleIncoming = handleIncoming
    };

    struct DHTModuleRegistry* reg = DHTModules_new();
    DHTModules_register(&module, reg);
    DHTModules_register(&module2, reg);

    DHTModules_handleIncoming(&theMessage, reg);

    /* This should be ignored. */
    DHTModules_handleOutgoing(&theMessage, reg);

    if (context.ret == -1) {
        printf("message not received");
    } else if (context.ret == -2){
        printf("wrong message received");
    } else if (context2.ret == -1) {
        printf("message not received by all handlers.");
    } else if (context2.ret == -2) {
        printf("wrong message received by second handler.");
    } else {
        return 0;
    }
    return -1;
}

int main()
{
    return testInputHandler();
}
