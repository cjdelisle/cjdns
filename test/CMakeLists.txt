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

add_library(TestFramework TestFramework.c)
target_link_libraries(TestFramework
    crypto
    interface
    switch
    dht
    dhtcore
    cjdbenc
    cjdbenc_JsonBencSerializer
    cjdbenc_StandardBencSerializer
    cjdmemory
    cjdns-admin
    cjdnet
    cjdns-util-events-libuv
    cjdns-tunnel-iptunnel
    cjdns-crypto-random
    cjdns-memory-canary
)


set(Test_FILES
    cjdroute_injection_test.c
    cjdroute_routerPing_test.c
    CryptoAddress_test.c
    twoNodes_test.c
    threeNodes_test.c
)
set(Test_LIBRARIES
    TestFramework
)
include(${CMAKE_SOURCE_DIR}/cmake/modules/Test.cmake)
