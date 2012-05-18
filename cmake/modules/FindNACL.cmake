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

if (NOT NACL_FOUND)

    find_path(NACL_INCLUDE_DIRS
        NAMES
            crypto_box_curve25519xsalsa20poly1305.h
        PATHS
            ${NACL_PREFIX}/include
            /usr/include
            /usr/include/nacl
            /usr/local/include
            /opt/local/include
            ${CMAKE_BINARY_DIR}/nacl/build/include/default
        NO_DEFAULT_PATH
    )

    find_library(NACL_LIBRARIES
        NAMES
            nacl
        PATHS
            ${NACL_INCLUDE_DIRS}/../lib
            ${NACL_INCLUDE_DIRS}/../../lib
        NO_DEFAULT_PATH
    )

    if(NACL_INCLUDE_DIRS AND NACL_LIBRARIES)
        message("libnacl found: ${NACL_INCLUDE_DIRS}")
        set(NACL_FOUND TRUE)
    endif()

endif()

if(NOT NACL_FOUND)
    message("libnacl not found, will be downloaded and compiled.")
    include(ExternalProject)

    # Without this, the build doesn't happen until link time.
    include_directories(${NACL_USE_FILES})

    set(functions "")
    list(APPEND functions crypto_verify_16)
    list(APPEND functions crypto_stream_salsa20)
    list(APPEND functions crypto_stream_hsalsa20)
    list(APPEND functions crypto_auth_poly1305)
    list(APPEND functions crypto_secretbox_xsalsa20poly1305)

    list(APPEND functions crypto_scalarmult_curve25519)
    list(APPEND functions crypto_box_curve25519xsalsa20poly1305)

    list(APPEND functions crypto_hash_sha256)
    list(APPEND functions crypto_hash_sha256)
    list(APPEND functions crypto_hash_sha512)

    string(REPLACE ";" "," func "${functions}")

    # the name of the tag
    set(tag "f3080eee8c79321a751d8f5e5d4ccf42776b1be3.tar.gz")

    # the sha256 of the tar.gz file
    set(hash "f7339f909117f61e8ed49061bb4fbcf773eae690460698b78d8b819653af7212")

    set(file ${CMAKE_BINARY_DIR}/nacl_ep-prefix/src/${tag})
    set(AssertSHA256 ${CMAKE_SOURCE_DIR}/cmake/modules/AssertSHA256.cmake)
    set(check ${CMAKE_COMMAND} -DFILE=${file} -DEXPECTED=${hash} -P ${AssertSHA256})
    ExternalProject_Add(nacl_ep
        URL "http://nodeload.github.com/cjdelisle/nacl/tarball/${tag}"
        SOURCE_DIR "${CMAKE_BINARY_DIR}/nacl"
        BINARY_DIR "${CMAKE_BINARY_DIR}/nacl"
        CONFIGURE_COMMAND ${check}
        BUILD_COMMAND ./do "-primitives=${func}"
        INSTALL_COMMAND ""
        UPDATE_COMMAND ""
        PATCH_COMMAND ""
    )

    add_custom_command(
        OUTPUT ${CMAKE_BINARY_DIR}/nacl/build/nacl_test.out
        COMMAND ${CMAKE_COMMAND} -P ${CMAKE_SOURCE_DIR}/cmake/modules/TestNACL.cmake
    )
    add_custom_target(nacl_test DEPENDS ${CMAKE_BINARY_DIR}/nacl/build/nacl_test.out)
    add_dependencies(nacl_test nacl_ep)

    # The source of this uglyness is a limit on adding dependencies to imported libraries.
    # see: http://www.cmake.org/Bug/print_bug_page.php?bug_id=10395
    # It's fixed in cmake 2.8.4 but it would be nice to continue supporting 2.8.2 and 2.8.3
    if(NOT EXISTS ${CMAKE_BINARY_DIR}/DoNothing.c)
        file(WRITE ${CMAKE_BINARY_DIR}/DoNothing.c "int DoNothing() { return 0; }")
    endif()
    add_library(nacl_test_dependency ${CMAKE_BINARY_DIR}/DoNothing.c)
    add_dependencies(nacl_test_dependency nacl_test)


    add_library(nacl STATIC IMPORTED)
    set_property(TARGET nacl
        PROPERTY IMPORTED_LOCATION ${CMAKE_BINARY_DIR}/nacl/build/lib/default/libnacl.a)

    set(NACL_INCLUDE_DIRS "${CMAKE_BINARY_DIR}/nacl/build/include/default/")
    set(NACL_LIBRARIES nacl nacl_test_dependency)
    set(NACL_FOUND TRUE)

endif()
