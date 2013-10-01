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

# uv-private/tree.h
include_directories(${LIBUV_INCLUDE_DIRS})

add_library(dhtcore
    NodeIndex.c
    NodeStore.c
    NodeStore_admin.c
    RouterModule.c
    RouterModule_admin.c
    SearchStore.c
    Janitor.c
    VersionList.c
    SearchRunner.c
    RouteTracer.c
    SwitchCompression.c
)

target_link_libraries(dhtcore
    cjdbenc
    util
    crypto
    switch
    cjdns-util-version-version
    cjdns-util-events-libuv-time
)

# Everything must be tested.
enable_testing()
add_subdirectory(test)
