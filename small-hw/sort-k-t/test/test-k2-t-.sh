#!/bin/sh

CMD=$1
shift
for arg do
    $CMD $arg -k 2 -t - | diff -u --from-file ${arg}.eta.k2t- - || exit 1
done
