#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "net/NetworkTools.h"
#include "dht/core/LegacyConnectorModule.h"
#include "dht/LibeventNetworkModule.h"
#include "dht/SerializationModule.h"
#include "dht/DebugModule.h"
#include "crypto/Crypto.h"
#include "dns/DNSModules.h"
#include "dns/DNSNetworkModule.h"
#include "dns/DNSCheckZoneModule.h"
#include "BridgeModule.h"

/*#include "event2/util.h"*/
#include "dht/core/juliusz/dht.h"

static int dht_random(void *buf, size_t size);

static void dht_hash_impl(void *hash_return, int hash_size,
                          const void *v1, int len1,
                          const void *v2, int len2,
                          const void *v3, int len3);

static void print_hex(const unsigned char *buf, int buflen);

int main(int argc, char** argv)
{
    if (argc < 4) {
        printf("Usage: cjdns dns_ip:port dht_ip:port dht_ping_ip:port\n"
               "   EG: cjdns 127.0.0.1:5353 0.0.0.0:5000 67.215.242.138:6881\n");
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

printf("binding dns on socket %s\n", argv[1]);
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

    char* buffer = malloc(32000);
    if (buffer == NULL) {
        printf("Failed to allocate memory\n");
        return -1;
    }
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 32000000);
    if (allocator == NULL) {
        printf("Failed to build a memory allocator\n");
        return -1;
    }

    Crypto_init();
    KeyPair* kp = Crypto_newKeyPair("p160");

    struct event_base* base = event_base_new();

    /* ------------------ DNS ------------------ */
    struct DNSModuleRegistry* dnsRegistry = DNSModules_new();
    struct DNSModule* bridgeDNS = BridgeModule_registerNew(dnsRegistry, allocator);
    struct DHTModule* bridgeDHT = BridgeModule_asDhtModule(bridgeDNS);

    if (bridgeDNS == NULL) {
        printf("Failed to create DNS bridge module.\n");
        return -1;
    }

    if (bridgeDHT == NULL) {
        printf("Failed to create DHT bridge module.\n");
        return -1;
    }
    
    /* ------------------ DHT ------------------ */
    struct DHTModuleRegistry* registry = DHTModules_new();

    if (registry == NULL) {
        printf("Failed to allocate DHT registry.\n");
        return -1;
    }

    printf("\n\nANNOUNCING: ");
    print_hex((unsigned char*) kp->publicKey.as.bstr->bytes, 20);
    printf(".dht\nThis is your domain address.\n\n\n\n");

    struct DHTModule* legacy =
        LegacyConnectorModule_new(base, kp->publicKey.as.bstr->bytes, registry, dht_random, dht_hash_impl);
    if (legacy == NULL) {
        printf("Failed to allocate DHT legacy connector module.\n");
        return -1;
    }

    struct DHTModule* serialization = SerializationModule_new();
    if (serialization == NULL) {
        printf("Failed to allocate DHT serialization module.\n");
        return -1;
    }

    struct DHTModule* debug = DebugModule_new(allocator);

    int ret = DHTModules_register(legacy, registry)
            | DHTModules_register(bridgeDHT, registry)
            | DHTModules_register(serialization, registry)
            | DHTModules_register(debug, registry)
            | LibeventNetworkModule_register(base, dhtSocket, 6, registry);

    if (ret != 0) {
        printf("Failed to register modules\n");
        return -1;
    }

    /* Send ping. */
    dht_ping_node(pingNode, addrLength);

    /* DNS */
    struct DNSModule* checkZone = DNSCheckZoneModule_new(allocator);
    struct DNSModule* dnsNetwork = DNSNetworkModule_new(base, dnsSocket, allocator);
    if (dnsRegistry == NULL || checkZone == NULL || dnsNetwork == NULL) {
        printf("Failed to allocate dns modules");
        return -1;
    }

    ret |= DNSModules_register(bridgeDNS, dnsRegistry);
    ret |= DNSModules_register(checkZone, dnsRegistry);
    ret |= DNSNetworkModule_register(dnsNetwork, dnsRegistry);
    if (ret != 0) {
        printf("Failed to register DNS modules");
    }

    event_base_loop(base, 0);
}

static int dht_random(void *buf, size_t size)
{
    gcry_randomize(buf, size, GCRY_STRONG_RANDOM);
    return 0;
}

static void dht_hash_impl(void *hashReturn, int hashSize,
                          const void *v1, int len1,
                          const void *v2, int len2,
                          const void *v3, int len3)
{
    gcry_md_hd_t mh;
    char *md;

    gcry_md_open(&mh, GCRY_MD_SHA1, GCRY_MD_FLAG_SECURE);

    gcry_md_write(mh, v1, len1);
    gcry_md_write(mh, v2, len2);
    gcry_md_write(mh, v3, len3);

    md = (char*) gcry_md_read(mh, 0);
    memcpy(hashReturn, md, hashSize > 20 ? 20 : hashSize);
    gcry_md_close(mh);
}

static void print_hex(const unsigned char *buf, int buflen)
{
    int i;
    for(i = 0; i < buflen; i++) {
        printf("%02x", buf[i]);
    }
}
