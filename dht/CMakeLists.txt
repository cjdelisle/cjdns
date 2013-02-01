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
add_subdirectory(dhtcore)

find_package(NACL REQUIRED)

include_directories(${NACL_INCLUDE_DIRS})

add_library(dht
    DHTModuleRegistry.c
    SerializationModule.c
    ReplyModule.c
    AddressMapper.c
)

# This is needed because NACL must be built first because header files are generated.
add_dependencies(dht NACL)

target_link_libraries(dht cjdbenc cjdbenc_StandardBencSerializer util interface ${NACL_LIBRARIES})

# Everything must be tested.
enable_testing()
add_subdirectory(test)
