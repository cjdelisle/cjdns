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

add_executable(cjdns-angel
    Angel.c
    AngelInit.c
    Waiter.c
)

target_link_libraries(cjdns-angel
    cjdbenc
    cjdbenc_StandardBencSerializer
    cjdmemory
    ${LIBEVENT2_LIBRARIES}
    cjdns-security
)

add_executable(cjdns-core
    Core.c
    Waiter.c
)

target_link_libraries(cjdns-core
    crypto
    crypto_benchmark
    interface
    switch
    dht
    dhtcore
    cjdbenc
    cjdbenc_JsonBencSerializer
    cjdmemory
    cjdadmin
    cjdnet
    ${LIBEVENT2_LIBRARIES}
    ${PLATFORM_LIBRARIES}
)

enable_testing()
#add_subdirectory(test)
