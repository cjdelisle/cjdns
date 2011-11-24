#include "BridgeModule.h"
#include "crypto/Crypto.h"
#include "dht/DebugModule.h"
#include "dht/DHTConstants.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/dhtstore/DHTStoreModule.h"
#include "dht/LibeventNetworkModule.h"
#include "dht/ReplyModule.h"
#include "dht/SerializationModule.h"
#include "dns/DNSCheckZoneModule.h"
#include "dns/DNSModules.h"
#include "dns/DNSNetworkModule.h"
#include "memory/MemAllocator.h"
#include "memory/MallocAllocator.h"
#include "memory/BufferAllocator.h"
#include "net/NetworkTools.h"

static int hexDecode(char* hex, size_t length, char* output, size_t* outLength);

int main(int argc, char** argv)
{
    if (argc < 4) {
        printf("Usage: cjdns dns_ip:port dht_ip:port dht_ping_ip:port [hex node id]\n"
               "   EG: cjdns 127.0.0.1:5353 0.0.0.0:5000 67.215.242.138:6881 "
               "whenYouChooseANonRandomIdGodKillsAKitten\n"
               "The Id really really should be random and in hex format.\n"
               "When you first start the node, just copy the id it gives you have into "
               "a text file\n"
               "then you can restart it with the same id and be in the same neighborhood\n");
        return -1;
    }

    struct sockaddr_storage pingNodeStore;
    struct sockaddr* pingNode = (struct sockaddr*) &pingNodeStore;
    int addrLength = sizeof(struct sockaddr_storage);
    if (evutil_parse_sockaddr_port(argv[3], pingNode, &addrLength) != 0)
    {
        printf("Failed to parse address and port ( %s ) for pinging.\n", argv[3]);
        return -1;
    }

    evutil_socket_t dnsSocket = NetworkTools_bindSocket(argv[1]);

    if (dnsSocket == -1) {
        printf("Failed to bind dns socket\n");
        return -1;
    }

    evutil_socket_t dhtSocket = NetworkTools_bindSocket(argv[2]);

    if (dhtSocket == -1) {
        printf("Failed to bind dht socket\n");
        return -1;
    }

    #define BUFFER_SIZE (20 * 1000 * 1000)
    struct MemAllocator* allocator = MallocAllocator_new(BUFFER_SIZE);


    char idBuff[20];
    char* id = idBuff;
    size_t idLen = 20;
    if (argc > 4) {
        hexDecode(argv[4], strlen(argv[4]), id, &idLen);
    } else {
        Crypto_randomize( &(String){ .bytes = idBuff, .len = 20 } );
    }

    struct event_base* eventBase = event_base_new();
    
    /* ------------------ DHT ------------------ */
    struct DHTModuleRegistry* registry = DHTModules_new(/*allocator*/);

    if (registry == NULL) {
        printf("Failed to allocate DHT registry.\n");
        return -1;
    }

    ReplyModule_register(registry, allocator);
    struct RouterModule* router =
        RouterModule_register(registry, allocator, (uint8_t*) id, eventBase);
    DHTStoreModule_register(10000, registry, allocator);


    /* Need 2 debug modules one for incoming and one for outgoing so that
     * the outgoing module will have access to the serialized message and the incoming
     * will have access to the fully parsed message. *
    struct DHTModule* debugIn = DebugModule_new(allocator);
    struct DHTModule* debugOut = DebugModule_forOutput(debugIn);
    FILE* log = fopen("cjdns.log", "a+");
    DebugModule_setLog(log, debugIn);*/

    //DHTModules_register(debugIn, registry);
    SerializationModule_register(registry, allocator);
    //DHTModules_register(debugOut, registry);
    LibeventNetworkModule_register(eventBase, dhtSocket, 6, registry, allocator);

    /* Send ping. */
    uint8_t nodeToPing[18];
    NetworkTools_addressFromSockaddr(&pingNodeStore, (char*) nodeToPing);
    RouterModule_pingNode(nodeToPing, router);

    /* DNS */
    struct DNSModuleRegistry* dnsRegistry = DNSModules_new();
    BridgeModule_registerNew(dnsRegistry, allocator, router, eventBase);
    struct DNSModule* checkZone = DNSCheckZoneModule_new(allocator);
    struct DNSModule* dnsNetwork = DNSNetworkModule_new(eventBase, dnsSocket, allocator);
    if (dnsRegistry == NULL || checkZone == NULL || dnsNetwork == NULL) {
        printf("Failed to allocate dns modules");
        return -1;
    }
    DNSModules_register(checkZone, dnsRegistry);
    DNSNetworkModule_register(dnsNetwork, dnsRegistry);

    event_base_loop(eventBase, 0);
}

/**
 * @return 0 if all went well, -1 if the length is not an even number, -2 if outLength is
 *           too small to hold the output, -3 if the input contains an invalid char.
 */
static int hexDecode(char* hex, size_t length, char* output, size_t* outLength)
{
    if ((size_t)(length &~ 2) != length) {
        /* That can't be hex. */
        return -1;
    }
    if (outLength == NULL || *outLength < length / 2) {
        /* Not enough output space. */
        return -2;
    }

    const char* allChars = "0123456789abcdefABCDEF";
    char* ptr;

    size_t i;
    int outIndex = 0;
    int thisByte = 0;
    for (i = 0; i < length; i++) {
        ptr = strchr(allChars, hex[i]);
        if (ptr == NULL) {
            /* invalid char. */
            return -3;
        }
        if (ptr - allChars > 15) {
            ptr -= 6;
        }

        thisByte += ptr - allChars;

        if (i & 1) {
            output[outIndex] = (char) thisByte;
            outIndex++;
            thisByte = 0;
        } else {
            thisByte = thisByte << 4;
        }
    }
    *outLength = outIndex;

    return 0;
}
