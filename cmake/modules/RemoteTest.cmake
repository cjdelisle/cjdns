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

if (CMAKE_CROSSCOMPILING AND REMOTE_TEST_IP_PORT)
    file(READ ${CMAKE_SOURCE_DIR}/cmake/modules/RemoteTestTemplate.bash template)
    string(REPLACE "__REMOTE_TEST_IP_PORT__" "${REMOTE_TEST_IP_PORT}" FILE_CONTENT "${template}")

    # try_run(RUN_RESULT_VAR COMPILE_RESULT_VAR
    #      bindir srcfile [CMAKE_FLAGS <Flags>]
    #      [COMPILE_DEFINITIONS <flags>]
    #      [COMPILE_OUTPUT_VARIABLE comp]
    #      [RUN_OUTPUT_VARIABLE run]
    #      [OUTPUT_VARIABLE var]
    #      [ARGS <arg1> <arg2>...])
    function(RemoteTest_tryRun RUN_RESULT COMPILE_RESULT bindir srcfile)
        set(params "")
        foreach(i RANGE ${ARGC})
            list(APPEND params ${ARGV${i}})
        endforeach()
        string(RANDOM rand)
        set(execFile ${CMAKE_CURRENT_BINARY_DIR}/${rand}_remote_executable)
        try_compile(${COMPILE_RESULT} ${bindir} ${srcfile} COPY_FILE ${execFile})

        message("Running remote test: ${execFile}_remote.sh")
        string(REPLACE "__TEST_FILE__" "${execFile}" fileContent "${FILE_CONTENT}")
        file(WRITE "${execFile}_remote.sh" "${fileContent}")
        execute_process(COMMAND chmod "a+x" ${execFile}_remote.sh)
        execute_process(COMMAND ${execFile}_remote.sh
            RESULT_VARIABLE runResult
            OUTPUT_VARIABLE output
        )
        set(${RUN_RESULT} ${runResult} PARENT_SCOPE)
        set(${RUN_OUTPUT_VARIABLE} ${output} PARENT_SCOPE)
        
    endfunction()

    function(RemoteTest_addTest executable)
        string(REPLACE "__TEST_FILE__" "${CMAKE_CURRENT_BINARY_DIR}/${executable}${CMAKE_EXECUTABLE_SUFFIX}" fileContent "${FILE_CONTENT}")
        file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh "${fileContent}")
        execute_process(COMMAND chmod
            "a+x" ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh
        )
        add_test(${executable} ${CMAKE_CURRENT_BINARY_DIR}/${executable}_remote.sh)
        message("        Running test remotely")
    endFunction()
elseif(CMAKE_CROSSCOMPILING)
    function(RemoteTest_addTest executable)
        message("        REMOTE_TEST_IP_PORT unspecified, skipping test.")
    endfunction()

    function(RemoteTest_tryRun)
        message(FATAL_ERROR "REMOTE_TEST_IP_PORT unspecified.")
    endfunction()
else()
    function(RemoteTest_addTest executable)
        add_test(${executable} ${executable})
    endfunction()

    function(RemoteTest_tryRun)
        set(params "")
        foreach(i RANGE ${ARGC})
            list(APPEND params ${ARGV${i}})
        endforeach()
        try_run(${params})
    endfunction()
endif()
