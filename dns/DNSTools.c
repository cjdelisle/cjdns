#include <stddef.h>
#include <string.h>

int DNSTools_getDomainLabel(const char* domain, int labelIndex, char* buffer, size_t bufferLength)
{
    int i = strlen(domain);
    int dotIndex = i;
    int dotCount = 0;
    for (i--; ; i--) {
        if (domain[i] == '.' || i <= 0) {
            if (dotCount == labelIndex) {

                /* Move off of the . */
                if (domain[i] == '.') {
                    i++;
                }

                if (dotIndex - i > (int) bufferLength) {
                    return -2;
                }
                /* Copy to buffer */
                memcpy(buffer, domain + i, dotIndex - i);

                buffer[dotIndex - i] = '\0';
                return 0;
            }
            dotCount++;
            dotIndex = i;

            if (i <= 0) {
                break;
            }
        }
    }
    return -1;
}
