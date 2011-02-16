/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 3 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA
 * 02111-1307 USA
 */

#include <assert.h>
#include "libbenc/bencode.h"
#include "DHTModules.h"

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
void DHTModules_free(struct DHTModuleRegistry *registry)
{
    int freeModule(struct DHTModule *module, struct DHTMessage* nothing)
    {
        nothing = nothing;
        if(module->free) {
            module->free(module);
        }
        return 0;
    }
    forEachModule(freeModule, NULL, registry);
    free(registry->members);
    free(registry);
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
int DHTModules_compareNodes(const benc_bstr_t nodeId,
                            const benc_bstr_t otherNodeId,
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

    if (!(message && registry && registry->members && registry->memberCount)) {
        return;
    }

    struct DHTModule** firstModulePtr = registry->members;
    struct DHTModule** modulePtr =
        &registry->members[registry->memberCount - 1];
    struct DHTModule* module;

    while (modulePtr >= firstModulePtr) {
        module = *modulePtr;
        if (module && module->handleIncoming) {
            module->handleIncoming(message, module->context);
        }
        modulePtr--;
    }
}

static int dhtModuleHandleOutgoing(struct DHTModule* module, struct DHTMessage* message)
{
    if (module->handleOutgoing) {
        return module->handleOutgoing(message, module->context);
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
benc_bstr_t* DHTModules_serialize(const struct DHTModuleRegistry* registry)
{
    int count = 0;
    int countModules(struct DHTModule* module, struct DHTMessage* nothing)
    {
        nothing = nothing;
        if (module && module->serialize && module->context) {
            count++;
        }
        return 0;
    }
    forEachModule(countModules, NULL, registry);

    /*
     * create a memory location which will contain:
     * dictEntry1, dictEntry1->key, dictEntry1->value,
     * dictEntry2, dictEntry2->key, dictEntry2->value,
     * etc.
     */
    benc_dict_entry_t* dictRoot = calloc(sizeof(benc_dict_entry_t)
                                             + sizeof(benc_bstr_t)
                                             + sizeof(bobj_t),
                                         count);

    if (dictRoot == NULL) {
        return NULL;
    }

    /*
     * Because the loop needs to skip to the next entry each time,
     * start it off on a pseudo entry.
     */
    benc_dict_entry_t* dictEntry = &(benc_dict_entry_t) {
        .key = NULL,
        .val = NULL,
        .next = dictRoot
    };

    unsigned int outBufferRequirement = 0;

    int serializeModule(struct DHTModule* module, struct DHTMessage* nothing)
    {
        nothing = nothing;
        if (!(module && module->serialize && module->context)) {
            return 0;
        }
        benc_bstr_t* serialized = module->serialize(module->context);
        if (!(serialized && serialized->bytes && serialized->len > 0)) {
            return 0;
        }

        dictEntry = dictEntry->next;
        benc_bstr_t* key = (benc_bstr_t*) ((char*)dictEntry + sizeof(benc_dict_entry_t));

        /* It is safe to drop the const here
         * because libbenc doesn't change the input.*/
        key->bytes = (char*) module->name;

        key->len = strlen(module->name);
        outBufferRequirement += key->len;
        dictEntry->key = key;

        bobj_t* valueObj = (bobj_t*) ((char*)key + sizeof(benc_bstr_t));
        valueObj->type = BENC_BSTR;
        valueObj->as.bstr = serialized;
        outBufferRequirement += serialized->len;
        dictEntry->val = valueObj;

        dictEntry->next = (benc_dict_entry_t*) ((char*)valueObj + sizeof(bobj_t));
        return 0;
    }
    forEachModule(serializeModule, NULL, registry);

    /* Otherwise libbenc is unhappy. */
    dictEntry->next = NULL;

    /* libbenc has no protection so if we miss low,
     * we get a segfault (or worse). */
    outBufferRequirement += 500;

    /* location containing bstring structure followed by it's content. */
    benc_bstr_t* out = calloc(sizeof(benc_bstr_t)
                                  + outBufferRequirement * sizeof(char),
                              1);

    if (out == NULL) {
        free(dictRoot);
        return NULL;
    }

    char* buffer = (char*) out + sizeof(benc_bstr_t);

    bbuf_t outAsBuf = {
        .len = outBufferRequirement,
        .base = buffer,
        .ptr = buffer
    };

    benc_dict_encode(&outAsBuf, dictRoot);
    out->bytes = buffer;
    out->len = outAsBuf.ptr - outAsBuf.base;

    free(dictRoot);

    if (out->len > 0) {
        return out;
    }

    free(out);

    return NULL;
}

/** @see DHTModules.h */
struct DHTModuleRegistry* DHTModules_deserialize(const benc_bstr_t serialData)
{
    bobj_t* contextByName = bobj_dict_new();
    if (contextByName == NULL) {
        return NULL;
    }

    bbuf_t dataBuf = {
        .len = serialData.len,
        .base = serialData.bytes,
        .ptr = serialData.bytes
    };

    if (benc_dict_decode(&dataBuf, &contextByName->as.dict)) {
        struct DHTModuleRegistry* reg = DHTModules_new();
        if (reg) {
            reg->serializedContexts = contextByName;
            return reg;
        }
    }

    if (contextByName) { bobj_free(contextByName); }

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
