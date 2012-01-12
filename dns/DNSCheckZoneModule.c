/*
 * You may redistribute this program and/or modify it under the terms of
 * the GNU General Public License as published by the Free Software Foundation,
 * either version 3 of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
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
static const char* ALLOWED_TLDS[2] = {"torrent", NULL};

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
        .handleIncoming = handleIncoming,
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
