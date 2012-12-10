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
find_package(Libevent2 REQUIRED)

if(WIN32)
  set(tunif TUNInterface_W32.c)
  set(tunconf TUNConfigurator_W32.c)
else()
  set(tunif TUNInterface.c)
endif()

set(ethif "")
if(HAS_ETH_INTERFACE)
    set(ethif ETHInterface_${SYSTEM}.c ETHInterface_admin.c)
endif()

add_library(interface
    UDPInterface.c
    UDPInterface_admin.c
    ${ethif}
    ${tunif}
    TUNConfigurator_${SYSTEM}.c
    InterfaceConnector.c
    SessionManager.c
    ICMP6Generator.c
    MultiInterface.c
)

target_link_libraries(interface
    util
    cjdns-crypto-key
    cjdns-util-events-libevent
    cjdns-util-platform-socket
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/TUNMessageType_${SYSTEM}.c)
    set(TUNMessageType TUNMessageType_${SYSTEM}.c)
else()
    set(TUNMessageType TUNMessageType.c)
endif()

add_library(cjdns-interface-tun-messagetype
    ${TUNMessageType}
)

add_library(cjdns-interface-pipeinterface
    PipeInterface.c
)

enable_testing()
add_subdirectory(test)
