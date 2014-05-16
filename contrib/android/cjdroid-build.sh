#!/usr/bin/env bash

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
#   Remove 'cjdns-android/cjdns' and below change: REPO="https://newaddr"
#
#  Use a different branch:
#   Below change: BRANCH="newbranch"

## Configurable Variables
REPO="https://github.com/cjdelisle/cjdns/"
BRANCH="master"
BUILD_DIR="$(pwd)/cjdns-android"
SRC_DIR="${BUILD_DIR}/source"
NDK_DIR="${SRC_DIR}/ndk"
WORK_DIR="${BUILD_DIR}/workspace"

## Other Variables
NDKVER="android-ndk-r9d"
ARCH=`uname -m`
[[ -z "$ARCH" ]] && (echo "ERROR: NO ARCH DETECTED"; exit 1)
[[ "$ARCH" = "i686" ]] && ARCH="x86"

## Make dirs if needed
install -d "$SRC_DIR"
install -d "$WORK_DIR"

## SETUP NDK
cd "$SRC_DIR"
if [ -z "$NDK" ]; then
    echo "${NDKVER}-linux-${ARCH}.tar.bz2"
    [[ -f "${NDKVER}-linux-${ARCH}.tar.bz2" ]] || wget "http://dl.google.com/android/ndk/${NDKVER}-linux-${ARCH}.tar.bz2" || (echo "Can't find download for your system" && exit 1)
    [[ -d "${NDKVER}" ]] || (tar jxf "${NDKVER}-linux-${ARCH}.tar.bz2" || exit 1)
    NDK="$NDKVER"
else
    [[ -d "$NDK" ]] || (echo "The NDK variable is not pointing to a valid directory" ; exit 1)
fi
[[ -e "$NDK_DIR" ]] && rm "$NDK_DIR" >> /dev/null 2>&1
ln -s "$NDK" "$NDK_DIR"

## BUILD toolchain - build gcc toolchain
cd "$BUILD_DIR"
if [ ! -x "${WORK_DIR}/android-arm-toolchain/bin/arm-linux-androideabi-gcc" ]; then
    cd "$SRC_DIR"
    "${NDK_DIR}/build/tools/make-standalone-toolchain.sh" \
        --platform=android-9 \
        --toolchain=arm-linux-androideabi-4.8 \
        --install-dir="${WORK_DIR}/android-arm-toolchain/" \
        --system=linux-${ARCH} \
        || exit 1
    # (android-9 includes pthread_rwlock_t which is used by libuv)
fi

##FETCH cjdns
cd "$BUILD_DIR"
if [ ! -d cjdns ]; then
    git clone ${REPO} cjdns || exit 1
    (cd cjdns && git checkout ${BRANCH}) || exit 1
else
    (cd cjdns && git pull && git checkout ${BRANCH}) || exit 1
fi

##SETUP toolchain vars
export PATH="${WORK_DIR}/android-arm-toolchain/bin:${PATH}"

##BUILD cjdns (without tests)
(cd cjdns; ./clean; CROSS_COMPILE=arm-linux-androideabi- ./cross-do) 2>&1 | tee cjdns-build.log || (echo "Failure while building, check cjdns-build.log"; exit 1)
[[ -x cjdns/cjdroute ]] && echo -e "\nBUILD COMPLETE! @ ${BUILD_DIR}/cjdns/cjdroute" || (echo -e "\nBUILD FAILED :("; exit 1)

##PACKAGE cjdroute and associated scripts for deployment
VERSION=$(git -C cjdns describe --always | sed 's|-|.|g;s|[^\.]*\.||;s|\.[^\.]*$||')
if [ ! -f "../cjdroid-${VERSION}.tar.gz" ]; then
    if [ -d cjdns/contrib/android/cjdroid ]; then
        cp -R cjdns/contrib/android/cjdroid .
        if [ -f cjdns/cjdroute ]; then
            install -Dm755 cjdns/cjdroute cjdroid/files/cjdroute
        else
            echo "Error: Package not built because ${PWD}/cjdns/cjdroute does not exist"
            exit 1
        fi
        tar cfz ../cjdroid-${VERSION}.tar.gz cjdroid
        echo -e "\nSuccess: A deployable package has been created @ $(pwd | sed 's/\/[^\/]*$//g')/cjdroid-${VERSION}.tar.gz"
    else
        echo "Error: Package not built because ${PWD}/cjdns/contrib/android/cjdroid does not exist"
        exit 1
    fi
else
    echo "Error: Package not built because $(pwd | sed 's/\/[^\/]*$//g')/cjdroid-${VERSION}.tar.gz already exists"
fi

##SAMPLES for deployment
#- settings > developer options > root access > adb or adb+apps
#  adb root
#  adb shell "whoami"
#- adb push build_linux/testcjdroute /data/local/
#  adb shell "chmod 500 /data/local/testcjdroute"
#  adb shell "/data/local/testcjdroute all"
#- adb push cjdroute /data/local/

#  adb shell "chmod 500 /data/local/cjdroute"
#- logging.logTo = stdout
#  noBackground = 1
#  adb push cjdroute.conf /data/local/
#  adb shell "chmod 400 /data/local/cjdroute.conf"
#- adb shell "/data/local/cjdroute < /data/local/cjdroute.conf"
