#!/usr/bin/env bash
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

[ -n "$PLATFORM" ] || PLATFORM=$(uname | tr '[:upper:]' '[:lower:]')
[ -n "$MARCH" ] || MARCH=$(uname -m | sed "s/i./x/g")
BUILDDIR="build_${PLATFORM}"
NODE_MIN_VER="v0.8.15"

hasOkNode()
{
    for NODE in "$(pwd)/${BUILDDIR}/nodejs/node/bin/node" "nodejs" "node"; do
        if ${NODE} -v >/dev/null 2>&1; then
            ${NODE} ./node_build/versionTest.js ${NODE_MIN_VER} && return 0;
            echo "You have a version of node [${NODE}] but it is too old [`${NODE} -v`]"
        fi
    done
    return 1
}

getNode()
{
    echo "Installing node.js"
    echo "You can bypass this step by manually installing node.js ${NODE_MIN_VER} or newer"
    if [ "${PLATFORM}-${MARCH}" = "linux-x86_64" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-linux-x64.tar.gz"
        NODE_SHA="1dac61c21fa21e47fc6e799757569c6c3914897ca46fc8f4dd2c8f13f0400626"
    elif [ "${PLATFORM}-${MARCH}" = "linux-x86" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-linux-x86.tar.gz"
        NODE_SHA="3f1fa0bb332b1354bca8d52d89e92c8884b6469a0f5fc3826ee72c2639279995"
    elif [ "${PLATFORM}-${MARCH}" = "linux-armv6l" ]; then #Raspberry Pi
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-linux-arm-pi.tar.gz"
        NODE_SHA="bdd5e253132c363492fa24ed9985873733a10558240fd45b0a4a15989ab8da90"
    elif [ "${PLATFORM}-${MARCH}" = "darwin-x86_64" ]; then
         NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-darwin-x64.tar.gz"
         NODE_SHA="5ae186f86564df87d82811614f667b9333a6219b02c6f405fd72807b459b8fc7"
    elif [ "${PLATFORM}-${MARCH}" = "darwin-x86" ]; then
         NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-darwin-x86.tar.gz"
         NODE_SHA="22af5001e5b0166fa91934107c17727e173677e5c972bd7122373743309c5372"
    elif [ "${PLATFORM}-${MARCH}" = "sunos-x86_64" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-sunos-x64.tar.gz"
        NODE_SHA="931059671413872c5c5e862df5f7a56066fdb1fe2b678b9ee3c3b242b23a4198"
    elif [ "${PLATFORM}-${MARCH}" = "sunos-x86" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.25/node-v0.10.25-sunos-x86.tar.gz"
        NODE_SHA="61d187416814dd10074d1db5666fd0ca61be6152fbb7920f0aaa4e285db10717"
    else
        echo "No nodejs executable available for ${PLATFORM}-${MARCH}"
        return 1;
    fi

    origDir="$(pwd)"
    [ -d "${BUILDDIR}/nodejs" ] && rm -r "${BUILDDIR}/nodejs"
    mkdir -p "${BUILDDIR}/nodejs"
    cd "${BUILDDIR}/nodejs"

    if wget --version > /dev/null 2>&1; then
        wget -O - "${NODE_DOWNLOAD}" > node.tar.gz
    elif curl --version > /dev/null 2>&1; then
        curl "${NODE_DOWNLOAD}" > node.tar.gz
    else
        echo 'wget or curl is required download node.js but you have neither!'
        return 1
    fi

    if ! ( ${SHA256SUM} node.tar.gz | grep -q ${NODE_SHA} ); then
        echo 'The downloaded file is damaged! Aborting.'
        return 1
    fi
    tar -xzf node.tar.gz
    find ./ -mindepth 1 -maxdepth 1 -type d -exec mv {} node \;
    cd ${origDir}
    hasOkNode && return 0
    return 1
}

realpath() {
    [[ $1 = /* ]] && echo "$1" || echo "$PWD"
}


buildNode() {
    export BASEDIR=$(realpath "$0")
    chmod +x contrib/bash/buildNode.sh
    . contrib/bash/buildNode.sh
    hasOkNode && return 0
    return 1
}

die() {
    echo "ERROR: $1" >&2
    exit 1
}

# get a sha256sum implementation.
getsha256sum() {
    expected="4ee73c05d5158b0fdfec9f5e52cab3fa85b98d6992a221bbff28fdbd935e8afc"
    testFile=test/$expected
    for hasher in sha256sum gsha256sum 'shasum -a 256' 'openssl sha256'
    do
        #echo "trying ${hasher} ${testFile}"
        ${hasher} ${testFile} 2>/dev/null | grep -q ${expected} && SHA256SUM=${hasher} && return 0
    done
    return 1
}

main() {
    cd "$(dirname $0)" || die "failed to set directory"
    [ -d "${BUILDDIR}" ] || mkdir "${BUILDDIR}" || die "failed to create build dir ${BUILDDIR}"
    getsha256sum || die "couldn't find working sha256 hasher";
    hasOkNode || getNode || buildNode || die "could not get working nodejs impl";

    $NODE ./node_build/make.js "${@}" || return 1
}

main
