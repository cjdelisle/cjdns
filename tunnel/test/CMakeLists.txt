# You may redistribute this program and/or modify it under the terms of
# the GNU General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program.  If not, see <http://www.gnu.org/licenses/>.

add_executable(IpTunnel_test
    IpTunnel_test.c
)

target_link_libraries(IpTunnel_test
    cjdns-tunnel-iptunnel
    cjdbenc
    cjdmemory
    crypto
    cjdns-util-log
    cjdbenc_StandardBencSerializer
    cjdns-crypto-random
    cjdns-util-events-libevent
)
add_test(IpTunnel_test IpTunnel_test)
