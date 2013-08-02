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

if (NOT LIBUV_FOUND AND "$ENV{NO_STATIC}" STREQUAL "")
    include(LibuvHelpers)

    include(ExternalProject)

    set(url "${CMAKE_SOURCE_DIR}/cmake/externals/libuv-v0.10.7.tar.gz")

    set(PATCH_LIBUV ${CMAKE_SOURCE_DIR}/cmake/patches/patch.sh ${CMAKE_SOURCE_DIR}/cmake/patches/)
    if (APPLE)
        set(MAKE_COMMAND "make")
    elseif (WIN32)
        set(MAKE_COMMAND make -f Makefile PLATFORM=mingw PREFIX=${TOOLCHAIN_PREFIX}-)
    else ()
        if (FREEBSD OR OPENBSD)
            set(make gmake)
						separate_arguments(MAKE_COMMAND UNIX_COMMAND "CFLAGS=-fPIC ${make} HAVE_DTRACE=0")
        else()
            set(make make)
						separate_arguments(MAKE_COMMAND UNIX_COMMAND "CFLAGS=-fPIC ${make}")
        endif ()
    endif ()

    ExternalProject_Add(libuv_ep
        URL ${url}
        SOURCE_DIR "${CMAKE_BINARY_DIR}/libuv"
        BINARY_DIR "${CMAKE_BINARY_DIR}/libuv"
        CONFIGURE_COMMAND ""
        BUILD_COMMAND ${MAKE_COMMAND}
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ${PATCH_LIBUV}
    )

    set(LIBUV_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/libuv/include")

    add_library(uv STATIC IMPORTED)

    set_property(TARGET uv
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/libuv/libuv.a)

    # The source of this uglyness is a limit on adding dependencies to imported libraries.
    # see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395
    # It's fixed in cmake 2.8.4 but it would be nice to continue supporting 2.8.2 and 2.8.3
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/DoNothing_libuv.c)
        file(WRITE ${CMAKE_BINARY_DIR}/DoNothing_libuv.c "int DoNothing() { return 0; }\n")
    endif()
    add_library(libuv_dep ${CMAKE_BINARY_DIR}/DoNothing_libuv.c)
    add_dependencies(libuv_dep libuv_ep)

    addDependencies()

    set(LIBUV_LIBRARIES uv libuv_dep)
    set(LIBUV_FOUND TRUE)

endif()
