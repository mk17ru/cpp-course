#!/bin/sh

CMD=$1
shift
for arg do
    $CMD $arg -k 2 | diff -u --from-file ${arg}.eta.k2 - || exit 1
done
