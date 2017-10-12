#!/bin/bash

killall tunsocks cjdroute
rm -rf pipe0 pipe1

mkfifo pipe0 pipe1

./tunsocks -D 8080 > pipe0 < pipe1 &
#prog2 < pipe0 > pipe1

#export LD_PRELOAD=$(pwd)/libpreload.so
./cjdroute --nobg < ./cjdroute.conf
