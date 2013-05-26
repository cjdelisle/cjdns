# vim: set expandtab ts=4 sw=4:
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

# This is needed because NACL must be built first because header files are generated.

if (APPLE)
    find_library(SECURITY_LIBRARY Security)
    find_path(SECURITY_INCLUDE_DIR Security/Security.h)
    set(SEED_LIBRARIES util ${NACL_LIBRARIES} ${SECURITY_LIBRARY})
else()
    set(SEED_LIBRARIES util ${NACL_LIBRARIES})
endif()

include_directories(${SECURITY_INCLUDE_DIR})

add_library(cjdns-crypto-random-seed
    BsdKernArndSysctlRandomSeed.c
    DevUrandomRandomSeed.c
    LinuxRandomUuidSysctlRandomSeed.c
    ProcSysKernelRandomUuidRandomSeed.c
    RandomSeed.c
    RtlGenRandomSeed.c
)

add_dependencies(cjdns-crypto-random-seed NACL)

target_link_libraries(cjdns-crypto-random-seed
    ${SEED_LIBRARIES}
)

if (Windows)
    # RtlGenRandom()
    if (NOT FOUND_ADVAPI)
        find_library(ADVAPI_LIBRARIES
            NAMES
                advapi32
            PATHS
                ${ADVAPI_PREFIX}/
                /lib/
        )

        if(ADVAPI_LIBRARIES)
            message("Yay advapi32 was found at [${ADVAPI_LIBRARIES}]")
            set(ADVAPI_FOUND TRUE)
        else()
            message(FATAL_ERROR "Could not find advapi32, try to setup ADVAPI_PREFIX accordingly")
        endif()
    endif()

    add_dependencies(cjdns-crypto-random-seed ${ADVAPI_LIBRARIES})
endif()
