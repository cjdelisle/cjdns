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
if (NOT HasRoot_CMAKE)

set(content "
    #ifdef WIN32
        #include <windows.h>
        #include <string.h>
        int main()
        {
            char buff[32] = {0};
            DWORD length = 32;
            GetUserNameA(buff, &length);
            printf(\"%s\\n\", buff);
            return strcmp(\"Administrator\", buff);
        }
    #else
        #include <unistd.h>
        int main()
        {
            return getuid();
        }
    #endif
")

file(WRITE ${CMAKE_BINARY_DIR}/HasRoot.c "${content}")
RemoteTest_tryRun(runRes compRes ${CMAKE_BINARY_DIR} ${CMAKE_BINARY_DIR}/HasRoot.c
    COMPILE_OUTPUT_VARIABLE compileOut
    RUN_OUTPUT_VARIABLE output
)
if (NOT compRes)
    message("${compileOut}")
    message(FATAL_ERROR "Compilation failed")
endif()
if ("${runRes}" STREQUAL "PLEASE_FILL_OUT-FAILED_TO_RUN")
    message(FATAL_ERROR "RunFailed")
endif()
if (NOT runRes)
    set(HasRoot TRUE CACHE INTERNAL "")
    message("Testing with root access, root tests enabled")
else()
    set(HasRoot FALSE CACHE INTERNAL "")
    message("Testing without root access, root tests disabled")
endif()

set(HasRoot_CMAKE TRUE CACHE INTERNAL "")
endif()
