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
if (NOT REMOTE_TEST)
set(REMOTE_TEST TRUE)

file(READ ${CMAKE_SOURCE_DIR}/cmake/modules/RemoteTestTemplate.bash template)
string(REPLACE "__REMOTE_TEST_IP_PORT__" "${REMOTE_TEST_IP_PORT}" FILE_CONTENT "${template}")
set(template FALSE)


if (REMOTE_TEST_IP_PORT)
    function(remoteTest executable)
        string(REPLACE "__TEST_FILE__" "${CMAKE_CURRENT_BINARY_DIR}/${executable}${CMAKE_EXECUTABLE_SUFFIX}" fileContent "${FILE_CONTENT}")
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh "${fileContent}")
        execute_process(COMMAND chmod
            "a+x" ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh
        )
        add_test(${executable} ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh)
        message("        Running test remotely")
    endFunction()
else()
    function(remoteTest executable)
        message("        REMOTE_TEST_IP_PORT unspecified, skipping test.")
    endfunction()
endif()

endif()
