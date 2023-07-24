#!/bin/bash

if [ $# -eq 1 ]
then
    INPUT=$1
fi

echo valgrind test: ${INPUT} >>valgrind.txt
valgrind    --leak-check=full --show-leak-kinds=all --track-origins=yes -s \
            Sviluppo/progetto Test/archivio_test_aperti/${INPUT}.txt >>valgrind.txt 2>&1

diff -u Test/archivio_test_aperti/${INPUT}.output.txt Sviluppo/output.txt >>valgrind.txt 2>&1
echo ' ' >>valgrind.txt
echo ' ' >>valgrind.txt

if [ $? -eq 0 ]
then
    echo "  *** SUCCESS ***"
else
    echo "  *** FAILURE ***"
fi
