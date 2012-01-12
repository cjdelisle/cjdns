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
/**
 * @param domain the input eg my.cool.domain.address.p2p
 * @param labelIndex the number of the label, counting BACK so label 0 will be 'p2p'.
 * @param buffer some space to work, this need not be bigger than domain.
 * @param bufferLength the size of the passed buffer.
 * @return 0 if all goes well, -1 if there aren't that many labels,
 *           -2 if the needed space exceeds the buffer length.
 */
int DNSTools_getDomainLabel(const char* domain, size_t labelIndex, char* buffer, size_t bufferLength);
