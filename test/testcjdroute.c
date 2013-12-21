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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#define string_strlen
#define string_strcmp
#include "util/Assert.h"
#include "util/platform/libc/string.h"
#include "util/events/Time.h"
#include "util/events/EventBase.h"
#include "memory/MallocAllocator.h"

#include <stdio.h>

<?js
    var done = this.async();
    var Tests = require("./Tests");
    Tests.get(function (tests) {
        var prototypes = [];
        var listContent = [];
        tests.forEach(function (test) {
            var main = /^.*\/([^\/]+)\.c$/.exec(test)[1] + '_main';
            (builder.config['cflags'+test] =
                builder.config['cflags'+test] || []).push('-D', 'main='+main);
            file.links.push(test);
            listContent.push('{ .func = '+main+', .name = "'+test.replace(/^.*\/|.c$/g, '')+'" },');
            prototypes.push('int '+main+'(int argc, char** argv);');
        });
        file.testcjdroute_tests = listContent.join('\n');
        file.testcjdroute_prototypes = prototypes.join('\n');
        done();
    });
?>

<?js return file.testcjdroute_prototypes; ?>

 typedef int (* Test)(int argc, char** argv);

static struct {
    Test func;
    char* name;
} TESTS[] = {
    <?js return file.testcjdroute_tests ?>
};

static uint64_t runTest(Test test,
                        char* name,
                        uint64_t startTime,
                        int argc,
                        char** argv,
                        struct EventBase* base)
{
    fprintf(stderr, "Running test %s", name);
    Assert_always(!test(argc, argv));
    uint64_t now = Time_hrtime();
    char* seventySpaces = "                                                                      ";
    int count = strlen(name);
    if (count > 69) { count = 69; }
    fprintf(stderr, "%s%d.%d ms\n",
            &seventySpaces[count],
            (int)((now - startTime)/1000000),
            (int)((now - startTime)/1000)%1000);
    return now;
}

void usage(char* appName)
{
    printf("%s <test>     run one test\n", appName);
    printf("%s all        run every test\n\n", appName);
    printf("Available Tests:\n");
    for (int i = 0; i < (int)(sizeof(TESTS)/sizeof(*TESTS)); i++) {
        printf("%s\n", TESTS[i].name);
    }
}

int main(int argc, char** argv)
{
    struct Allocator* alloc = MallocAllocator_new(4096);
    struct EventBase* base = EventBase_new(alloc);
    uint64_t now = Time_hrtime();
    uint64_t startTime = now;
    if (argc < 2) {
        Assert_always(argc > 0);
        usage(argv[0]);
        return 100;
    }
    if (strcmp("all", argv[1])) {
        for (int i = 0; i < (int)(sizeof(TESTS)/sizeof(*TESTS)); i++) {
            if (!strcmp(TESTS[i].name, argv[1])) {
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
