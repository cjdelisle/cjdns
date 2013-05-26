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

set(ethif "")
if(HAS_ETH_INTERFACE)
    add_library(cjdns-interface-eth
        ETHInterface_${SYSTEM}.c
        ETHInterface_admin.c
    )
    target_link_libraries(cjdns-interface-eth
        cjdns-util-events-libuv
    )
    set(ethif cjdns-interface-eth)
endif()

add_library(interface
    UDPInterface.c
    UDPInterface_admin.c
    InterfaceConnector.c
    SessionManager.c
    ICMP6Generator.c
    MultiInterface.c
    FramingInterface.c
)

target_link_libraries(interface
    ${ethif}
    cjdns-interface-tuntap
    util
    cjdns-crypto-key
    cjdns-util-platform-socket
)

add_subdirectory(addressable)
add_subdirectory(tuntap)

enable_testing()
add_subdirectory(test)
