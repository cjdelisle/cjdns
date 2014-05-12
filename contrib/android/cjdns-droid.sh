#!/bin/bash

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

# Possible Deps
#  CyanogenMod has a TUN device as do a few branded phones.
#  If your phone doesn't have a TUN device at /dev/tun get it below,
#  or find com.aed.tun.installer app on your own.
#  http://cjdns.ca/com.aed.tun.installer.apk

# Report success/failure including phone type, android version, kernel version,
# and as much information as possible to #cjdns @ HypeIRC

# NOTE: TO USE CUSTOM NDK DIRECTORY, SET NDK="/path/to/ndk" AT RUNTIME

## Configurable Variables
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
#  (android-9 includes pthread_rwlock_t which is used by libuv)
fi

##FETCH cjdns
cd "$BUILD_DIR"
if [ ! -d cjdns ]; then
#  git clone https://github.com/lgierth/cjdns cjdns || exit 1
#  (cd cjdns && git checkout android-ndk) || exit 1
  git clone https://github.com/cjdelisle/cjdns/ cjdns || exit 1
  (cd cjdns && git checkout crashey) || exit 1
else
  (cd cjdns && git pull) || exit 1
fi

##SETUP toolchain vars
export PATH="${WORK_DIR}/android-arm-toolchain/bin:${PATH}"

##BUILD cjdns (without tests)
(cd cjdns; ./clean; CROSS_COMPILE=arm-linux-androideabi- ./cross-do) 2>&1 | tee cjdns-build.log || (echo "Failure while building, check cjdns-build.log"; exit 1)
[[ -x cjdns/cjdroute ]] && echo -e "\nBUILD COMPLETE! @ ${BUILD_DIR}/cjdns/cjdroute" || (echo -e "\nBUILD FAILED :("; exit 1)

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
