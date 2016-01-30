#!/bin/sh
for x in $* ; do
    echo Running $x
    ./$x
done
