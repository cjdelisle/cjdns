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
  if(LINUX)
    set(tunconf TUNConfigurator_Linux.c)
  elseif(APPLE)
    set(tunconf TUNConfigurator_OSX.c)
  elseif(ILLUMOS)
    add_definitions(-D Illumos=1)
    set(tunconf TUNConfigurator_Illumos.c)
  else()
    message(FATAL_ERROR "no TUN adapter to handle your system [${CMAKE_SYSTEM_NAME}]")
  endif()
endif()

add_library(interface
    UDPInterface.c
    UDPInterface_admin.c
    ETHInterface.c
    ETHInterface_admin.c
    ${tunif}
    ${tunconf}
    InterfaceConnector.c
    SessionManager.c
)

target_link_libraries(interface util ${LIBEVENT2_LIBRARIES})

enable_testing()
add_subdirectory(test)
