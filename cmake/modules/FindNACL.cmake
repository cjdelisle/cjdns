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
if (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
    set(NACL_FOUND TRUE)
endif (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)

if (NOT NACL_FOUND)
    message("Checking for nacl crypto library.")
    if(NOT EXISTS "${CMAKE_BINARY_DIR}/nacl.cmake")
        MESSAGE("Compiling nacl crypto library.")
        execute_process(COMMAND sh "${CMAKE_SOURCE_DIR}/crypto/nacl/build-nacl.sh"
                        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}//")
    endif(NOT EXISTS "${CMAKE_BINARY_DIR}/nacl.cmake")
    include("${CMAKE_BINARY_DIR}/nacl.cmake")
    if (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
        set(NACL_FOUND TRUE)
    endif (NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
endif (NOT NACL_FOUND)

if (NACL_FOUND)
    message(STATUS "Found libnacl: includes ${NACL_INCLUDE_DIRS} libs ${NACL_LIBRARIES}")
else (NACL_FOUND)
    if (NACL_FIND_REQUIRED)
        message(FATAL_ERROR "Could not find libnacl, try to setup NACL_PREFIX accordingly")
    endif (NACL_FIND_REQUIRED)
endif (NACL_FOUND)
