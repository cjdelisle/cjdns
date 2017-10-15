#!/bin/bash

#export LD_PRELOAD=$(pwd)/libpreload.so

#export VPNFDIN=9
#export VPNFDOUT=8

#export CJDNSFDIN=8
#export CJDNSFDOUT=9

sudo killall tunsocks cjdroute
rm -rf pipe0 pipe1

mkfifo pipe0 pipe1

./tunsocks -D 8080 > pipe0 < pipe1&

#sudo ./br&

if [ $? -eq 0 ]; then
    exec ./cjdroute --nobg < ./cjdroute.conf&>log&
else
    echo FAIL
fi
