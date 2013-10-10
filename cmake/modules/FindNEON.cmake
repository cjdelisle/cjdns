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

if (NOT NEON AND "${CMAKE_SYSTEM_PROCESSOR}" MATCHES "^arm" AND NOT NO_NEON)
    # If we're dealing with an ARM processor then we need to run a (potentially remote)
    # test to see if it supports NEON and if so then we add the -mfpu=neon flag so that
    # it will be detected in the compiler test and used.

    message("Processor type is ARM, testing for ARM NEON")
    try_run(returnCode
            compileFail
            ${CMAKE_BINARY_DIR}
            ${CMAKE_SOURCE_DIR}/cmake/modules/ProbeNEON.c
            COMPILE_DEFINITIONS -mfpu=neon -mfloat-abi=softfp
            COMPILE_OUTPUT_VARIABLE compileOut
            RUN_OUTPUT_VARIABLE runOut)

    if (compileFail)
        message("NEON check failed to compile [${compileOut}]")
    elseif (returnCode)
        message("NEON check failed to run [${runOut}]")
    else ()
        message("Success! Building NACL for ARM NEON")
        set(NEON TRUE)
    endif()
endif()
