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
    execute_process(COMMAND ${perl} ${CSD}/cmake/modules/CodestyleChecker.pl
        WORKING_DIRECTORY ${CSD}
        OUTPUT_VARIABLE out
    )
    if (NOT "${out}" STREQUAL "")
        message("${out}")
        message(FATAL_ERROR "There were codestyle violations.")
    endif()

endif()
