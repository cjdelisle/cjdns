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

if (NOT NACL_INCLUDE_DIRS)
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${NACL_USE_FILES})

    ExternalProject_Add(NACL
        GIT_REPOSITORY git://github.com/cjdelisle/nacl.git
        GIT_TAG c737ed821164f397a1a0639b9361c66edf2d4000
        SOURCE_DIR "${CMAKE_BINARY_DIR}/nacl"
        BINARY_DIR "${CMAKE_BINARY_DIR}/nacl"
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
    )

    set(NACL_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/nacl/build/include/default/")
    set(NACL_LIBRARIES    "${CMAKE_BINARY_DIR}/nacl/build/lib/default/libnacl.a")

endif (NOT NACL_INCLUDE_DIRS)
