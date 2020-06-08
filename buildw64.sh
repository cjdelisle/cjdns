#!/bin/bash
SYSTEM=win64 CROSS_CFLAGS="-std=gnu11 -gdwarf -gstatement-frontiers -ginline-points -DDEBUG -fstack-protector-all -ggdb -g -O0" CROSS_COMPILE=x86_64-w64-mingw32- ./cross-do
cp build_win64/client_cjdroute2_c.exe /media/sf_sharedv/cjdroute.exe
