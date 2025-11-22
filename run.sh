#!/bin/bash
set -e

# compile the solution
gcc solution.c -o solution.out

# compile the tests
gcc test.c -o test.out

# run the tests
./test.out
