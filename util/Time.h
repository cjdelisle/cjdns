#include <sys/time.h>

static inline uint64_t Time_currentTimeMilliseconds()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return ((uint64_t) tv.tv_sec) + (tv.tv_usec / 1000);
}
