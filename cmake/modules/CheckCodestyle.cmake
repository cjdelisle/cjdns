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

find_program(perl perl)
if(NOT perl)
    message("Couldn't find perl, skipping codestyle check.")
else()
    string(REPLACE "," ";" skipDirs "${SKIP}")
    set(filesToCheck "")
    file(GLOB_RECURSE files "${CSD}/*.[ch]")
    foreach(f ${files})
        set(skip FALSE)
        foreach(dir ${skipDirs})
            if (${f} MATCHES "${CSD}/${dir}/.*")
                set(skip TRUE)
                break()
            endif()
        endforeach()
        # ignore all *build* subdirectories
        if (${f} MATCHES "${CSD}/[^/]*build[^/]*/.*")
            set(skip TRUE)
        endif()
        if (skip OR IS_DIRECTORY ${f})
            # skip
        elseif(${f} MATCHES "${CSD}/.*")
            #message("checking ${f}")
            list(APPEND filesToCheck "${f}\n")
        endif()
    endforeach()
    string(REPLACE ";" "" fileList "${filesToCheck}")
    file(REMOVE ${CMAKE_BINARY_DIR}/files_to_check.txt)
    file(WRITE ${CMAKE_BINARY_DIR}/files_to_check.txt ${fileList})

    execute_process(COMMAND ${perl} ${CSD}/scripts/checkfiles.pl
        INPUT_FILE ${CMAKE_BINARY_DIR}/files_to_check.txt
        OUTPUT_VARIABLE out
    )
    if (NOT "${out}" STREQUAL "")
        message("${out}")
        message(FATAL_ERROR "There were codestyle violations.")
    endif()

endif()
