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

nodeUpToDate()
# accepts 1 parameter: path to Node.js binary
{
    local_version=$(${1} -v | sed 's/[^[0-9]/0000/g')
    minimal_required_version=$(echo "${NODE_MIN_VER}" | sed 's/[^[0-9]/0000/g')
    [ "${local_version}" -ge "${minimal_required_version}" ] && return 0
    return 1
}

hasOkNode()
{
    for NODE in "$(pwd)/${BUILDDIR}/nodejs/node/bin/node" "nodejs" "node"; do
        if ${NODE} -v >/dev/null 2>&1; then
            if nodeUpToDate "${NODE}"; then
                return 0 #Found it!
            else
                echo "You have a version of node but it is too old"
            fi
        fi
    done
    return 1
}

getNode()
{
    echo "Installing node.js"
    echo "You can bypass this step by manually installing node.js ${NODE_MIN_VER} or newer"
    if [ "${PLATFORM}-${MARCH}" = "linux-x86_64" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-linux-x64.tar.gz"
        NODE_SHA="6ef93f4a5b53cdd4471786dfc488ba9977cb3944285ed233f70c508b50f0cb5f"
    elif [ "${PLATFORM}-${MARCH}" = "linux-x86" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-linux-x86.tar.gz"
        NODE_SHA="fb6487e72d953451d55e28319c446151c1812ed21919168b82ab1664088ecf46"
    elif [ "${PLATFORM}-${MARCH}" = "linux-armv6l" ]; then #Raspberry Pi
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-linux-arm-pi.tar.gz"
        NODE_SHA="bdd5e253132c363492fa24ed9985873733a10558240fd45b0a4a15989ab8da90"
    elif [ "${PLATFORM}-${MARCH}" = "darwin-x86_64" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-darwin-x64.tar.gz"
        NODE_SHA="c1c523014124a0327d71ba5d6f737a4c866a170f1749f8895482c5fa8be877b0"
    elif [ "${PLATFORM}-${MARCH}" = "darwin-x86" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-darwin-x86.tar.gz"
        NODE_SHA="8b8d2bf9828804c3f8027d7d442713318814a36df12dea97dceda8f4aff42b3c"
    elif [ "${PLATFORM}-${MARCH}" = "sunos-x86_64" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-sunos-x64.tar.gz"
        NODE_SHA="7cb714df92055b93a908b3b6587ca388a2884b1a9b5247c708a867516994a373"
    elif [ "${PLATFORM}-${MARCH}" = "sunos-x86" ]; then
        NODE_DOWNLOAD="http://nodejs.org/dist/v0.10.24/node-v0.10.24-sunos-x86.tar.gz"
        NODE_SHA="af69ab26aae42b05841c098f5d11d17e21d22d980cd32666e2db45a53ddffe34"
    else
        echo "No nodejs executable available for ${PLATFORM}-${MARCH}"
        echo -n "Please install nodejs (>= ${NODE_MIN_VER}) from "
        echo "your distribution package repository or from source."
        return 1
    fi

    origDir="$(pwd)"
    [ -d "${BUILDDIR}/nodejs" ] && rm -r "${BUILDDIR}/nodejs"
    mkdir -p "${BUILDDIR}/nodejs"
    cd "${BUILDDIR}/nodejs"

    APP="$(which wget || which curl || echo 'none')"
    [ "$APP" = 'none' ] && echo 'wget or curl is required download node.js but you have neither!' && return 1
    [ "x$APP" = x"$(which wget)" ] && $APP ${NODE_DOWNLOAD}
    [ "x$APP" = x"$(which curl)" ] && $APP ${NODE_DOWNLOAD} > node.tar.gz

    if ! ( ${SHA256SUM} ./*.tar.gz | grep -q ${NODE_SHA} ); then
        echo 'The downloaded file is damaged! Aborting.'
        return 1
    fi
    tar -xzf ./*.tar.gz
    find ./ -mindepth 1 -maxdepth 1 -type d -exec mv {} node \;
    cd "$origDir"
    hasOkNode && return 0;
    return 1;
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
    hasOkNode || getNode || die "could not get working nodejs impl";

    $NODE ./node_build/make.js "${@}" || return 1
}

main
