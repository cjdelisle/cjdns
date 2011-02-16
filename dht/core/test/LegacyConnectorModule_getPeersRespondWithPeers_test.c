#include "LegacyConnectorModule_framework.c"

int main()
{
    /* Since the order of the values is random, we must validate both possibilities. */
    const char* control =
        "d1:y1:r1:t2:aa1:rd2:id20:abcdefghij01234567896:valuesl6:CCCC356:AAAA33e"
        "5:token8:aoeusnth5:nodes40:abcdefghijklmnopqrst1234567890abcdefghijee";

    const char* controlB =
        "d1:y1:r1:t2:aa1:rd2:id20:abcdefghij01234567896:valuesl6:AAAA336:CCCC35e"
        "5:token8:aoeusnth5:nodes40:abcdefghijklmnopqrst1234567890abcdefghijee";

    const char* control6 =
        "d1:y1:r1:t2:aa1:rd2:id20:abcdefghij01234567896:valuesl18:BBBBBBBBBBBBBBBB34"
        "18:DDDDDDDDDDDDDDDD36e5:token8:aoeusnth5:nodes"
        "40:abcdefghijklmnopqrst1234567890abcdefghijee";

    const char* control6B =
        "d1:y1:r1:t2:aa1:rd2:id20:abcdefghij01234567896:valuesl18:DDDDDDDDDDDDDDDD36"
        "18:BBBBBBBBBBBBBBBB34e5:token8:aoeusnth5:nodes"
        "40:abcdefghijklmnopqrst1234567890abcdefghijee";


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

    struct peer peers[4];

    memcpy(peers[0].ip, "AAAA", 4);
    peers[0].port = 13107;
    peers[0].len = 4;

    memcpy(peers[1].ip, "BBBBBBBBBBBBBBBB", 16);
    peers[1].port = 13108;
    peers[1].len = 16;

    memcpy(peers[2].ip, "CCCC", 4);
    peers[2].port = 13109;
    peers[2].len = 4;

    memcpy(peers[3].ip, "DDDDDDDDDDDDDDDD", 16);
    peers[3].port = 13110;
    peers[3].len = 16;

    struct storage store = {
        .numpeers = 4,
        .peers = peers
    };

    /* 40 byes long, twice the length of a node id (2 nodes). */
    const unsigned char* nodes =
        (unsigned char*) "abcdefghijklmnopqrst1234567890abcdefghij";

    int out = 0;

    send_nodes_peers((struct sockaddr*) &ipAddr, -1,
                                           (unsigned char*) "aa", 2,
                                           nodes, 40,
                                           (unsigned char*) "", 0,
                                           AF_INET,
                                           &store,
                                           (unsigned char*) "aoeusnth", 8);

    printf("\nget_peers response with peers: %s\n", testContext.message);

    out |= (memcmp(control, testContext.message, strlen(control))
            & memcmp(controlB, testContext.message, strlen(controlB)));

    send_nodes_peers((struct sockaddr*) &ipAddr, -1,
                                           (unsigned char*) "aa", 2,
                                           nodes, 40,
                                           (unsigned char*) "", 0,
                                           AF_INET6,
                                           &store,
                                           (unsigned char*) "aoeusnth", 8);

    printf("get_peers for ip6 response with peers: %s\n\n", testContext.message);

    out |= (memcmp(control6, testContext.message, strlen(control6))
            & memcmp(control6B, testContext.message, strlen(control6B)));

    return out;
}
