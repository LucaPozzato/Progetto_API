#!/bin/bash

gcc test.c -o test
/usr/bin/time ./test << EOF
$(
    for i in {1..2000000}; do
        echo A;
    done
)
a
EOF