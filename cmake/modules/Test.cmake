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
include(${CMAKE_SOURCE_DIR}/cmake/modules/RemoteTest.cmake)

string(REGEX REPLACE "^.*/" "" main_dir_name ${CMAKE_SOURCE_DIR})
string(REPLACE ${CMAKE_SOURCE_DIR} ${main_dir_name} this_dir ${CMAKE_CURRENT_SOURCE_DIR})
message("-- Tests to run for " ${this_dir})
add_definitions(-g -DTEST)

foreach(test_source ${Test_FILES})
    string(REPLACE "test.c" "test" test_bin ${test_source})
    message("     " ${test_source})
    add_executable(${test_bin} ${test_source})
    target_link_libraries(${test_bin} ${Test_LIBRARIES})
    if (CMAKE_CROSSCOMPILING)
        remoteTest(${test_bin})
    else()
        add_test(${test_bin} ${test_bin})
    endif()
endforeach()
message("")
