#!/system/bin/sh

# Change to the script directory
if [ ! "${0%/*}" = $(echo $0 | sed 's|^.*/||') ]; then cd "${0%/*}"; fi

# Fail if user isn't root
if [ ! `whoami` = "root" ]; then echo "Please run this script as root"; exit 1; fi

# Check that /dev/net/tun exists and create it if it doesn't
if [ ! -c /dev/tun ]; then
    mknod /dev/tun c 10 200
fi

# Remount /system read/write
SYSTEM=`mount | grep " /system " | grep -oe "^[^\ ]*"`
if [ ! -e "$SYSTEM" ]; then echo "The device /system is mounted on could not be detected"; exit 1; fi
mount -o rw,remount $SYSTEM /system

# Copy cjdns-related-files to /system
cp files/cjdaemon /system/bin/
cp files/cjdctl /system/bin/
cp files/cjdroute /system/bin/
cp files/99cjdroute /system/etc/init.d/

# Set permissions for cjdns-related-files
chmod 755 /system/bin/cjd*
chmod 755 /system/etc/init.d/99cjdroute

# Remount /system read-only
mount -o ro,remount $SYSTEM /system
