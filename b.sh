#!/bin/bash

make clean
make -f qdv/Makefile.min decode_min encode_min
qdv/sizes.sh decode_min 
qdv/sizes.sh encode_min

make clean
make -f qdv/Makefile.max decode_max encode_max
qdv/sizes.sh decode_max 
qdv/sizes.sh encode_max

