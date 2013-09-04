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

include(CheckLibraryExists)
include(CheckFunctionExists)
include(FindThreads)
include(FindClockGettime)

find_package(Socket REQUIRED)
find_package(ClockGettime)
find_package(Threads)

function(addDependencies)
  if (APPLE)
    find_library(CORE_SERVICES_LIB CoreServices)
    set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
      ${SOCKET_LIBRARIES}
      ${CLOCK_GETTIME_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      ${CORE_SERVICES_LIB}
      )
  elseif (FREEBSD OR OPENBSD)
    set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
      ${SOCKET_LIBRARIES}
      ${CLOCK_GETTIME_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      kvm
      )
  elseif (ILLUMOS)
    set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
      ${SOCKET_LIBRARIES}
      ${CLOCK_GETTIME_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      kstat
      sendfile
      xnet
      )
  elseif (WIN32)
    set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
      ${SOCKET_LIBRARIES}
      ${CLOCK_GETTIME_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      psapi # GetProcessMemoryInfo()
      iphlpapi # GetAdapterAddresses()
      )
  else ()
    set_property(TARGET uv PROPERTY IMPORTED_LINK_INTERFACE_LIBRARIES
      ${SOCKET_LIBRARIES}
      ${CLOCK_GETTIME_LIBRARIES}
      ${CMAKE_THREAD_LIBS_INIT}
      )
  endif ()
endFunction()
