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

if(NOT "$ENV{GIT_VERSION}" STREQUAL "")
    SET(GIT_VERSION "$ENV{GIT_VERSION}")
elseif(EXISTS ${CMAKE_SOURCE_DIR}.git_commit)
    FILE(READ .git_commit GIT_VERSION)
else()
    find_package(Git)
    execute_process(
        COMMAND ${GIT_EXECUTABLE} rev-parse HEAD
        WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
        OUTPUT_VARIABLE GIT_VERSION
        OUTPUT_STRIP_TRAILING_WHITESPACE
    )
    message("The git version ID is ${GIT_VERSION}")
endif()

add_definitions("-D GIT_VERSION=\\\"${GIT_VERSION}\\\"")
add_library(cjdns-util-version-version
    Version.c
)
