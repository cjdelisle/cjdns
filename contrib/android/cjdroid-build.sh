#!/usr/bin/env sh

# http://cjdns.ca/cjdns-droid.sh

# Does most things required to build cjdns for android.
# See bottom of file for tips on installing/usage.
# ADB, Android, plus basic command line skills required
# ircerr 20140507

# Parts stolen from:
#  https://github.com/cjdelisle/cjdns/pull/476
#  https://gist.github.com/lgierth/01ce4bda638f8c863349
#  larsg@HypeIRC
# + mods by prurigro

# Update files from:
#  https://developer.android.com/tools/sdk/ndk/index.html

# Possible Deps (phone):
#  1. rooted:
#   The method required to root a phone differs from model to model.
#   If your phone isn't rooted yet and you're not sure where to
#   start, look for the subforum for your device on XDA forums
#   (linked below), and hopefully you'll find something that works.
#
#   http://forum.xda-developers.com/index.php?tab=all
#
#  2. tun device:
#   Most (if not all) 4.0+ phones include tun support. If yours
#   uses 2.x, CyanogenMod and some stock ROMs include support, but
#   many don't. If your phone doesn't have a TUN device at /dev/tun,
#   download the link below (or find 'com.aed.tun.installer' to
#   download it yourself), then install and run it to have it set
#   one up for you.
#
#   http://cjdns.ca/com.aed.tun.installer.apk

# Report success/failure including phone type, android version, kernel version,
# and as much information as possible to #cjdns @ HypeIRC

# NOTES:
#  Use a custom NDK directory:
#   Before running this script, configure $NDK: export NDK="/path/to/ndk"
#
#  Use a different repo:
#   Remove 'cjdns-android/cjdns' and below change: cjdns_repo="https://newaddr"
#
#  Use a different branch:
#   Run: cjdroid-bulid.sh branchname

die() { printf "Error: %s\n" "$*"; exit 1; }
commands="install curl git tar"
use_curl=true

for i in $commands; do
  case $i in
    curl)
      if ! command -v "$i" > /dev/null 2>&1; then
        use_curl=false
        if ! command -v "wget" > /dev/null 2>&1; then
          commands_failed="[curl || wget] $commands_failed"
        fi
      fi
      ;;
    *)
      if ! command -v "$i" > /dev/null 2>&1; then
        commands_failed="$i $commands_failed"
      fi
      ;;
  esac
done

if [ -n "$commands_failed" ]; then
  commands_failed=${commands_failed% *}
  die "$commands_failed is not found in your \$PATH.
Please install them and re-run the script."
fi




##CONFIGURABLE VARIABLES
cjdns_repo="https://github.com/cjdelisle/cjdns/"
if [ -n "$1" ]; then
  cjdns_repo_branch="-$1"
fi

build_dir="$PWD/build_android"
src_dir="$build_dir/source"
ndk_dir="$src_dir/ndk"
work_dir="$build_dir/workspace"

ndkver="android-ndk-r10e"
cpu_arch="$(uname -m)"
if [ -z "$cpu_arch" ]; then
    die "NO CPU ARCHITECTURE DETECTED"
fi
if [ "$cpu_arch" = "i686" ]; then
  cpu_arch="x86"
fi

##CREATE REQUIRED DIRECTORIES
install -d "$src_dir" || die "Cannot install $src_dir"
install -d "$work_dir" || die "Cannot install $work_dir"

##SETUP NDK
cd -- "$src_dir" || die "Cannot cd to $src_dir"
if [ -z "$NDK" ]; then
    if [ -z "$ANDROID_NDK" ]; then
        android_ndk_bin="${ndkver}-linux-${cpu_arch}.bin"
        printf "%s\n" "$android_ndk_bin"
        if [ ! -f "$android_ndk_bin" ]; then
            url="https://dl.google.com/android/ndk/$android_ndk_bin"
            if [ $use_curl = true ]; then
                curl -LfS --tlsv1.3 --output "$android_ndk_bin" -- "$url" || die "curl $url failed."
            else
                wget --secure-protocol=TLSv1_3 -O "$android_ndk_bin" -- "$url" || die "wget $url failed."
            fi
        fi
        if [ ! -d "$ndkver" ]; then
            chmod a+x "$android_ndk_bin" || die "Please check permissions of file $android_ndk_bin"
            if ! ./"$android_ndk_bin"; then
                rm -rf -- "$ndkver"
                die "Cannot run ndk $android_ndk_bin. Please install manually."
            fi
        fi
        NDK="$ndkver"
    else
        NDK="$ANDROID_NDK"
    fi
