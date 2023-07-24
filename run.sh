#!/bin/bash

mkdir -p Output
./build.sh

if [ $# -eq 1 ]
then
    INPUT=$1
fi

/usr/bin/time -o Output/${INPUT}.time Sviluppo/file_ver Test/archivio_test_aperti/${INPUT}.txt
echo

diff -u Test/archivio_test_aperti/${INPUT}.output.txt Sviluppo/output.txt


if [ $? -eq 0 ]
then
    OUTPUT=${INPUT}.ok
    echo "    SUCCESS    "
    echo
else
    OUTPUT=${INPUT}.ko
    echo "*** FAILURE ***"
    echo
fi

mv Sviluppo/output.txt "Output/${OUTPUT}"
