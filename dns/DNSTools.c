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
