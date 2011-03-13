#include <assert.h>
#include <string.h>

#include "memory/MemAllocator.h"
#include "memory/BufferAllocator.h"
#include "io/Reader.h"
#include "io/Writer.h"
#include "libbenc/bencode.h"
#include "DHTModules.h"

#define DEBUG2(x, y)
/* #define DEBUG2(x, y) fprintf(stderr, x, y); fflush(stderr) */

/*--------------------Prototypes--------------------*/
static void forEachModule(int (*doThis)(struct DHTModule* module, struct DHTMessage* message),
                          struct DHTMessage* message,
                          const struct DHTModuleRegistry* registry);
static inline void deserializeContext(struct DHTModule* module,
                                      struct DHTModuleRegistry* registry);

/*--------------------Interface--------------------*/

/** @see DHTModules.h */
struct DHTModuleRegistry* DHTModules_new()
{
    struct DHTModuleRegistry *reg = calloc(sizeof(struct DHTModuleRegistry), 1);
    struct DHTModule **newMembersList = calloc(sizeof(void*), 1);
    if (reg && newMembersList) {
        reg->members = newMembersList;
        return reg;
    }
    if (reg) { free(reg); }
    if (newMembersList) { free(newMembersList); }
    return NULL;
}

/** @see DHTModules.h */
int DHTModules_register(struct DHTModule* module,
                        struct DHTModuleRegistry* registry)
{
    if (module == NULL || registry == NULL) {
        return -2;
    }

    deserializeContext(module, registry);

    int memberCount = registry->memberCount;
    struct DHTModule** newMembersList = calloc(sizeof(void*), memberCount + 2);
    if (newMembersList == NULL) {
        return -1;
    }
    memcpy(newMembersList, registry->members, (sizeof(void*) * (memberCount + 1)));
    assert(newMembersList[memberCount] == NULL);
    newMembersList[memberCount] = module;

    memberCount++;

    /* Must be null terminated. */
    assert(newMembersList[memberCount] == NULL);

    struct DHTModule** oldMembersList = registry->members;
    registry->members = newMembersList;
    registry->memberCount = memberCount;
    free(oldMembersList);

    return 0;
}

/** @see DHTModules.h */
int DHTModules_compareNodes(const char nodeId[20],
                            const char otherNodeId[20],
                            const struct DHTModuleRegistry* registry)
{
    int score = 0;
    int compare(struct DHTModule *module, struct DHTMessage* nothing)
    {
        nothing = nothing;
        if (module->compareNodes && module->context) {
            score += module->compareNodes(nodeId,
                                          otherNodeId,
                                          module->context);
        }
        return 0;
    }
    forEachModule(compare, NULL, registry);
    return score;
}

/** @see DHTModules.h */
void DHTModules_handleIncoming(struct DHTMessage* message,
                               const struct DHTModuleRegistry* registry)
{
    assert(message);
    assert(registry);
    struct DHTModuleRegistry kkkkkkk = *registry;
    kkkkkkk.members = kkkkkkk.members;
    assert(registry->members);
    assert(registry->memberCount);
    if (!(message && registry && registry->members && registry->memberCount)) {
        return;
    }

    struct DHTModule** firstModulePtr = registry->members;
    struct DHTModule** modulePtr = registry->members + registry->memberCount - 1;
    struct DHTModule* module;

    while (modulePtr >= firstModulePtr) {
        module = *modulePtr;
        if (module && module->handleIncoming) {
            DEBUG2("<< calling: %s->handleIncoming\n", module->name);
            module->handleIncoming(message, module->context);
        } else {
            DEBUG2("<< skipping %s->handleIncoming\n", module->name);
        }
        modulePtr--;
    }
}

static int dhtModuleHandleOutgoing(struct DHTModule* module, struct DHTMessage* message)
{
    assert(module != NULL);
    if (module->handleOutgoing) {
        DEBUG2(">> calling: %s->handleOutgoing\n", module->name);
        return module->handleOutgoing(message, module->context);
    } else {
        DEBUG2(">> skipping: %s->handleOutgoing\n", module->name);
    }
    return 0;
}

/** @see DHTModules.h */
void DHTModules_handleOutgoing(struct DHTMessage* message,
                               const struct DHTModuleRegistry* registry)
{
    forEachModule(dhtModuleHandleOutgoing, message, registry);
}

/** @see DHTModules.h */
void DHTModules_serialize(const struct DHTModuleRegistry* registry,
                          const struct Writer* writer)
{
    char buffer[1024];
    struct MemAllocator* allocator = BufferAllocator_new(buffer, 1024);
    Dict* dictionary = benc_newDictionary(allocator);

    struct DHTModule** modulePtr = registry->members;
    struct DHTModule* module = *modulePtr;
    while (module) {
        if (module->serialize != NULL) {
            benc_putObject(dictionary,
                           benc_newString(module->name, allocator),
                           module->serialize(module->context),
                           allocator);
        }
        modulePtr++;
        module = *modulePtr;
    }
    bobj_serialize(writer, &(bobj_t) { .type = BENC_DICT, .as.dict = *dictionary });
}

/** @see DHTModules.h */
struct DHTModuleRegistry* DHTModules_deserialize(const struct Reader* reader,
                                                 const struct MemAllocator* allocator)
{
    bobj_t* obj = NULL;
    if (bobj_parse(reader, allocator, &obj) || obj == NULL || obj->type != BENC_DICT) {
        return NULL;
    }

    struct DHTModuleRegistry* reg = DHTModules_new();
    if (reg) {
        reg->serializedContexts = obj;
        return reg;
    }

    return NULL;
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
 * @param registry the DHT module registry.
 */
static inline void deserializeContext(struct DHTModule* module,
                                      struct DHTModuleRegistry* registry)
{
    char* name = (char*) module->name;
    if (module && registry && registry->serializedContexts) {
        bobj_t* serContext =
            bobj_dict_lookup(registry->serializedContexts,
                             &(benc_bstr_t) { .len = strlen(name), .bytes = name } );
        if (module->deserialize && module->context && serContext && serContext->as.bstr) {
            module->deserialize(serContext, module->context);
        }
    }
}

/**
 * Do something to every module which is registered.
 * @param doThis the callback.
 * @param registry state.
 */
static void forEachModule(int (*doThis)(struct DHTModule* module, struct DHTMessage* message),
                          struct DHTMessage* message,
                          const struct DHTModuleRegistry* registry)
{
    struct DHTModule** modulePtr = registry->members;
    struct DHTModule* module = *modulePtr;
    while (module) {
        if (doThis(module, message) != 0) {
            return;
        }
        modulePtr++;
        module = *modulePtr;
    }
}
