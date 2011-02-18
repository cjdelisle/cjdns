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

    char* buffer = malloc(32000000);
    if (buffer == NULL) {
        printf("Failed to allocate memory\n");
        return -1;
    }
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 32000000);
    if (allocator == NULL) {
        printf("Failed to build a memory allocator\n");
        return -1;
    }

    /* Ohhhh jeeee: operation is not possible without initialized secure memory */
    Crypto_init();

    char idBuff[20];
    char* id = idBuff;
    size_t idLen = 20;
    if (argc > 4) {
        hexDecode(argv[4], strlen(argv[4]), id, &idLen);
    } else {
        KeyPair* kp = Crypto_newKeyPair("p160");
        id = kp->publicKey.as.bstr->bytes;
    }

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
    print_hex((unsigned char*) id, 20);
    printf(".dht\nThis is your domain address.\n\n\n\n");

    struct DHTModule* legacy =
        LegacyConnectorModule_new(base, id, registry, dht_random, dht_hash_impl);
    if (legacy == NULL) {
        printf("Failed to allocate DHT legacy connector module.\n");
        return -1;
    }

    struct DHTModule* serialization = SerializationModule_new();
    if (serialization == NULL) {
        printf("Failed to allocate DHT serialization module.\n");
        return -1;
    }

    /* Need 2 debug modules one for incoming and one for outgoing so that
     * the outgoing module will have access to the serialized message and the incoming
     * will have access to the fully parsed message. */
    struct DHTModule* debugIn = DebugModule_new(allocator, -1);
    struct DHTModule* debugOut = DebugModule_new(allocator, 1);

    int ret = DHTModules_register(legacy, registry)
            | DHTModules_register(bridgeDHT, registry)
            | DHTModules_register(debugIn, registry)
            | DHTModules_register(serialization, registry)
            | DHTModules_register(debugOut, registry)
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
        if (ptr - allChars > 16) {
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
