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

add_library(cjdns-interface-waiter
    InterfaceWaiter.c
)
target_link_libraries(cjdns-interface-waiter
    cjdns-util-events-libuv-time
)

add_library(cjdns-angel
    Angel.c
    AngelInit.c
)

target_link_libraries(cjdns-angel
    cjdbenc
    cjdbenc_StandardBencSerializer
    cjdmemory
    cjdns-util-events-libuv
    cjdns-security
    cjdns-interface-waiter
    util # Hex_decode(), Pipe_createUniPipe()
    cjdns-util-log-writer
    cjdns-crypto-random
    cjdns-util-platform-socket
    cjdns-interface-addressable-adapter
    cjdns-interface-tuntap
    cjdns-util-platform-netdev
    interface
)

add_library(cjdns-admin-hermes
    Hermes.c
)

add_library(cjdns-core
    Core.c
)

target_link_libraries(cjdns-core
    cjdns-admin-hermes
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
    cjdns-util-events-libuv
    cjdns-interface-waiter
    cjdns-util-log-filewriter
    cjdns-admin-logger
    cjdns-util-log-indirect
    cjdns-tunnel-iptunnel
    cjdns-crypto-random
    cjdns-crypto-random-libuv
)

add_executable(cjdns
    Cjdns.c
)

target_link_libraries(cjdns cjdns-angel cjdns-core)

add_executable(cjdroute2
    cjdroute2.c
)

target_link_libraries(cjdroute2
    cjdns-angel
    cjdns-core

    cjdns-admin-client
    crypto
    cjdns-crypto-bench
    cjdns-util-events-libuv
    cjdbenc_StandardBencSerializer
    cjdbenc_JsonBencSerializer
    cjdns-util-log-writer
    cjdns-crypto-random
    cjdns-util-version-version
    cjdns-interface-waiter
)

install(TARGETS cjdroute2
    RENAME cjdroute
    RUNTIME DESTINATION bin
)

enable_testing()
#add_subdirectory(test)
