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
project(cjdio C)
cmake_minimum_required(VERSION 2.4)

add_library(cjdio ArrayReader.c ArrayWriter.c FileReader.c FileWriter.c)
target_link_libraries(cjdio cjdmemory ${LIBEVENT2_LIBRARIES})

# Everything must be tested.
enable_testing()
add_subdirectory(test)
