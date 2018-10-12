/* vim: set expandtab ts=4 sw=4: */
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
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
 */
#include "crypto/random/Random.h"
#include "crypto/random/test/DeterminentRandomSeed.h"
#include "util/Assert.h"
#include "util/events/Time.h"
#include "util/events/EventBase.h"
#include "util/CString.h"
#include "memory/MallocAllocator.h"
#include "wire/Message.h"
#include "test/FuzzTest.h"
#include "util/Js.h"

#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

#ifdef SUBNODE
    #define testcjdroute_SUBNODE 1
#else
    #define testcjdroute_SUBNODE 0
#endif

Js({
    require("./test/testcjdroute.js").generate(
        file, builder, testcjdroute_SUBNODE, this.async());
})
Js({ return file.testcjdroute_prototypes; })

typedef int (* Test)(int argc, char** argv);
typedef void* (* FuzzTestInit)(struct Allocator* alloc, struct Random* rand);
typedef void (* FuzzTest)(void* ctx, struct Message* fuzz);
typedef struct FuzzTest* (* MkFuzz)(struct Allocator* alloc);

static const struct {
    Test func;
    char* name;
} TESTS[] = { Js({ return file.testcjdroute_tests }) };
static const int TEST_COUNT = (int) (sizeof(TESTS) / sizeof(*TESTS));

static const struct {
    FuzzTestInit init;
    FuzzTest fuzz;
    char* name;
} FUZZ_TESTS[] = { Js({ return file.testcjdroute_fuzzTests }) };
static const int FUZZ_TEST_COUNT = (int) (sizeof(FUZZ_TESTS) / sizeof(*FUZZ_TESTS));

static const char* FUZZ_CASES[] = { Js({ return file.testcjdroute_fuzzCases }) };
static const int FUZZ_CASE_COUNT = (int) (sizeof(FUZZ_CASES) / sizeof(*FUZZ_CASES));


static uint64_t runTest(Test test,
                        char* name,
                        uint64_t startTime,
                        int argc,
                        char** argv)
{
    fprintf(stderr, "Running test %s", name);
    Assert_true(!test(argc, argv));
    uint64_t now = Time_hrtime();
    char* seventySpaces = "                                                                      ";
    int count = CString_strlen(name);
    if (count > 69) { count = 69; }
    fprintf(stderr, "%s%d.%d ms\n",
            &seventySpaces[count],
            (int)((now - startTime)/1000000),
            (int)((now - startTime)/1000)%1000);
    return now;
}

static void usage(char* appName)
{
    printf("%s <test>     run one test\n", appName);
    printf("%s all        run every test\n\n", appName);
    printf("Available Tests:\n");
    for (int i = 0; i < TEST_COUNT; i++) {
        printf("%s\n", TESTS[i].name);
    }
    printf("\nAvailable Fuzz Tests:\n");
    for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
        printf("%s fuzz < %s\n", appName, FUZZ_CASES[i]);
    }
}

static struct Message* readFile(int fileNo, struct Allocator* alloc)
{
    uint8_t buff[4096] = { 0 };
    ssize_t length = read(fileNo, buff, 4096);
    if (length >= 4096) {
        printf("No test files over 4096 bytes\n");
        length = 0;
    }
    int capacity = length;
    while (capacity % 8) { capacity++; }
    struct Message* msg = Message_new(capacity, 128, alloc);
    msg->length = length;
    Bits_memcpy(msg->bytes, buff, length);
    return msg;
}

static void** initFuzzTests(struct Allocator* alloc, struct Random* rand)
{
    void** contexts = Allocator_calloc(alloc, sizeof(char*), FUZZ_TEST_COUNT);
    for (int i = 0; i < FUZZ_TEST_COUNT; i++) {
        contexts[i] = FUZZ_TESTS[i].init(alloc, rand);
    }
    return contexts;
}

static int runFuzzTest(
    void** ctxs,
    struct Allocator* alloc,
    struct Random* rand,
    struct Message* fuzz,
    const char* testCase)
{
    if (fuzz->length < 4) { return 100; }
    uint32_t selector = Message_pop32(fuzz, NULL);
    if (selector >= FUZZ_TEST_COUNT) {
        printf("selector [%x] out of bounds [%u]\n", selector, FUZZ_TEST_COUNT);
        return 100;
    }
    if (!testCase) { testCase = FUZZ_TESTS[selector].name; }
    fprintf(stderr, "Running fuzz %s", testCase);
    void* ctx = ctxs ? ctxs[selector] : FUZZ_TESTS[selector].init(alloc, rand);
    FUZZ_TESTS[selector].fuzz(ctx, fuzz);
    return 0;
}

