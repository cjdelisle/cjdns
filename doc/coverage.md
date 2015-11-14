# Test Coverage

Test coverage is for determining how many (what percentage) of lines of code in the project have
been executed by a given test. This is a BAD METRIC, you can have 100% test coverage on a project
that still doesn't work ! Keep in mind before you go off joining the church of industrial software
design, there are a lot of people who take these things extremely seriously and they are annoying.

Test coverage can provide you with 2 pieces of information, 1. where the tests are not reaching
and 2. where there might be dead code which is never executed (and should therefore be deleted).

## Checking coverage with gcov

1. Build with GCOV=1 aka `GCOV=1 ./do`  (make sure you are using gcc to compile)
2. For the file you are interested in, find the corresponding .i file in the build directory,
for `net/InterfaceController.c` the file will be `./build_linux/net_InterfaceController_c.i`
3. run gcov on the .i file but **without the .i extension** for example:
`$ gcov-5 ./build_darwin/net_InterfaceController_c`
You should see a result which looks like the following:

    File 'net/InterfaceController.c'
    Lines executed:55.58% of 412
    Creating 'InterfaceController.c.gcov'

4. review the resulting .gcov file `atom ./InterfaceController.c.gcov` lines which were not touched
by any test will have `#####` before them.

## Checking coverage for a particular test

Same as the global coverage check except you will want to disable tests at the beginning so you
do not check for all tests `GCOV=1 NO_TEST=1 ./do` then run the test you want
`./build_linux/test_testcjdroute_c CryptoAuth_fuzz_test` then run gcov on the file you want to
check.

## Last but not least

Clean up the enormous pile of crap which gcov generates !
