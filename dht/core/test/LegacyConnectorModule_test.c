/*
#define INCLUDE_CONTEXT
#include "LegacyConnectorModule_framework.c"

static int setUp()
{
    struct DHTModuleRegistry* registry = DHTModules_new();
    LegacyConnectorModuleInternal_setContext(
        &(struct LegacyConnectorModule_context) {
            .registry = registry,
            .myId = {20, "abcdefghij0123456789"},
            .whenToCallDHTPeriodic = 0
        }
    );

    struct event_base* base = event_base_new();
    struct DHTModule* module =
        LegacyConnectorModule_new(base, "abcdefghij0123456789", registry);

    

    LegacyConnectorModule_new(struct event_base* base,
                              char* nodeId,
                              struct DHTModuleRegistry* registry)
}

static int send_ping_test()
{
    const char* control = "d1:q4:ping1:y1:q1:t2:aa1:ad2:id20:abcdefghij0123456789ee";

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

    send_ping((struct sockaddr*) &ipAddr,
                                    -1,
                                    (unsigned char*) "aa",
                                    2);

    printf("\n\nping: %s\n\n", testContext.message);

    return memcmp(control, testContext.message, strlen(control));
}

*/
int main()
{
   /*
    char* id = "\x22\x59\x63\x63\xb3\xde\x40\xb0\x6f\x98\x1f\xb8\x5d\x82\x31\x2e\x8c\x0e\xd5\x11";
    struct event_base* base = event_base_new();
    LegacyConnectorModule_new(base, id, NULL);
    struct timeval twentySec = {20, 0};
    event_base_loopexit(base, &twentySec);
    event_base_dispatch(base);
   */


    return 0;
}
