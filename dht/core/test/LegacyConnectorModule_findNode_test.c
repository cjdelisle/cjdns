#include "LegacyConnectorModule_framework.c"

int main()
{
    const char* control =
        "d1:q9:find_node1:y1:q1:t2:aa1:ad2:id20:mnopqrstuvwxyz123456"
        "6:target20:mnopqrstuvwxyz123456ee";

    const char* control4 =
        "d4:wantl2:n4e1:q9:find_node1:y1:q1:t2:aa1:ad2:id20:mnopqrstuvwxyz123456"
        "6:target20:mnopqrstuvwxyz123456ee";

    const char* control6 =
        "d4:wantl2:n6e1:q9:find_node1:y1:q1:t2:aa1:ad2:id20:mnopqrstuvwxyz123456"
        "6:target20:mnopqrstuvwxyz123456ee";

    const char* control46 =
        "d4:wantl2:n62:n4e1:q9:find_node1:y1:q1:t2:aa1:ad2:id20:mnopqrstuvwxyz123456"
        "6:target20:mnopqrstuvwxyz123456ee";

    struct sockaddr_in ipAddr;
    NetworkTools_getPeerAddress("\x7F\x00\x00\x01\x1E\xD3", 6, (struct sockaddr_storage*) &ipAddr);

    struct DHTModuleRegistry* registry = DHTModules_new();
    LegacyConnectorModuleInternal_setContext(
        &(struct LegacyConnectorModule_context) {
            .registry = registry,
            .myId = {20, "mnopqrstuvwxyz123456"},
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

    int out = 0;

    send_find_node((struct sockaddr*) &ipAddr, -1,
                                         (unsigned char*) "aa", 2,
                                         (unsigned char*) "mnopqrstuvwxyz123456",
                                         0,
                                         0);

    printf("\n\nfind_node:    %s\n", testContext.message);
    out |= memcmp(control, testContext.message, strlen(control));

    send_find_node((struct sockaddr*) &ipAddr, -1,
                                         (unsigned char*) "aa", 2,
                                         (unsigned char*) "mnopqrstuvwxyz123456",
                                         WANT4,
                                         0);

    printf("find_node4:   %s\n", testContext.message);
    out |= memcmp(control4, testContext.message, strlen(control));

    send_find_node((struct sockaddr*) &ipAddr, -1,
                                         (unsigned char*) "aa", 2,
                                         (unsigned char*) "mnopqrstuvwxyz123456",
                                         WANT6,
                                         0);

    printf("find_node6:   %s\n", testContext.message);
    out |= memcmp(control6, testContext.message, strlen(control));

    send_find_node((struct sockaddr*) &ipAddr, -1,
                                         (unsigned char*) "aa", 2,
                                         (unsigned char*) "mnopqrstuvwxyz123456",
                                         WANT6 | WANT4,
                                         0);

    printf("find_node4&6: %s\n\n", testContext.message);
    out |= memcmp(control46, testContext.message, strlen(control));

    return out;
}
