#!/bin/sh

CMD=$1
shift
for arg do
    $CMD $arg -k 3 | diff -u --from-file ${arg}.eta.k3 - || exit 1
done
