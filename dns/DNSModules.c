#include <assert.h>
#include <string.h>
#include "libbenc/bencode.h"
#include "DNSModules.h"

/*--------------------Prototypes--------------------*/
static inline void forEachModule(int (*doThis)(struct DNSMessage* message,
                                               struct DNSModule *module,
                                               const struct DNSModuleRegistry* registry),
                                 struct DNSMessage* message,
                                 const struct DNSModuleRegistry *registry);
static inline void deserializeContext(struct DNSModule* module,
                                      struct DNSModuleRegistry* registry);

/*--------------------Interface--------------------*/

/** @see DNSModules.h */
struct DNSModuleRegistry* DNSModules_new()
{
    struct DNSModuleRegistry *reg = calloc(sizeof(struct DNSModuleRegistry), 1);
    struct DNSModule **newMembersList = calloc(sizeof(void*), 1);
    if (reg && newMembersList) {
        reg->members = newMembersList;
        return reg;
    }
    if (reg) { free(reg); }
    if (newMembersList) { free(newMembersList); }
    return NULL;
}

/** @see DNSModules.h */
void DNSModules_free(struct DNSModuleRegistry* registry)
{
    int freeModule(struct DNSMessage* message,
                   struct DNSModule* module,
                   const struct DNSModuleRegistry* registry)
    {
        message = message;
        registry = registry;
        if(module->free) {
            module->free(module);
        }
        return 0;
    }
    forEachModule(freeModule, NULL, registry);
    free(registry->members);
    free(registry);
}

/** @see DNSModules.h */
int DNSModules_register(struct DNSModule* module,
                        struct DNSModuleRegistry* registry)
{
    if (module == NULL || registry == NULL) {
        return -2;
    }

    deserializeContext(module, registry);

    int memberCount = registry->memberCount;
    struct DNSModule** newMembersList = calloc(sizeof(void*), memberCount + 2);
    if (newMembersList == NULL) {
        return -1;
    }
    memcpy(newMembersList, registry->members, (sizeof(void*) * (memberCount + 1)));
    assert(newMembersList[memberCount] == NULL);
    newMembersList[memberCount] = module;

    memberCount++;

    /* Must be null terminated. */
    assert(newMembersList[memberCount] == NULL);

    struct DNSModule** oldMembersList = registry->members;
    registry->members = newMembersList;
    registry->memberCount = memberCount;
    free(oldMembersList);

    return 0;
}

/** @see DNSModules.h */
void DNSModules_handleIncoming(struct DNSMessage* message,
                               const struct DNSModuleRegistry* registry)
{

    if (!(message && registry && registry->members && registry->memberCount)) {
        return;
    }

    struct DNSModule** firstModulePtr = registry->members;
    struct DNSModule** modulePtr =
        &registry->members[registry->memberCount - 1];
    struct DNSModule* module;

    while (modulePtr >= firstModulePtr) {
        module = *modulePtr;
        if (module && module->handleIncoming) {
            if (module->handleIncoming(message, module, registry) != 0) {
                return;
            }
        }
        modulePtr--;
    }
}

/** @see DNSModules.h */
void DNSModules_handleOutgoing(struct DNSMessage* message,
                               const struct DNSModuleRegistry* registry)
{
    int handleOutgoing(struct DNSMessage* message,
                       struct DNSModule* module,
                       const struct DNSModuleRegistry* registry)
    {
        if (module->handleOutgoing) {
            return module->handleOutgoing(message, module, registry);
        }
        return 0;
    }
    forEachModule(handleOutgoing, message, registry);
}

/*----------------------Internals----------------------*/

/**
 * Deserialize the context for this module.
 * First the registry is deserialized then the modules are registered.
 * When the modules are registered, if they have serialized contexts, 
 * those are deserialized by this function which calls their own
 * deserialization functions.
 *
 * @param module the module to deserialize the context for.
 * @param registry the DNS module registry.
 */
static inline void deserializeContext(struct DNSModule* module,
                                      struct DNSModuleRegistry* registry)
{
    char* name = (char*) module->name;
    if (module && registry && registry->serializedContexts) {
        bobj_t* serContext =
            bobj_dict_lookup(registry->serializedContexts,
                             &(benc_bstr_t) { .len = strlen(name),
                                              .bytes = name } );
        if (module->deserialize
            && module->context
            && serContext
            && serContext->as.bstr)
        {
            module->deserialize(*serContext->as.bstr,
                                module->context);
        }
    }
}

/**
 * Do something to every module which is registered.
 * @param doThis the callback.
 * @param registry state.
 */
static inline void forEachModule(int (*doThis)(struct DNSMessage* message,
                                               struct DNSModule *module,
                                               const struct DNSModuleRegistry* registry),
                                 struct DNSMessage* message,
                                 const struct DNSModuleRegistry *registry)
{
    struct DNSModule** modulePtr = registry->members;
    struct DNSModule* module = *modulePtr;
    int ret;
    while (module) {
        ret = doThis(message, module, registry);
        if (ret != 0) {
            /* If doThis does not return 0 then stop. */
            return;
        }
        modulePtr++;
        module = *modulePtr;
    }
}
