#!/bin/bash

#
# A script to find and configure an available tun interface.
#

if ! [[ `uname -s` == "Darwin" ]]
then
    echo "This script is only for Mac OS X"
    exit
fi

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

avail_tun_size=${#avail_tun[@]}
avail_tun_max=$((avail_tun_size - 1))

for i in $(lsof -Fn | tail -n +2 | cut -c2- | grep "/dev/tun" | sort -u | uniq ) ; do
    for ((j = 0; j <= avail_tun_max; j++)); do
        if [ $(echo "$i" | grep "${avail_tun[j]}") ]; then
            unset avail_tun[$j]
        fi
    done
done

for i in ${avail_tun[@]} ; do
    echo $i
done
