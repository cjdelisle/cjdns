#include <string.h>
#include <stdio.h>
#include <time.h>

#include <event2/dns.h>
#include <event2/dns_struct.h>
#include <event2/util.h>
/*#include <event2/event.h>*/

#include "memory/MemAllocator.h"

#include "BridgeModule.h"
#include "dns/DNSTools.h"
#include "dns/DNSReturnCodes.h"
#include "dht/DHTModules.h"
#include "dht/DHTConstants.h"
#include "dht/core/juliusz/dht.h"

#define MAX_CONCURRENT_REQUESTS 1024
#define TIME_TO_LIVE 30

struct BridgeModule_waitingDnsRequest {
    char key[20];
    time_t timeout;
    struct evdns_server_request* request;
    const struct evdns_server_question* question;
};

struct BridgeModule_context {
    unsigned int topRequest;
    struct BridgeModule_waitingDnsRequest waitingRequests[MAX_CONCURRENT_REQUESTS];
    struct DHTModule asDHT;
    struct DNSModule asDNS;
    struct DNSModuleRegistry* dnsRegistry;
};

static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry);
static int hexDecode(char* hex, size_t length, char* output, size_t* outLength);
static int handleQuestion(const struct evdns_server_question* question,
                          struct evdns_server_request* request,
                          struct BridgeModule_context* context);
static int handleKeyLookup(const struct evdns_server_question* question,
                           struct evdns_server_request* request,
                           struct BridgeModule_context* context);
static int removeWaitingRequest(unsigned int requestNumber,
                                struct BridgeModule_context* context);
static int handleIncomingDHT(struct DHTMessage* message,
                             void* vcontext);

#define WARN(x) fprintf(stderr, x);
#define DEBUG(x)

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param registry where the register the module.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* BridgeModule_registerNew(struct DNSModuleRegistry* registry,
                                           struct MemAllocator* allocator)
{
    struct BridgeModule_context* context =
        allocator->malloc(sizeof(struct BridgeModule_context), allocator);

    if (context == NULL) {
        return NULL;
    }

    context->dnsRegistry = registry;

    struct DNSModule localModule = {
        .name = "BridgeModule",
        .context = context,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .handleIncoming = handleIncoming,
        .handleOutgoing = NULL
    };
    memcpy(&(context->asDNS), &localModule, sizeof(struct DNSModule));

    DNSModules_register(&(context->asDNS), registry);

    return &context->asDNS;
}

struct DHTModule* BridgeModule_asDhtModule(struct DNSModule* module)
{
    if (module == NULL || strcmp(module->name, "BridgeModule") != 0) {
        return 0;
    }

    struct BridgeModule_context* context =
        (struct BridgeModule_context*) module->context;

    struct DHTModule localModule = {
        .name = "BridgeModule",
        .context = module->context,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .handleIncoming = handleIncomingDHT,
        .handleOutgoing = NULL
    };
    memcpy(&context->asDHT, &localModule, sizeof(struct DHTModule));

    return &context->asDHT;
}

/*--------------------Internal--------------------*/

static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry)
{
    struct BridgeModule_context* context =
        (struct BridgeModule_context*) module->context;

    int i;
    int ret;
    for (i = 0; i < message->request->nquestions; ++i) {
        ret = handleQuestion(message->request->questions[i], message->request, context);
        if (ret > -1) {
            if (ret > 0 && ret < 22) {
                message->returnCode = ret;
            }
            DNSModules_handleOutgoing(message, registry);
            return -1;
        }
    }

    return 0;
}

static int handleQuestion(const struct evdns_server_question* question,
                          struct evdns_server_request* request,
                          struct BridgeModule_context* context)
{
    const char* domain = question->name;
    char buffer[12];
    int ret = DNSTools_getDomainLabel(domain, 0, buffer, 12);
    if (ret != 0) {
        return DNS_RETURN_FORMAT_ERROR;
    }
    if (evutil_ascii_strcasecmp(buffer, "dht") == 0) {
        return handleKeyLookup(question, request, context);
    }

