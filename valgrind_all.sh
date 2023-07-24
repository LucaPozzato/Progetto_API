#!/bin/bash

rm valgrind.txt
./build.sh

LIST=`seq 1 100`

for F in ${LIST}
do
    BF=`basename open_${F}.txt | sed -e "s/.txt//"`
    ./valgrind.sh $BF
done

