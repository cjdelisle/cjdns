#include <time.h>

int64_t DHTStoreTools_currentTimeMinutes()
{
    return time(NULL) / 60;
}