    return 0;
}

/**
 * @return -1 if the operation is in progress and a response will be sent later.
 *          0 if no error but no domain.
 *            DNS_RETURN_FORMAT_ERROR if the label is too long for a key.
 */
static int handleKeyLookup(const struct evdns_server_question* question,
                           struct evdns_server_request* request,
                           struct BridgeModule_context* context)
{
    char buffer[40];
    int ret = DNSTools_getDomainLabel(question->name, 1, buffer, 40);
    if (ret != 0) {
        /* No error but no domain. */
        return 0;
    }

    struct BridgeModule_waitingDnsRequest waitingRequest = {
        .timeout = time(NULL) + 5,
        .request = request,
        .question = question
    };

    size_t outLen = 20;
    ret = hexDecode(buffer, strlen(buffer), waitingRequest.key, &outLen);
    if (ret != 0) {
        /* No matter what, it's a "valid domain" but we can't handle it so it doesn't exist. */
        return 0;
    }

    if (context->topRequest == MAX_CONCURRENT_REQUESTS) {
        WARN("Maximum requests reached, discarding request! "
             "Consider increasing MAX_CONCURRENT_REQUESTS in BridgeModule.c");
        return -1;
    }

    struct BridgeModule_waitingDnsRequest* waitingRequestPtr =
        &context->waitingRequests[context->topRequest++];

    memcpy(waitingRequestPtr, &waitingRequest, sizeof(struct BridgeModule_waitingDnsRequest));

    dht_search((unsigned char*) waitingRequestPtr->key, 0, AF_INET, NULL, NULL);

    return -1;
}

static int handleIncomingDHT(struct DHTMessage* message,
                             void* vcontext)
{
    struct BridgeModule_context* context =
        (struct BridgeModule_context*) vcontext;

    if (message->bencoded == NULL || message->bencoded->type != BENC_DICT) {
        /* the message must always be a dictionary. */
        return -1;
    } 
    bobj_t* reply = bobj_dict_lookup(message->bencoded, &DHTConstants_reply);

    /* If it's not a reply then 99% sure we don't have the node we want. */
    if (reply == NULL) {
        return 0;
    }

    if (reply->type != BENC_DICT) {
        /* 'r' must always be a dictionary. */
        return -1;
    }

    bobj_t* idObj = bobj_dict_lookup(reply, &DHTConstants_myId);

    if (idObj == NULL || idObj->type != BENC_BSTR || idObj->as.bstr->len != 20) {
        /* Don't bother letting this packet get to the core, the node is broken. */
        return -1;
    }

    char* id = idObj->as.bstr->bytes;
    time_t now = time(NULL);

    /* Now look for a matching job. */
    size_t i;
    struct BridgeModule_waitingDnsRequest* waitingRequest;
    for (i = 0; i < context->topRequest; i++) {
        waitingRequest = context->waitingRequests + i;
        if (memcmp(id, waitingRequest->key, 20) == 0) {
            DEBUG("MATCH!!\n");
            /* Send a response. */
            evdns_server_request_add_a_reply(waitingRequest->request,
                                             waitingRequest->question->name,
                                             1,
                                             message->peerAddress,
                                             TIME_TO_LIVE);
            struct DNSMessage response = {
                .request = waitingRequest->request,
                .returnCode = 0
            };
            DNSModules_handleOutgoing(&response, context->dnsRegistry);

            removeWaitingRequest(i, context);
            /* There might be more requests for the same node so we won't break. */
        } else if (waitingRequest->timeout < now) {
            DEBUG("NO MATCH! Removing entry.\n");
            removeWaitingRequest(i, context);
        } else {
            DEBUG("NO MATCH!\n");
        }
    }
    return 0;
}

static int removeWaitingRequest(unsigned int requestNumber,
                                struct BridgeModule_context* context)
{
    memcpy(context->waitingRequests + requestNumber,
           context->waitingRequests + context->topRequest,
           sizeof(struct BridgeModule_waitingDnsRequest));
    context->topRequest--;
    return 0;
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
