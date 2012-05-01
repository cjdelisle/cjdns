#include "memory/BufferAllocator.h"
#include "dht/AddressMapper.h"

#include <stdio.h>
#include <time.h>

/* repeat each test this many times to guard against an occasional pass */
#define NUM_REPEAT 100

/* structure used by tests to report back their information */
struct TestInfo {
    /* test name */
    const char *name;
    /* set to true if the test passed (default=false) */
    bool pass;
    /* if the test failed then the failure message is given here (default=NULL) */
    const char *failMessage;
};

/* generates an incremental number for a dummy address */
static uint8_t* getDummyAddress(void)
{
    static uint8_t address[16];

    if(++address[0] == 0x00) {
        for(int i = 1; i < 15; i++) {
            if(address[i] == 0xFF) {
                address[i + 1]++;
                address[i] = 0;
            } else {
                address[i]++;
                break;
            }
        }
    }

    return address;
}

/* runTest_ReplaceLeastRecentlyUsed
 *
 * Tests that the address mapper replaces the last recently used item
 * when adding a new item to a full address map.
 *
 * 1. Fill the address map
 * 2. Add another entry
 * 3. Check that only the least recent entry was removed
 */
static int runTest_replaceLeastRecentlyUsed(struct AddressMapper *map, struct TestInfo *info)
{
    info->name = "Replace least recently used";

    /* fill the address map */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
        AddressMapper_put(label, getDummyAddress(), map);
    }

    /* santity check that the address mapper was filled */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
        int index;

        index = AddressMapper_indexOf(label, map);

        if(index == -1) {
            info->failMessage = "Address mapper could not be filled";
            return 0;
        }
    }

    /* add one more entry */
    AddressMapper_put(AddressMapper_MAX_ENTRIES + 1, getDummyAddress(), map);

    /* now check that only the first item was evicted from the address map */

    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES + 1; label++) {
        int index;

        index = AddressMapper_indexOf(label, map);

        if((label != 1) && (index == -1)) {
            info->failMessage = "An item other than the least recently used was removed";
            return 0;
        } else if ((label == 1) && (index != -1)) {
            info->failMessage = "Least recently used item was not removed";
            return 0;
        }
    }

    info->pass = true;
    return 0;
}

/* runTest_removeAndPut
 *
 * Tets the "remove" functionality of the address mapper is optimal by checking
 * that the address mapper replaces "removed" entries when new entries are added,
 * rather than enties that are still valid.
 *
 * 1. Fill the address map
 * 2. Remove a random selection of entries
 * 3. Add new entries to replace the previously removed entries
 * 4. Check that only the removed entries were replaced
 */
static int runTest_removeAndPut(struct AddressMapper *map, struct TestInfo *info)
{
    /* the labels to remove - a maximum of 8 */
    uint64_t removeLabels[2];
    info->name = "Remove and put";

    /* fill the address map */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
            AddressMapper_put(label, getDummyAddress(), map);
    }

    /* remove a random number (between 1 and 8) of labels, at random */
    unsigned int numToRemove = (rand() % 2) + 1;

    for(unsigned int i = 0; i < numToRemove; i++) {
        generateRandomLabel:
        removeLabels[i] = (rand() % AddressMapper_MAX_ENTRIES) + 1;
        /* check the label is unique */
        for(unsigned int j = 0; j < i; j++) {
            if(removeLabels[j] == removeLabels[i])
                goto generateRandomLabel;
        }


        int index;
        index = AddressMapper_indexOf(removeLabels[i], map);

        if(index == -1) {
            info->failMessage = "Could not find item which should have been "
                                                            "present in map";
            return 0;
        }

        int result;
        result = AddressMapper_remove(index, map);

        if(result != 0) {
            info->failMessage = "Could not remove item which should have been "
                                                            "present in map";
            return 0;
        }
    }

    /* put some new items in the map to replace the removed ones */
    for(unsigned int i = 1; i <= numToRemove; i++) {
        AddressMapper_put(AddressMapper_MAX_ENTRIES + i, getDummyAddress(), map);
    }

    /* check that the map contents is as expected.
     * check that all of the original items are present apart from the ones
     * that were removed
     */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
        /* find out if this item should have been removed */
        bool wasRemoved = false;

        for(unsigned int i = 0; i < numToRemove; i++) {
            if(label == removeLabels[i]) {
                wasRemoved = true;
            }
        }

        int index;

        index = AddressMapper_indexOf(label, map);

        if((wasRemoved == true) && (index != -1)) {
            info->failMessage = "Removed item still exists in the map";
            return 0;
        } else if((wasRemoved == false) && (index == -1)) {
            info->failMessage = "Non-removed item is missing from the map";
            return 0;
        }
    }

    info->pass = true;
    return 0;
}

