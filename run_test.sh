#!/bin/bash

rm results_test.txt
gcc -Dlocal -Dfile -Wall -Werror -std=gnu11 -O2 -lm Sviluppo/list_ver.c -o Sviluppo/list_ver

for i in {1..111}
do
    echo test number: $i >>results_test.txt
    /usr/bin/time -h ./Sviluppo/list_ver Test/archivio_test_aperti/open_$i.txt >>results_test.txt 2>&1
    diff -u Sviluppo/output.txt Test/archivio_test_aperti/open_$i.output.txt >>results_test.txt 2>&1
    echo ' ' >>results_test.txt
    echo ' ' >>results_test.txt
done
