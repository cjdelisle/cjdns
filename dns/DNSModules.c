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

/** @see DNSModules.h */
benc_bstr_t* DNSModules_serialize(const struct DNSModuleRegistry* registry)
{
    int count = 0;
    int countModules(struct DNSMessage* message,
                     struct DNSModule* module,
                     const struct DNSModuleRegistry* registry)
    {
        message = message;
        registry = registry;
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

    int serializeModule(struct DNSMessage* message,
                        struct DNSModule* module,
                        const struct DNSModuleRegistry* registry)
    {
        message = message;
        registry = registry;
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

/** @see DNSModules.h */
struct DNSModuleRegistry* DNSModules_deserialize(const benc_bstr_t serialData)
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
        struct DNSModuleRegistry* reg = DNSModules_new();
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
