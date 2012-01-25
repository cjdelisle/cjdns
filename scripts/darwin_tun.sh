#!/bin/bash

#
# A script to find an available tun interface.
#

if ! [ `uname -s` == "Darwin" ] ; then
    echo "DARWIN_TUN_NOT_USING_DARWIN"
    echo "This script is only for Mac OS X"
    exit
fi

if ! [ $(/usr/sbin/kextstat -l | awk '{print $6}' | grep foo.tun) ] ; then
    echo "DARWIN_TUN_KEXT_NOT_FOUND"
    echo "A TUN driver is not loaded. Please visit"
    echo "<http://tuntaposx.sourceforge.net/> for installation instructions."
    exit
fi

if [ "$(id -u)" != "0" ]; then
   echo "DARWIN_TUN_NOT_ROOT"
   echo "This script must be run as root" 1>&2
   exit 1
fi

if ! [ -c "/dev/tun0" ]; then
    echo "DARWIN_TUN_NOT_CHAR_DEVICE"
    echo "Unknown error. /dev/tun0 is not a character device."
fi

# A list of all possible tun devices.
avail_tun=(
    "/dev/tun0"
    "/dev/tun1"
    "/dev/tun2"
    "/dev/tun3"
    "/dev/tun4"
    "/dev/tun5"
    "/dev/tun6"
    "/dev/tun7"
    "/dev/tun8"
    "/dev/tun9"
    "/dev/tun10"
    "/dev/tun11"
    "/dev/tun12"
    "/dev/tun13"
    "/dev/tun14"
    "/dev/tun15"
)

# Search for opened tun devices and exclude them from the list.
orig_tun_count=${#avail_tun[@]}
orig_tun_max=$((orig_tun_count - 1))
for i in $(lsof -Fn | tail -n +2 | cut -c2- | grep "/dev/tun" | sort -u | uniq ) ; do
    for ((j = 0; j <= orig_tun_max; j++)); do
        if [ $(echo "$i" | grep "${avail_tun[j]}") ]; then
            unset avail_tun[$j]
        fi
    done
done

# Update length
avail_tun_size=${#avail_tun[@]}
avail_tun_max=$((avail_tun_size - 1))

# Pick a random tun device from the available ones.
tun_dev=${avail_tun[$[ ( $RANDOM % $avail_tun_max ) + 0 ]]}

if ! [ $tun_dev ] ; then
    echo "DARWIN_TUN_NONE_AVAILABLE"
    echo "You have no TUN devices available. Please free up some resources."
fi

# Output it back to the main program.
echo "DARWIN_TUN_DEVICE_AVAILABLE ${tun_dev}"

# EOF
