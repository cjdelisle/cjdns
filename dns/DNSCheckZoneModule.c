#include <event2/dns.h>
#include <event2/dns_struct.h>
#include <event2/util.h>
#include <event2/event.h>
#include "dns/DNSModules.h"
#include "dns/DNSReturnCodes.h"
#include "dns/DNSTools.h"

/* memcpy */
#include <string.h>

/** A NULL terminated list of zones which thei dns server is permistted to serve. */
static const char* ALLOWED_TLDS[2] = {"dht", NULL};

static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry);

/**
 * Create a new DNS module for failing any requests which are not in the .key zone.
 *
 * @param allocator the memory allocator to use for getting memory.
 */
struct DNSModule* DNSCheckZoneModule_new(struct MemAllocator* allocator)
{
    struct DNSModule* module = allocator->malloc(sizeof(struct DNSModule), allocator);

    if (module == NULL) {
        return NULL;
    }

    struct DNSModule localModule = {
        .name = "DNSCheckZoneModule",
        .context = NULL,
        .free = NULL,
        .serialize = NULL,
        .deserialize = NULL,
        .handleIncoming = handleIncoming,
        .handleOutgoing = NULL
    };
    memcpy(module, &localModule, sizeof(struct DNSModule));

    return module;
}

/*--------------------Internal--------------------*/


static int handleIncoming(struct DNSMessage* message,
                          struct DNSModule* module,
                          const struct DNSModuleRegistry* registry)
{
    module = module;

    int i;
    for (i = 0; i < message->request->nquestions; i++) {
        const char* domain = message->request->questions[i]->name;
        char tld[8];
        if (DNSTools_getDomainLabel(domain, 0, tld, 8) != 0) {
            break;
        }
        int j;
        for (j = 0; ALLOWED_TLDS[j] != NULL; j++) {
            if (evutil_ascii_strcasecmp(tld, ALLOWED_TLDS[j]) != 0) {
                break;
            }
        }
        if (ALLOWED_TLDS[j] == NULL) {
            return 0;
        }
    }
    /* Wrong zone. */
    message->returnCode = DNS_RETURN_NOT_AUTH;
    DNSModules_handleOutgoing(message, registry);
    return -1;
}
