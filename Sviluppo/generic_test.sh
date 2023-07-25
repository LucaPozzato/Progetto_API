#!/bin/bash

gcc Sviluppo/generic_test.c -o Sviluppo/generic_test
/usr/bin/time ./Sviluppo/generic_test << EOF
$(
    for i in {1..6000000}; do
        echo A;
    done
)
a
EOF