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
cmake_minimum_required(VERSION 2.8.2)

if (NOT NACL_FOUND)

    find_path(NACL_INCLUDE_DIRS
        NAMES
            crypto_box_curve25519xsalsa20poly1305.h
        PATHS
            ${NACL_PREFIX}/include
            /usr/include
            /usr/include/nacl
            /usr/local/include
            /opt/local/include
            ${CMAKE_BINARY_DIR}/nacl_build/include/
        NO_DEFAULT_PATH
    )

    find_library(NACL_LIBRARIES
        NAMES
            libnacl.a
        PATHS
            ${NACL_INCLUDE_DIRS}/../
            ${NACL_INCLUDE_DIRS}/../lib
            ${NACL_INCLUDE_DIRS}/../lib/nacl/
            ${NACL_INCLUDE_DIRS}/../../lib/
            ${NACL_INCLUDE_DIRS}/../../lib/nacl/
        NO_DEFAULT_PATH
    )

    if(NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
        message("libnacl found: ${NACL_INCLUDE_DIRS}")
        set(NACL_FOUND TRUE)
    endif()

endif()
