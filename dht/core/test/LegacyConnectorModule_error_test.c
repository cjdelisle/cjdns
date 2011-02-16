#include "LegacyConnectorModule_framework.c"

int main()
{
    const char* control = "d1:eli201e23:A Generic Error Ocurrede1:y1:e1:t2:aae";

    struct sockaddr_in ipAddr;
    NetworkTools_getPeerAddress("\x7F\x00\x00\x01\x1E\xD3", 6, (struct sockaddr_storage*) &ipAddr);

    struct DHTModuleRegistry* registry = DHTModules_new();
    LegacyConnectorModuleInternal_setContext(
        &(struct LegacyConnectorModule_context) {
            .registry = registry,
            .myId = {20, "abcdefghij0123456789"},
            .whenToCallDHTPeriodic = 0
        }
    );

    struct LegacyConnectorModuleTest_context testContext;
    memset(&testContext, 0, sizeof(testContext));

    struct DHTModule receiver = {
        .name = "TestModule",
        .context = &testContext,
        .handleOutgoing = handleOutgoing
    };

    DHTModules_register(&receiver, registry);

    send_error((struct sockaddr*) &ipAddr, -1,
               (unsigned char*) "aa", 2,
               201,
               "A Generic Error Ocurred");

    printf("error: %s\n\n", testContext.message);

    return memcmp(control, testContext.message, strlen(control));
}
