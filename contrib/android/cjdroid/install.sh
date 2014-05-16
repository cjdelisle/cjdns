#!/system/xbin/bash

cd ${0%/*}

if [ ! `whoami` = "root" ]; then echo "Please run this script as root"; exit 1; fi

SYSTEM=`mount | grep " /system " | grep -oe "^[^\ ]*"`
if [ ! -e "$SYSTEM" ]; then echo "The device /system is mounted on could not be detected"; exit 1; fi

mount -o rw,remount $SYSTEM /system
cp files/cjdaemon /system/bin/
cp files/cjdctl /system/bin/
cp files/cjdroute /system/bin/
cp files/99cjdroute /system/etc/init.d/
chmod 755 /system/bin/cjd*
chmod 755 /system/etc/init.d/99cjdroute
mount -o ro,remount $SYSTEM /system
