#include "memory/BufferAllocator.h"
#include "dht/AddressMapper.h"

#include <stdio.h>
#include <time.h>

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


/* runTest_ReplaceLRU:
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

/* runTest_dontReplaceMostRecentlyUsed
 *
 * Tests that the address mapper does not replace the most recently used item
 * when adding a new item to a full address map
 *
 * 1. Fill address map with labels 1 .. N
 * 2. Query the index of the least recently used item, in order to promote it to
 *    being the most recently used item
 * 3. Add a new entry to the address map
 * 4. Ensure it is entry with label==2 which has been removed
 */
static int runTest_dontReplaceMostRecentlyUsed(struct AddressMapper *map, struct TestInfo *info)
{
    /* fill the address map such that the item with label==1 is the least recently used */
    for(unsigned int label = 1; label <= AddressMapper_MAX_ENTRIES; label++) {
        AddressMapper_put(label, getDummyAddress(), map);
    }

    /* touch item #1. #2 is now the least recently used */
    int index;
    index = AddressMapper_indexOf(1, map);
    assert(index != -1);

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

/* runTest_OrderCheck
 *
 * Tests that the address mapper properly maintains its entries in order of most
 * recently used.
 *
 * 1. Fill address mapper
 * 2. Shuffle the contents of the address map
 * 2. Queries the address mapper in a random order
 * 3. Add new entries to the map until it is full, each time testing to make
 *    sure that only the least recently used item was removed
 */
static int runTest_OrderCheck(struct AddressMapper *map, struct TestInfo *info)
{
    /* the labels used for this test, stored in a random order */
    uint64_t *labels;
    const uint64_t labelMin = 0xF000000000000000;
    const uint64_t labelMax = 0xFFFFFFFFFFFFFFFF;

    labels = calloc(AddressMapper_MAX_ENTRIES, sizeof(uint64_t));

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
            return 0;
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
            return 0;
        }
    }

    /* now check the items are replaced in the correct order.
     * The new labels are guaranteed to be unique from the labels already
     * in the map
     */
    for(int i = 0; i < AddressMapper_MAX_ENTRIES; i++) {
        /* add a new entry */
        uint64_t newLabel = labels[i] - labelMin;
        AddressMapper_put(newLabel, getDummyAddress(), map);

        /* test that only the least recently used label was removed */
        for(int j = 0; j < AddressMapper_MAX_ENTRIES; j++) {
            int index;
            index = AddressMapper_indexOf(labels[j], map);

            /* items <=i should have been replaced as least used */
            if((j <= i) && (index != -1)) {
                info->failMessage = "Element was not removed as expected";
                return 0;
            }

            /* items > i should not have been replaced yet */
            if((j > i) && (index == -1)) {
                info->failMessage = "Unexpected item was lost";
                return 0;
            }
        }
    }

    info->pass = true;
    return 1;

}

/* list of tests to run */
static int (*testRunList[])(struct AddressMapper *map, struct TestInfo *info) = {
                    &runTest_replaceLeastRecentlyUsed,
                    runTest_dontReplaceMostRecentlyUsed,
                    runTest_OrderCheck,
                    NULL
};


int main(int argc, char** argv)
{
    int testNum = 0;

    srand(time(NULL));

    while(testRunList[testNum] != NULL) {
        testRunList[testNum](NULL, NULL);
    }


    return 0;
}