/* runTest_dontReplaceMostRecentlyUsed
 *
 * Tests that the address mapper does not replace the most recently used item
 * when adding a new item to a full address map
 *
 * 1. Fill address map with labels 1 .. N
 * 2. Query the index of the least recently used item (label=1) in order to promote it to
 *    being the most recently used item
 * 3. Add a new entry to the address map
 * 4. Ensure it is entry with label==2 which has been removed
 */
static int runTest_dontReplaceMostRecentlyUsed(struct AddressMapper *map, struct TestInfo *info)
{
    info->name = "Don't replace most recently used";

    /* fill the address map such that the item with label==1 is the least recently used */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
        AddressMapper_put(label, getDummyAddress(), map);
    }

    /* touch item #1. #2 is now the least recently used */
    int index;
    index = AddressMapper_indexOf(1, map);

    if(index == -1) {
        info->failMessage = "Failed to fill address map - first item was dropped";
    }

    /* add a new entry and check that #1 was not replaced.
     * check that #2 was replaced.
     */

    AddressMapper_put(AddressMapper_MAX_ENTRIES + 1, getDummyAddress(), map);

    index = AddressMapper_indexOf(1, map);

    if(index == -1) {
        info->failMessage = "Address mapper wrongly removed #1 when it was not the "
                                                          "least recently used";
        return 0;
    }

    index = AddressMapper_indexOf(2, map);

    if(index != -1) {
        info->failMessage = "Address mapper replaced an item other than the "
                                                    "least recently used";
        return 0;
    }

    info->pass = true;
    return 0;
}

static uint64_t rand64(void)
{
    uint64_t r;

    r = (uint32_t) rand();
    r <<= 32;
    r |= (uint32_t) rand();
    return r;
}

/* runTest_orderCheck
 *
 * Tests that the address mapper properly maintains its entries in order of most
 * recently used and replaces entries in order of least recently used.
 *
 * 1. Fill address mapper
 * 2. Shuffle the contents of the address map
 * 2. Queries the address mapper in a random order
 * 3. Add new entries to the map such that only the most recently used
 *    of the previously entries should still be presnt.
 * 4. Query the address map to see if only the most recently used item remains.
 */
static int runTest_orderCheck(struct AddressMapper *map, struct TestInfo *info)
{
    info->name = "Order check";

    /* the labels used for this test, stored in a random order */
    uint64_t *labels;
    const uint64_t labelMin = 0xF000000000000000;
    const uint64_t labelMax = 0xFFFFFFFFFFFFFFFF;

    labels = calloc(AddressMapper_MAX_ENTRIES, sizeof(uint64_t));

    if(labels == NULL) {
        fprintf(stderr, "Failed to allocate memory\n");
        return -1;
    }

    /* now generate random unique labels and add each one to the map */

    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        generateLabel:
        labels[i] = (rand64() % (labelMax - labelMin + 1)) + labelMin;

        /* check the label is unique */
        for(int j = 0; j < i; j++) {
            if(labels[i] == labels[j])
                goto generateLabel;
        }

        AddressMapper_put(labels[i], getDummyAddress(), map);
    }

    /* shuffle the map */
    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        int index;
        uint64_t randomLabel;

        randomLabel = labels[rand() % AddressMapper_MAX_ENTRIES];
        index = AddressMapper_indexOf(randomLabel, map);

        if(index == -1) {
            info->failMessage = "Map has lost an element";
            goto out;
        }
    }

    /* query the items in a random order.
     * after this, labels[0] will be the least recently used
     * item and labels[AddressMapper_MAX_ENTRIES - 1] will be the
     * most recently used
     */
    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        uint64_t tmp;
        size_t r;

        /* swap labels[i] with a random element */
        r = rand() % AddressMapper_MAX_ENTRIES;

        tmp = labels[i];
        labels[i] = labels[r];
        labels[r] = tmp;

        int index;
        index = AddressMapper_indexOf(labels[i], map);

        if(index == -1) {
            info->failMessage = "Map has lost an element";
            goto out;
        }
    }

    /* now check the items are replaced in the correct order.
     * The new labels are guaranteed to be unique from the labels already
     * in the map.
     * After this action there should be just one item left,
     * the most recently used item from above - this is the
     * last item in the shuffled array.
     */
    for(int i = 0; i < AddressMapper_MAX_ENTRIES - 1; i++) {
        /* add a new entry */
        uint64_t newLabel = labels[i] - labelMin;
        AddressMapper_put(newLabel, getDummyAddress(), map);
    }

    /* verify that only most recently used item remains, as expected */
    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        int index;
        index = AddressMapper_indexOf(labels[i], map);

        /* items < (AddressMapper_MAX_ENTRIES - 1) should have been replaced as least used */
        if((i < (AddressMapper_MAX_ENTRIES - 1)) && (index != -1)) {
            info->failMessage = "Element was not removed as expected";
            goto out;
        }

        /* item == (AddressMapper_MAX_ENTRIES - 1) should not have been replaced */
        if((i == (AddressMapper_MAX_ENTRIES - 1)) && (index == -1)) {
            info->failMessage = "Unexpected item was lost";
            goto out;
        }
    }

    info->pass = true;
