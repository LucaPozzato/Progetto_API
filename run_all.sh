#!/bin/bash


rm -rf Output/*
#LIST=`find Test/archivio_test_aperti/ -type f -not -name \*output.txt | sort`
LIST=`seq 1 100`

for F in ${LIST}
do
    BF=`basename open_${F}.txt | sed -e "s/.txt//"`
    ./run.sh $BF
done

ls -l Output/*.ko
