#!/bin/bash

mkdir -p Output

INPUT=open_50
if [ $# -eq 1 ]
then
    INPUT=$1
fi

/usr/bin/time -o Output/${INPUT}.time Sviluppo/progetto Test/archivio_test_aperti/${INPUT}.txt
echo

diff -u Test/archivio_test_aperti/${INPUT}.output.txt Sviluppo/output.txt


if [ $? -eq 0 ]
then
    OUTPUT=${INPUT}.ok
    echo SUCCESS
else
    OUTPUT=${INPUT}.ko
    echo
    echo "  *** FAILURE ***"
fi

mv Sviluppo/output.txt "Output/${OUTPUT}"
