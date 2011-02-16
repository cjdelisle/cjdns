#include "LegacyConnectorModule_framework.c"

int main()
{
    const char* control =
        "d1:q13:announce_peer1:y1:q1:t2:aa1:ad4:porti13111e2:id20:abcdefghij0123456789"
        "5:token8:aoeusnth9:info_hash20:abcdefghijklmnopqrstee";

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

    int out = 0;

    send_announce_peer((struct sockaddr*) &ipAddr, -1,
                       (unsigned char*) "aa", 2,
                       (unsigned char*) "abcdefghijklmnopqrst",
                       (unsigned short) 13111,
                       (unsigned char*) "aoeusnth", 8,
                       -1);

    printf("announce_peer: %s\n\n", testContext.message);

    out |= memcmp(control, testContext.message, strlen(control));

    return out;
}
