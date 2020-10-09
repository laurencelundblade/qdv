#!/bin/sh

cc -I ../inc errstringtest.c ../src/qcbor_err_to_str.c 

./a.out