static uint64_t runFuzzTestManual(
    struct Allocator* alloc,
    struct Random* detRand,
    const char* testCase,
    uint64_t startTime)
{
    int f = open(testCase, O_RDONLY);
    Assert_true(f > -1);
    struct Message* fuzz = readFile(f, alloc);
    close(f);

    runFuzzTest(NULL, alloc, detRand, fuzz, testCase);

    uint64_t now = Time_hrtime();
    char* seventySpaces = "                                                                      ";
    int count = CString_strlen(testCase);
    if (count > 69) { count = 69; }
    fprintf(stderr, "%s%d.%d ms\n",
            &seventySpaces[count],
            (int)((now - startTime)/1000000),
            (int)((now - startTime)/1000)%1000);
    return now;
}

// We don't really want to let AFL write the random seed because the amount of mixing
// that occurs between the input and output makes any attempt at optimizing the seed
// useless.
//
// We do, however, want to make sure that crashes discovered by AFL are reproducable.
//
// We might imagine letting part of the AFL message content be the random data which
// is returned, but we don't know how much will be requested in advance. Possibly
// something for the future.
#define RANDOM_SEED "0123456789abcdef0123456789abcdef0123456789abcdef0123456789abcdef"

static int fuzzMain(struct Allocator* alloc, struct Random* detRand)
{
    void** ctxs = NULL;
#ifdef __AFL_INIT
    // Enable AFL deferred forkserver mode. Requires compilation using afl-clang-fast
    ctxs = initFuzzTests(alloc, detRand);
    __AFL_INIT();
#else
    // avoid warning
    if (0) { initFuzzTests(alloc, detRand); }
#endif

    struct Message* fuzz = readFile(STDIN_FILENO, alloc);
    int out = runFuzzTest(ctxs, alloc, detRand, fuzz, NULL);
    printf("\n");
    return out;
}

static int main2(int argc, char** argv, struct Allocator* alloc, struct Random* detRand)
{
    if (argc > 1 && !CString_strcmp("fuzz", argv[1])) { return fuzzMain(alloc, detRand); }
    uint64_t now = Time_hrtime();
    uint64_t startTime = now;
    if (argc < 2) {
        Assert_true(argc > 0);
        usage(argv[0]);
        return 100;
    }
    if (argc > 1 && CString_strcmp("all", argv[1])) {
        for (int i = 0; i < TEST_COUNT; i++) {
            if (!CString_strcmp(TESTS[i].name, argv[1])) {
                TESTS[i].func(argc, argv);
                return 0;
            }
        }
        for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
            if (!CString_strcmp(FUZZ_CASES[i], argv[1])) {
                runFuzzTestManual(alloc, detRand, FUZZ_CASES[i], now);
                return 0;
            }
        }
        usage(argv[0]);
        return 100;
    }
    for (int i = 0; i < TEST_COUNT; i++) {
        now = runTest(TESTS[i].func, TESTS[i].name, now, argc, argv);
    }
    for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
        // TODO(cjd): Apparently a race condition in the allocator
        // if you have async freeing in progress and then you come in and
        // free the root allocator, you get an assertion.
        //
        //struct Allocator* child = Allocator_child(alloc);
        struct Allocator* child = MallocAllocator_new(1<<24);

        now = runFuzzTestManual(child, detRand, FUZZ_CASES[i], now);
        Allocator_free(child);
    }
    fprintf(stderr, "Total test time %d.%d ms\n",
            (int)((now - startTime)/1000000),
            (int)((now - startTime)/1000)%1000);
    return 0;
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(1<<24);
    struct RandomSeed* rs = DeterminentRandomSeed_new(alloc, RANDOM_SEED);
    struct Random* detRand = Random_newWithSeed(alloc, NULL, rs, NULL);
    int out = main2(argc, argv, alloc, detRand);
    Allocator_free(alloc);
    return out;
}
