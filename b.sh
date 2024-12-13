#!/bin/bash
#
# b.sh -- Before-release comprehensive test and build script for QCBOR
#
# Copyright (c) 2019-2024, Laurence Lundblade. All rights reserved.
#
# SPDX-License-Identifier: BSD-3-Clause
#
# See BSD-3-Clause license in README.md
#

# This creates strings with all the permutations of the compiler ifdefs
function stringpermutations {
    local prefix=$1 # the prefix is the first argument
    local theset=$2 # the set left to process is the second argument
  
    # Output the new prefix
    echo "$prefix"

    # Loop over each item in the set adding it to the prefix
    # All items eventually get added to the prefix and the set
    # goes to nothing. Prefixes are what is output
    for i in $theset; do

        # Make a new prefix by appending the item from the set to it
        local newprefix="$prefix ${i}" 

        # Update the set by removing one more item from it 
        if [[ ! $theset = *[\ ]* ]]; then
            theset=""
        else
            theset=${theset#* }
        fi

        if [[ ! -z "$theset" ]]; then
           # The set is not empty, recurse to process it
           stringpermutations "$newprefix" "$theset"
        else
           # The set is empty, just output the new prefix
           echo "$newprefix"
        fi

        #echo "set:" $set
        #echo "pre:" $pre
    done
}


# The set of pre-processor defines for disabling features
set="-DQCBOR_DISABLE_FLOAT_HW_USE"
set+=" -DQCBOR_DISABLE_PREFERRED_FLOAT"
set+=" -DQCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA"
set+=" -DQCBOR_DISABLE_ENCODE_USAGE_GUARDS"
set+=" -DQCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS"
set+=" -DQCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS"
set+=" -DQCBOR_DISABLE_UNCOMMON_TAGS"
set+=" -DUSEFULBUF_DISABLE_ALL_FLOAT"
set+=" -DQCBOR_DISABLE_TAGS"
set+=" -DQCBOR_DISABLE_NON_INTEGER_LABELS"

# Compute the combinations of ifdefs and write to lines in a file
stringpermutations "" "$set" > /tmp/b.$$


# Build and print out the code size for smallest configuration
make clean > /dev/null
make "CMD_LINE=$set" libqcbor.a > /dev/null
make -f qdv/Makefile.qdv decode_min encode_min > /dev/null
qdv/sizes.sh decode_min
qdv/sizes.sh encode_min


# Build and print out the code size for largest configuration
make clean > /dev/null
make libqcbor.a > /dev/null
make -f qdv/Makefile.qdv decode_max encode_max > /dev/null
qdv/sizes.sh decode_max
qdv/sizes.sh encode_max


# Link the library with a C++ app
make clean > /dev/null
make libqcbor.a > /dev/null
make -f qdv/Makefile.qdv qcbormincpp


# Make once with warnings on
# the warning flags set
make --silent warn 2>&1 | grep -v 'ar: creating'

#TODO: fix this up to use either/both llvm or gcc
#It used to be like this, but gcc doesn't work (so far) on latest MacOS versions

# This is the big test fan out that takes some time to run. All
# combinations of configuration flags are compiled and the tests run.
# All warnings are turned on.  It would be good to use GCC is used
# but it is hard to install on latest MacOS. Gcc is more strict.
while read opts; do
   echo "$opts"
   make clean > /dev/null
   # Throw away stdout, but not stderr so the compiler warnings show
   make --silent warn "CMD_LINE=$opts" 2>&1 | grep -v 'ar: creating'
   ./qcbortest > /tmp/bb.$$
   grep SUMMARY /tmp/bb.$$
done < /tmp/b.$$

rm -f /tmp/b.$$ /tmp/bb.$$

