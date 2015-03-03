#!/usr/bin/env bash
#
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
#

[[ -n "$PLATFORM" ]] \
    || PLATFORM=$(uname | tr '[:upper:]' '[:lower:]')

[[ -n "$MARCH" ]] \
    || MARCH=$(uname -m | sed 's/i./x/g')

build_dir="build_$PLATFORM"
node_min_ver='v0.8.15'
node_dl_ver='v0.10.24'

read -d '' version_test <<"EOF"
var currentVersion = process.version;
var verArray = currentVersion.substring(1).split(".");
var minVerArray = process.argv[process.argv.length-1].substring(1).split(".");
for (var i = 0; i < minVerArray.length; i++) {
    if (Number(verArray[i]) < Number(minVerArray[i])) {
        process.exit(1);
    } else if (Number(verArray[i]) > Number(minVerArray[i])) {
        process.exit(0);
    }
}
EOF

# return true if the input command exists in $PATH
function cmd_exists() {
    type -P "$1" >/dev/null
}

# output an error and exit with a failed status
function die() {
    printf '%s\n' "ERROR: $1" >&2
    exit 1
}

# detect and configure a copy of node.js to use with at least the minimum version
function check_node_tool() {
    for node_tool in "$build_dir/nodejs/node/bin/node" 'nodejs' 'node'; do
        cmd_exists "$node_tool"
        if [ $? = 0 -o -f "$node_tool" ]; then
            "$node_tool" '' "$node_min_ver" <<< "$version_test" && {
                node_cmd="$node_tool"
                return 0
            }
            printf '%s\n' "You have a version of node [$node_tool] but it is too old [$($node_tool -v)]"
        fi
    done
    return 1
}

# detect and configure a sha256sum implementation to use that produces the expected sum for an empty string
function get_shasum_tool() {
    expected_sum='01ba4719c80b6fe911b091a7c05124b64eeece964e09c058ef8f9805daca546b'
    for shasum_tool in 'sha256sum' 'gsha256sum' 'sha256' 'shasum -a 256' 'openssl sha256'; do
        if cmd_exists "${shasum_tool/ *}"; then
            [[ $($shasum_tool <<< '') =~ "$expected_sum" ]] && {
                shasum_cmd="$shasum_tool"
                return 0
            }
        fi
    done
    return 1
}

# download and configure a copy of node.js to use based on the current system
function get_node_tool() {
    printf '%s %s\n\n' '###' "Installing node.js (you can bypass this step by manually installing node.js $node_min_ver or newer)"
    case "$PLATFORM-$MARCH" in
        linux-x86_64)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-linux-x64.tar.gz"
            node_sha='6ef93f4a5b53cdd4471786dfc488ba9977cb3944285ed233f70c508b50f0cb5f'
            ;;
        linux-x86)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-linux-x86.tar.gz"
            node_sha='fb6487e72d953451d55e28319c446151c1812ed21919168b82ab1664088ecf46'
            ;;
        linux-armv6l|linux-armv7l)
            # Raspberry Pi or Cubieboard
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-linux-arm-pi.tar.gz"
            node_sha='bdd5e253132c363492fa24ed9985873733a10558240fd45b0a4a15989ab8da90'
            ;;
        darwin-x86_64)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-darwin-x64.tar.gz"
            node_sha='c1c523014124a0327d71ba5d6f737a4c866a170f1749f8895482c5fa8be877b0'
            ;;
        darwin-x86)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-darwin-x86.tar.gz"
            node_sha='8b8d2bf9828804c3f8027d7d442713318814a36df12dea97dceda8f4aff42b3c'
            ;;
        sunos-x86_64)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-sunos-x64.tar.gz"
            node_sha='7cb714df92055b93a908b3b6587ca388a2884b1a9b5247c708a867516994a373'
            ;;
        sunos-x86)
            node_download="http://nodejs.org/dist/$node_dl_ver/node-$node_dl_ver-sunos-x86.tar.gz"
            node_sha='af69ab26aae42b05841c098f5d11d17e21d22d980cd32666e2db45a53ddffe34'
            ;;
        *)
            printf '%s\n%s\n' \
                "No nodejs executable available for $PLATFORM-$MARCH" \
                "Please install nodejs (>= $node_min_ver) from your distribution package repository or from source"
            return 1
            ;;
    esac

    [[ -d "$build_dir/nodejs" ]] \
        && rm -r "$build_dir/nodejs"
    install -d "$build_dir/nodejs"

    pushd "$build_dir/nodejs" >/dev/null
    node_dl="node-${node_dl_ver}.tar.gz"
    if cmd_exists wget; then
        printf '\n%s %s ' '==>' "Downloading $node_download with wget..."
        wget -q "$node_download" -O "$node_dl"
    elif cmd_exists curl; then
        printf '%s %s ' '==>' "Downloading $node_download with curl..."
        curl -s "$node_download" > "$node_dl"
    elif cmd_exists fetch; then
        printf '%s %s ' '==>' "Downloading $node_download with fetch..."
        fetch "$node_download" -o "$node_dl"
    else
        die 'wget, curl or fetch is required download node.js but you have none!'
    fi
    [[ -f "$node_dl" ]] \
        || die 'Failed to download node.js'
    printf '%s\n' 'DONE!'

    printf '%s %s ' '==>' "Verifying the checksum of the downloaded archive..."
    [[ $($shasum_cmd "$node_dl") =~ $node_sha ]] \
        || die 'The downloaded file is damaged! Aborting'
    printf '%s\n' 'DONE!'

    printf '%s %s ' '==>' "Extracting the downloaded archive..."
    install -d node
    tar xzf "$node_dl" -C node --strip-components=1
    [[ -d 'node' ]] \
        || die 'An error prevented the archive from being extracted'
    printf '%s\n\n' 'DONE!'
    popd >/dev/null

    # Return with the success status of the check_node_tool function
    check_node_tool
}

cd "$(dirname $0)" \
    || die 'failed to set directory'

[[ -d "$build_dir" ]] \
    || install -d "$build_dir" \
        || die "failed to create build dir $build_dir"

get_shasum_tool \
    || die "couldn't find working sha256 shasum_tool";

check_node_tool \
    || get_node_tool \
        || die "couldn't get working node.js implementation";

"$node_cmd" ./node_build/make.js "$@"

