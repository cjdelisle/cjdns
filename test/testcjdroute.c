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
                        char** argv,
                        struct EventBase* base)
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
    printf("Available Fuzz Tests:\n");
    for (int i = 0; i < FUZZ_CASE_COUNT; i++) {
        printf("%s fuzz < %s\n", appName, FUZZ_CASES[i]);
    }
}

static struct Message* readStdin(struct Allocator* alloc)
{
    uint8_t buff[4096] = { 0 };
    ssize_t length = read(STDIN_FILENO, buff, 4096);
    if (length >= 4096) {
        printf("No test files over 4096 bytes\n");
        length = 0;
    }
    struct Message* msg = Message_new(length, 128, alloc);
    Bits_memcpy(msg->bytes, buff, length);
    return msg;
}

static void** initFuzzTests(struct Allocator* alloc, struct Random* rand, struct EventBase* base)
{
    void** contexts = Allocator_calloc(alloc, sizeof(char*), FUZZ_TEST_COUNT);
    for (int i = 0; i < FUZZ_TEST_COUNT; i++) {
        contexts[i] = FUZZ_TESTS[i].init(alloc, rand);
    }
    return contexts;
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

static int fuzzMain()
{
    struct Allocator* alloc = MallocAllocator_new(1<<24);
    struct EventBase* base = EventBase_new(alloc);
    struct RandomSeed* rs = DeterminentRandomSeed_new(alloc, RANDOM_SEED);
    struct Random* rand = Random_newWithSeed(alloc, NULL, rs, NULL);

#ifdef __AFL_INIT
    // Enable AFL deferred forkserver mode. Requires compilation using afl-clang-fast
    void** ctxs = initFuzzTests(alloc, rand, base);
    __AFL_INIT();
    #define CTX(selector) ctxs[selector]
#else
    // avoid warning
    if (0) { initFuzzTests(alloc, rand, base); }
    #define CTX(selector) FUZZ_TESTS[selector].init(alloc, rand)
#endif

    struct Message* fuzz = readStdin(alloc);
    if (fuzz->length < 4) { return 100; }
    uint32_t selector = Message_pop32(fuzz, NULL);
    printf("x\n");
    if (selector >= FUZZ_TEST_COUNT) {
        printf("selector [%x] out of bounds [%u]\n", selector, FUZZ_TEST_COUNT);
        return 100;
    }
    printf("Running [%s] ([%u])\n", FUZZ_TESTS[selector].name, selector);
    void* ctx = CTX(selector);
    FUZZ_TESTS[selector].fuzz(ctx, fuzz);
    Allocator_free(alloc);
    return 0;
}
/*
static int mkFuzz()
{
    struct Allocator* alloc = MallocAllocator_new(1<<24);
    for (int i = 0; i < FUZZ_TEST_COUNT; i++) {
        printf("Making test data [%s]\n", FUZZ_TESTS[i].name);
        struct Allocator* child = Allocator_child(alloc);
        struct FuzzTest* ft = FUZZ_TESTS[i].mkFuzz(child);
        uint32_t i_be = Endian_hostToBigEndian32(i);
        while (ft) {
            int f = open(ft->name, O_WRONLY | O_CREAT | O_TRUNC, 0666);
            if (f == -1) { perror(""); }
            Assert_true(f > -1);
            Assert_true(write(f, &i_be, 4) == 4);
            Assert_true(write(f, ft->fuzz->bytes, ft->fuzz->length) == ft->fuzz->length);
            close(f);
            ft = ft->next;
        }
        Allocator_free(child);
    }
    return 0;
}*/

int main(int argc, char** argv)
{
    if (argc > 1 && !CString_strcmp("fuzz", argv[1])) { return fuzzMain(); }
    struct Allocator* alloc = MallocAllocator_new(4096);
    struct EventBase* base = EventBase_new(alloc);
    uint64_t now = Time_hrtime();
    uint64_t startTime = now;
    if (argc < 2) {
        Assert_true(argc > 0);
        usage(argv[0]);
        return 100;
    }
    if (argc > 1 && CString_strcmp("all", argv[1])) {
        for (int i = 0; i < (int)(sizeof(TESTS)/sizeof(*TESTS)); i++) {
            if (!CString_strcmp(TESTS[i].name, argv[1])) {
                TESTS[i].func(argc, argv);
                return 0;
            }
        }
        usage(argv[0]);
        return 100;
    }
    for (int i = 0; i < (int)(sizeof(TESTS)/sizeof(*TESTS)); i++) {
        now = runTest(TESTS[i].func, TESTS[i].name, now, argc, argv, base);
    }
    fprintf(stderr, "Total test time %d.%d ms\n",
            (int)((now - startTime)/1000000),
            (int)((now - startTime)/1000)%1000);
    Allocator_free(alloc);
    return 0;
}