out:
    free(labels);
    return 0;
}

/* collection of data required to run tests */
struct AppState {
    void *buffer;
    struct AddressMapper* map;
    struct Allocator* allocator;
};

static int initAppState(struct AppState *state)
{
    const size_t bufferSize = 64 * 1024;

    state->buffer = malloc(bufferSize);

    if(state->buffer == NULL) {
        fprintf(stderr, "Failed to allocate buffer\n");
        goto errorExitMalloc;
    }

    state->allocator = BufferAllocator_new(state->buffer, bufferSize);

    if(state->allocator == NULL) {
        fprintf(stderr, "Failed to create allocator\n");
        goto errorExitAllocator;
    }

    state->map = AddressMapper_new(state->allocator);

    if(state->map == NULL) {
        fprintf(stderr, "Failed to create address map\n");
        goto errorExitMap;
    }

    return 0;

errorExitMap:
    state->allocator->free(state->allocator);
errorExitAllocator:
    free(state->buffer);
errorExitMalloc:
    return -1;
}

static void deinitAppState(struct AppState *state)
{
    state->allocator->free(state->allocator);
    free(state->buffer);
}

/* if the user passed an unsigned integer then use that as a seed,
 * otherwise use the current time
 */
static void applySeed(int argc, char **argv)
{
    unsigned int seedValue;

    if(argc <= 1) {
        seedValue = time(NULL);
    } else {
        int n;
        n = sscanf(argv[1], "%u", &seedValue);
        if(n != 1) {
            fprintf(stderr, "Invalid seed value: %s\n", argv[1]);
            abort();
        }
    }

    srand(seedValue);
}

/* list of tests to run */
static int (*testRunList[])(struct AddressMapper *map, struct TestInfo *info) = {
                    runTest_replaceLeastRecentlyUsed,
                    runTest_dontReplaceMostRecentlyUsed,
                    runTest_removeAndPut,
                    runTest_orderCheck,
                    NULL
};


int main(int argc, char** argv)
{
    int err = 0;
    struct AppState state;

    err = initAppState(&state);

    if(err != 0) {
        goto errorExitAppState;
    }

    applySeed(argc, argv);

    int testNum = 0;
    while(testRunList[testNum] != NULL) {
        struct TestInfo info;
        for(int rpt = 0; rpt < NUM_REPEAT; rpt++) {
            int testErr;

            memset(&info, 0, sizeof(info));
            testErr = testRunList[testNum](state.map, &info);

            if(testErr != 0) {
                fprintf(stderr, "Failed to initialise test: %s\n", info.name);
                goto errorExitTest;
            }

            /* if the test failed then print its error message now and move on to the next test */
            if(info.pass == false) {
                printf("%s: Failed", info.name);
                if(info.failMessage != NULL) {
                    printf(" - %s.", info.failMessage);
                }
                printf("\n");
                /* nonzero exit code if a test has failed */
                err = -1;
                goto nextTest;
            }
        }

        nextTest:
        testNum++;
    }

errorExitTest:
    deinitAppState(&state);
errorExitAppState:
    return err;
}