fi
if [ ! -d "$NDK" ]; then
    die "The NDK variable is not pointing to a valid directory."
fi
if [ -h "$ndk_dir" ]; then
    rm -- "$ndk_dir"
fi
if [ ! -e "$ndk_dir" ]; then
    ln -sf "$NDK" "$ndk_dir"
fi


if [ "$TARGET_ARCH" = "arm64" ]; then
    GCC="$work_dir/android-arm-toolchain/bin/aarch64-linux-android-gcc"
    TOOLCHAIN="aarch64-linux-android-4.9"
    COMPILER="aarch64-linux-android-"
else
    GCC="$work_dir/android-arm-toolchain/bin/arm-linux-androideabi-gcc"
    TOOLCHAIN="arm-linux-androideabi-4.9"
    COMPILER="arm-linux-androideabi-"
fi


##BUILD TOOLCHAIN: build gcc toolchain
if [ ! -x "$GCC" ]; then
    cd -- "$src_dir" || die "Cannot cd to $src_dir"
    "$ndk_dir/build/tools/make-standalone-toolchain.sh" \
        --platform=android-21 \
        --toolchain="$TOOLCHAIN" \
        --install-dir="$work_dir/android-arm-toolchain/" \
        --system="linux-$cpu_arch" \
            || die "Cannot run $ndk_dir/build/tools/make-standalone-toolchain.sh. Please re-run the script."
fi

##CLONE or PULL: the repo and change branch if requested
cd -- "$build_dir" || die "Cannot cd to $src_dir"

if cd cjdns; then
    git pull --ff-only
else
    git clone -- "$cjdns_repo" cjdns || die "Cannot clone $cjdns_repo"
    cd cjdns || die "Cannot cd to cjdns"
fi
if [ -n "$1" ]; then
    git checkout -- "$1" || die "Invalid commit $1"
fi
./clean || die "Too old version of git."

##SETUP TOOLCHAIN VARS
export PATH="$work_dir/android-arm-toolchain/bin:$PATH"

##BUILD cjdns (without tests)
log="$(CROSS_COMPILE="$COMPILER" ./cross-do 2>&1)"
success=$?

printf "%s" "$log" | tee cjdns-build.log

if ! $success || [ ! -f 'cjdroute' ]; then
    die "BUILD FAILED :("
fi
printf "\nBUILD COMPLETE! @ %s/cjdns/cjdroute\n" "$build_dir"
unset success log

##PACKAGE CJDROUTE AND ASSOCIATED SCRIPTS FOR DEPLOYMENT
cd -- "$build_dir" || die "Cannot cd to $build_dir"
cjdns_version="$(git -C cjdns describe --always | sed 's|-|.|g;s|[^\.]*\.||;s|\.[^\.]*$||')"
file="../cjdroid-${cjdns_version}${cjdns_repo_branch}.tar.gz"
if [ -f "$file" ]; then
    die "Package not built because $(readlink -f "$file") already exists."
fi
if [ ! -f cjdns/cjdroute ]; then
    die "Package not built because $PWD/cjdns/cjdroute does not exist."
fi
if [ ! -d cjdns/contrib/android/cjdroid ]; then
    die "Package not built because $PWD/cjdns/contrib/android/cjdroid does not exist."
fi

cp -R cjdns/contrib/android/cjdroid .
install -Dm755 cjdns/cjdroute cjdroid/files/cjdroute
tar cfz "$file" cjdroid
printf "\nSuccess: A deployable package has been created @ %s\n" "$(readlink -f "$file")"
