#include <string.h>
#include <stdio.h>
#include <time.h>

#include <event2/dns.h>
#include <event2/util.h>

#include "memory/MemAllocator.h"

#include "BridgeModule.h"
#include "dns/DNSTools.h"
#include "dns/DNSReturnCodes.h"
#include "dht/dhtcore/RouterModule.h"
#include "dht/DHTConstants.h"
#include "libbenc/benc.h"
#include "util/Timeout.h"

#define TIME_TO_LIVE 30
#define TIMEOUT_MILLISECONDS 5000

struct Context {
    struct DNSModule asDNS;
    struct DNSModuleRegistry* dnsRegistry;
    struct RouterModule* routerModule;
    struct event_base* eventBase;
    struct MemAllocator* allocator;
};

struct WaitingRequest
{
    struct evdns_server_request* request;
    const struct evdns_server_question* question;
    struct Timeout* timeout;
    struct MemAllocator* allocator;
    struct Context* context;
    struct RouterModule_Search* search;
};

static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry);
static int hexDecode(char* hex, size_t length, char* output, size_t* outLength);
static int handleQuestion(const struct evdns_server_question* question,
                          struct evdns_server_request* request,
                          struct Context* context);
static int handleKeyLookup(const struct evdns_server_question* question,
                           struct evdns_server_request* request,
                           struct Context* context);

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param registry where the register the module.
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* BridgeModule_registerNew(struct DNSModuleRegistry* registry,
                                           struct MemAllocator* allocator,
                                           struct RouterModule* routerModule,
                                           struct event_base* eventBase)
{
    struct Context* context =
        allocator->malloc(sizeof(struct Context), allocator);

    context->dnsRegistry = registry;
    context->routerModule = routerModule;
    context->eventBase = eventBase;
    context->allocator = allocator;

    struct DNSModule localModule = {
        .name = "BridgeModule",
        .context = context,
        .handleIncoming = handleIncoming
    };
    memcpy(&(context->asDNS), &localModule, sizeof(struct DNSModule));

    DNSModules_register(&(context->asDNS), registry);

    return &context->asDNS;
}

/*--------------------Internal--------------------*/

static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry)
{
    struct Context* context =
        (struct Context*) module->context;

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
        /* If we handle more than one question the request gets released too many times. */
        return 0;
    }

    return 0;
}

static int handleQuestion(const struct evdns_server_question* question,
                          struct evdns_server_request* request,
                          struct Context* context)
{
    const char* domain = question->name;
    char buffer[12];
    int ret = DNSTools_getDomainLabel(domain, 0, buffer, 12);
    if (ret != 0) {
        return DNS_RETURN_FORMAT_ERROR;
    }
    if (evutil_ascii_strcasecmp(buffer, "torrent") == 0) {
        return handleKeyLookup(question, request, context);
    }

    return 0;
}

static bool routerCallback(void* callbackContext, struct DHTMessage* message)
{
    struct WaitingRequest* waitingRequest = (struct WaitingRequest*) callbackContext;

    if (message == NULL) {
        // Couldn't find anything :(
        evdns_server_request_drop(waitingRequest->request);
        waitingRequest->allocator->free(waitingRequest->allocator);
        return false;
    }

    Dict* arguments = benc_lookupDictionary(message->asDict, &DHTConstants_reply);
    List* values = benc_lookupList(arguments, &DHTConstants_values);
    if (values != NULL) {
        for (int32_t i = 0; i < benc_itemCount(values); i++) {
            String* val = benc_getString(values, i);
            if (val != NULL && val->len == 6) {
                char* cname = waitingRequest->allocator->malloc(32, waitingRequest->allocator);
                sprintf(cname, "%d.%d.%d.%d:%d", (uint32_t) val->bytes[1] & 0xFF,
                                                 (uint32_t) val->bytes[2] & 0xFF,
                                                 (uint32_t) val->bytes[3] & 0xFF,
                                                 (uint32_t) val->bytes[4] & 0xFF,
                                                 (uint32_t) ntohs((uint16_t)val->bytes[5]) & 0xFFFF);
                evdns_server_request_add_cname_reply(waitingRequest->request,
                                                     waitingRequest->question->name,
                                                     cname,
                                                     TIME_TO_LIVE);
            } else if (val == NULL) {
                printf("Got an entry that wasn't a string!?\n");
            } else {
                printf("got entry of length %d\n", (uint32_t) val->len);
            }

            if (i > 7) {
                // Too many entries will make it not fit in a udp packet.
                break;
            }
        }
        struct DNSMessage response = {
            .request = waitingRequest->request,
            .returnCode = 0
        };
        DNSModules_handleOutgoing(&response, waitingRequest->context->dnsRegistry);
        waitingRequest->allocator->free(waitingRequest->allocator);
        return true;
    }
    return false;
}

static void timeoutCallback(void* callbackContext)
{
    struct WaitingRequest* waitingRequest = (struct WaitingRequest*) callbackContext;
    RouterModule_cancelSearch(waitingRequest->search);
    evdns_server_request_drop(waitingRequest->request);
    waitingRequest->allocator->free(waitingRequest->allocator);
}

/**
 * @return -1 if the operation is in progress and a response will be sent later.
 *          0 if no error but no domain.
 */
static int handleKeyLookup(const struct evdns_server_question* question,
                           struct evdns_server_request* request,
                           struct Context* context)
{
    char buffer[40];
    int ret = DNSTools_getDomainLabel(question->name, 1, buffer, 40);
    if (ret != 0) {
        /* No error but no domain. */
        return 0;
    }

    char infoHash[20];
    size_t outLen = 20;
    ret = hexDecode(buffer, strlen(buffer), infoHash, &outLen);
    if (ret != 0) {
        /* No matter what, it's a "valid domain" but we can't handle it so it doesn't exist. */
        return 0;
    }

    struct MemAllocator* requestAllocator = context->allocator->child(context->allocator);
    struct WaitingRequest* waitingRequest =
        requestAllocator->malloc(sizeof(struct WaitingRequest), requestAllocator);

    waitingRequest->request = request;
    waitingRequest->question = question;
    waitingRequest->timeout =
        Timeout_setTimeout(timeoutCallback,
                           waitingRequest,
                           TIMEOUT_MILLISECONDS,
                           context->eventBase,
                           requestAllocator);
    waitingRequest->allocator = requestAllocator;
    waitingRequest->context = context;
    waitingRequest->search =
        RouterModule_beginSearch(&DHTConstants_getPeers,
                                 &DHTConstants_infoHash,
                                 (uint8_t*)infoHash,
                                 routerCallback,
                                 waitingRequest,
                                 context->routerModule);

    return -1;
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
