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

add_library(cjdns-waiter
    Waiter.c
)

add_library(cjdns-interface-waiter
    InterfaceWaiter.c
)

add_library(cjdns-angel
    Angel.c
    AngelInit.c
)

target_link_libraries(cjdns-angel
    cjdbenc
    cjdbenc_StandardBencSerializer
    cjdmemory
    ${LIBEVENT2_LIBRARIES}
    cjdns-security
    cjdns-process
    cjdns-interface-waiter
    cjdns-waiter
    util # Hex_decode()
    cjdns-util-log-writer
    cjdns-crypto-random
)

add_library(cjdns-core
    Core.c
    Core_admin.c
)

target_link_libraries(cjdns-core
    crypto
    interface
    switch
    dht
    dhtcore
    cjdbenc
    cjdbenc_StandardBencSerializer
    cjdmemory
    cjdns-admin
    cjdnet
    ${LIBEVENT2_LIBRARIES}
    ${PLATFORM_LIBRARIES}
    cjdns-interface-waiter
    cjdns-util-log-writer
    cjdns-admin-logger
    cjdns-util-log-indirect
    cjdns-tunnel-iptunnel
    cjdns-crypto-random
)

add_executable(cjdns
    Cjdns.c
)

target_link_libraries(cjdns cjdns-angel cjdns-core)

add_executable(cjdroute2
    cjdroute2.c
)

target_link_libraries(cjdroute2
    cjdns-admin-client
    crypto
    cjdns-crypto-bench
    cjdbenc_StandardBencSerializer
    cjdbenc_JsonBencSerializer
    cjdns-process
    cjdns-waiter
    cjdns-util-log-writer
    cjdns-crypto-random
)



enable_testing()
#add_subdirectory(test)
