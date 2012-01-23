#!/bin/bash

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

nacl_dist_dir="../crypto/nacl"
nacl_dist_vers="nacl-20110221"
nacl_dist_file="$nacl_dist_dir/$nacl_dist_vers.tar.bz2"
nacl_build_dir="."

function nacl_do_build()
{
    tar -xjf "$nacl_dist_file" -C "$nacl_build_dir"
    pushd "$nacl_dist_vers"

    # Apply patches
    for i in $( ls "patch" ); do
        cat "${i}" | patch -p1 --forward
    done

    cat ./do | sed -e '/^exec >"$top\/log"$/d' > do.loudly
    sh ./do.loudly
    rm -f ./do.loudly

    popd
}

function nacl_find_artifacts()
{
    build_arch="`uname -m`"
    nacl_artifacts="`find $nacl_build_dir/$nacl_dist_vers -iname $1`"

    for i in $nacl_artifacts; do
        if [[ $i =~ $build_arch ]]; then
            echo "${i}" && return
        elif [[ $i =~ "amd64" ]]; then
            if [[ "x86_64" =~ $build_arch ]]; then
                echo "${i}" && return
            fi
        elif [[ $i =~ "x86_64" ]]; then
            if [[ "amd64" =~ $build_arch ]]; then
                echo "${i}" && return
            fi
        fi
    done
}

set +x

nacl_do_build

# Get the path of the library
nacl_lib_path=$(nacl_find_artifacts '*.a')

# Get the path of an include file, then get the directory it's in.
nacl_include_path=$(nacl_find_artifacts '*.h')
nacl_include_dir="`echo $nacl_include_path | sed 's|\(.*\)/.*|\1|'`"

echo "SET(NACL_INCLUDE_DIRS $nacl_include_dir)" > nacl.cmake
echo "SET(NACL_LIBRARIES $nacl_lib_path)" >> nacl.cmake

set -x
