#include "dht/DHTModules.h"
#include <stdio.h>

int testOutputHandler()
{
    struct DHTMessage theMessage;

    int handleOutgoing(struct DHTMessage* message, void* vcontext) {
        int* context = (int*) vcontext;
        if (message == &theMessage) {
            *context = 0;
        } else {
            *context = -2;
        }
        return 0;
    }

    int context = -1;

    struct DHTModule module = {
        .name = "TestModule",
        .context = &context,
        .handleOutgoing = handleOutgoing
    };

    struct DHTModuleRegistry* reg = DHTModules_new();
    DHTModules_register(&module, reg);

    DHTModules_handleOutgoing(&theMessage, reg);

    /* These should be ignored. */
    DHTModules_handleIncoming(&theMessage, reg);
    DHTModules_compareNodes((benc_bstr_t) {2, "id"},
                            (benc_bstr_t) {3, "id2"},
                            reg);
    DHTModules_free(reg);

    if (context == -1) {
        printf("message not received");
    } else if (context == -2) {
        printf("wrong message received");
    } else {
        return 0;
    }
    return -1;
}

int main()
{
    return testOutputHandler();
}
