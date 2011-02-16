#include <stdio.h>

int assertTrue(int mustBeTrue, const char *failureMessage)
{
    if (!mustBeTrue) {
        printf(failureMessage);
        return -1;
    }
    return 0;
}
