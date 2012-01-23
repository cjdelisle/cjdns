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

nacl_do_build () {
    tar -xjf "$nacl_dist_file" -C "$nacl_build_dir"
    cd "$nacl_dist_vers"

    # Apply patches
    for i in $( ls "patch" ); do
        cat "${i}" | patch -p1 --forward
    done

    cat ./do | sed -e '/^exec >"$top\/log"$/d' > do.loudly
    sh ./do.loudly
    rm -f ./do.loudly

    cd -
}

nacl_find_artifacts () {
    build_arch="`uname -m`"
    nacl_artifacts="`find $nacl_build_dir/$nacl_dist_vers -iname $1`"

    for i in $nacl_artifacts; do
        if [ $(echo "$i" | grep "$build_arch") ]; then
            echo "$i" && return
        elif [ $(echo "$i" | grep "amd64") ]; then
            if [ $(echo "$build_arch" | grep "x86_64") ]; then
                echo "$i" && return
            fi
        elif [ $(echo "$i" | grep "x86_64") ]; then
            if [ $(echo "$build_arch" | grep "amd64") ]; then
                echo "$i" && return
            fi
        fi
    done
}

nacl_do_build

# Get the path of the library
nacl_lib_path=$(nacl_find_artifacts 'libnacl.a')

# Get the path of an include file, then get the directory it's in.
nacl_include_path=$(nacl_find_artifacts 'cpucycles.h')
nacl_include_dir="`echo $nacl_include_path | sed 's|\(.*\)/.*|\1|'`"

echo "SET(NACL_INCLUDE_DIRS $(pwd)/$nacl_include_dir)" > nacl.cmake
echo "SET(NACL_LIBRARIES $(pwd)/$nacl_lib_path)" >> nacl.cmake
