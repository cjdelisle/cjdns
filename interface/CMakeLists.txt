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
  set(tunif W32TUNInterface.c)
  set(tunconf W32TUNConfigurator.c)
else()
  set(tunif TUNInterface.c)
  if(${CMAKE_SYSTEM_NAME} STREQUAL Linux)
    set(tunconf TUNConfigurator_Linux.c)
  endif()
endif()

add_library(interface
    UDPInterface.c
    UDPInterface_admin.c
    ${tunif}
    ${tunconf}
    InterfaceConnector.c
    SessionManager.c
)

target_link_libraries(interface util ${LIBEVENT2_LIBRARIES})

enable_testing()
add_subdirectory(test)
