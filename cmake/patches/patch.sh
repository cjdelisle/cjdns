#!/bin/sh
ls $1 | while read x
do
    echo $x | grep '.patch$' >/dev/null || continue;
    echo "applying $x"
    patch -p1 < $1/$x
done
