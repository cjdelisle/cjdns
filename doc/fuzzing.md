# Fuzzing !
(testing by trying a lot of random stuff)

Cjdns has a few fuzz tests, at the time of this writing these include FramingIface_fuzz_test
and CryptoAuth_fuzz_test.

The standard when developing a fuzz test is to name it WhateverBlah_fuzz_test.c

Fuzz tests are configured by default to run for only a few milliseconds on a fast machine so that
they do not take an extraordinary amount of time on a small device.

To run a fuzz test for longer, you should pass the parameter `--fuzz` in the command line when
running the test executable.

For example:

    ./build_linux/test_testcjdroute_c CryptoAuth_fuzz_test --fuzz

This will run the test a number of cycles which corresponds to about 10 seconds of testing on a
fast machine (mine). You can expect a line like the following to be printed at the end:

    1445844036 DEBUG CryptoAuth_fuzz_test.c:333 ===+++=== Completed Ok ===++++===
