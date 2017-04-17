#!/bin/bash

#change this to valid ndk path
ANDROID_NDK_PATH=/bad/path/to/android-ndk-r10d/
ANDROID_PROJECT_PATH=$(pwd)/../android
ANDROID_BUILD_PATH=$(pwd)/build_android

if [ "$ANDROID_NDK_PATH" == "/bad/path/to/android-ndk-r10d/" ];
then
  echo -e "\e[1;31mPlease, change Android NDK path!\e[0m"  
  exit 1
fi
android_log=android_build_$$.log
enabled_log=${LOG}

mkdir $ANDROID_BUILD_PATH

$ANDROID_NDK_PATH/build/tools/make-standalone-toolchain.sh --platform=android-21 --toolchain=arm-linux-androideabi-4.9 --install-dir=$ANDROID_BUILD_PATH/arm-linux-androideabi/ --system=linux-x86_64

Seccomp_NO=1

PATH=$PATH:$(pwd)/arm-linux-androideabi/bin
CROSS_COMPILE=arm-linux-androideabi-
TARGET_ARCH=arm
gcc_version=$(${CC} --version)
log_filename="${android_log%.*}"
echo Using $gcc_version
rm -rf build_*
if [ "x$enabled_log" == "x1" ]; then
  echo Compiler CC: $CC > $android_log
  echo Compiler CFLAGS: $CFLAGS >> $android_log
  echo Compiler LDFLAGS: $LDFLAGS >> $android_log
  time ./do >> $android_log 2>&1
  mv cjdroute ${log_filename}_cjdroute
else
  echo Compiler CC: $CC
  echo Compiler CFLAGS: $CFLAGS
  echo Compiler LDFLAGS: $LDFLAGS
  time ./do
fi

cp cjdroute $ANDROID_PROJECT_PATH/src/main/assets/armeabi-v7a/ || ret=$?

if [ "$ret" != "" ] && [ "$ret" != "0" ];
then
  echo -e "\e[1;31mCopying failed, non zero status returned - $ret\e[0m"
else
  echo -e "\e[1;32mCopied successfully \e[0m"
fi

exit $ret
