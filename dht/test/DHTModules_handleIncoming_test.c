#include "dht/DHTModules.h"
#include <stdio.h>

int testInputHandler()
{
    struct DHTMessage theMessage;

    int handleIncoming(struct DHTMessage* message, void* vcontext) {
        int* context = (int*) vcontext;
        if (message == &theMessage) {
            *context = 0;
        } else {
            *context = -2;
        }
        return 0;
    }

    int context = -1;
    int context2 = -1;

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

    /* These should be ignored. */
    DHTModules_handleOutgoing(&theMessage, reg);
    DHTModules_compareNodes((benc_bstr_t) {2, "id"},
                            (benc_bstr_t) {3, "id2"},
                            reg);
    DHTModules_free(reg);

    if (context == -1) {
        printf("message not received");
    } else if (context == -2){
        printf("wrong message received");
    } else if (context2 == -1) {
        printf("message not received by all handlers.");
    } else if (context2 == -2) {
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
