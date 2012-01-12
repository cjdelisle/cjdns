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
#include <string.h>
#include <stdio.h>
#include "dns/DNSTools.h"

int testGetDomainLabel()
{
    const char* domain = "this.is.a.domain.which.you.might.see.on.irc.tld";
    const char* labels[11] =
        {"tld", "irc", "on", "see", "might", "you", "which", "domain", "a", "is", "this"};
    int i = 0;
    int ret = 0;
    char buffer[256];
    while (ret != -1) {
        ret = DNSTools_getDomainLabel(domain, i, buffer, 256);
        if (ret != -1) {
            if (strlen(labels[i]) != strlen(buffer) || strcmp(labels[i], buffer) != 0) {
                printf("Label was incorrect\n");
                return -1;
            }
        } else if (i != 11) {
            printf("First index after last label was wrong, expected 11, got %d\n", i);
            return -1;
        }
        i++;
    }

    if (DNSTools_getDomainLabel(domain, 100, buffer, 256) != -1) {
        /* index out of bounds. */
        printf("Should have failed with index out of bounds but did not\n");
        return -1;
    }
    if (DNSTools_getDomainLabel(domain, 7, buffer, 5) != -2) {
        /* Buffer too small. */
        printf("Should have failed from insifficient buffer size but did not\n");
        return -2;
    }

    return 0;
}

int main()
{
    return testGetDomainLabel();
}
