#!/bin/bash
#
# b.sh -- Before-release comprehensive test and build script for QCBOR
#
# Copyright (c) 2019-2021, Laurence Lundblade. All rights reserved.
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


# Build and print out the code size for smallest configuration 
make clean > /dev/null
make -f qdv/Makefile.min decode_min encode_min > /dev/null
qdv/sizes.sh decode_min 
qdv/sizes.sh encode_min

# Build and print out the code size for largest configuration
make clean > /dev/null
make -f qdv/Makefile.max decode_max encode_max > /dev/null
qdv/sizes.sh decode_max 
qdv/sizes.sh encode_max



# Compute the combinations of ifdefs and write to lines in a file
set="-DQCBOR_DISABLE_FLOAT_HW_USE"
set+=" -DQCBOR_DISABLE_PREFERRED_FLOAT"
set+=" -DQCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA"
set+=" -DQCBOR_DISABLE_ENCODE_USAGE_GUARDS"
set+=" -DQCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS"
set+=" -DQCBOR_DISABLE_INDEFINITE_LENGTH_ARRAYS"
set+=" -DQCBOR_DISABLE_UNCOMMON_TAGS"

stringpermutations "" "$set" > /tmp/b.$$


# The QCBOR makefile uses a minimum of compiler flags so that it will
# work out-of-the-box with a wide variety of compilers. This is where
# compilation with a full set of warning flags is performed. This set
# of flags has been thought through for gcc and clang/llvm.
warn_flags="-Wall"
warn_flags+=" -Wextra"
warn_flags+=" -Wpedantic"
warn_flags+=" -Wshadow"
warn_flags+=" -Wconversion"
warn_flags+=" -Wcast-qual"

cpp_warn_flags="$warn_flags"
cpp_warn_flags+=" -std=c++11"

# Build for C++ with clang and LLVM
make -f qdv/Makefile.min clean > /dev/null
make -f qdv/Makefile.min qcbormincpp "CMD_LINE=$warn_flags" "CXX_CMD_LINE=$cpp_warn_flags"
make -f qdv/Makefile.max clean > /dev/null
make -f qdv/Makefile.max qcbormincpp "CMD_LINE=$warn_flags" "CXX_CMD_LINE=$cpp_warn_flags"


# Add these after the C++ tests
warn_flags+=" -std=c99" 
warn_flags+=" -xc"
warn_flags+=" -Wstrict-prototypes"


# Make once with the default compiler, llvm/clang on MacOS, and all
# the warning flags set
make --silent "CMD_LINE=$warn_flags" 2>&1 | grep -v 'ar: creating'


# This is the big test fan out that takes some time to run. All
# combinations of configuration flags are compiled and the tests run.
# All warnings are turned on.  GCC is used since it is more strict.
while read opts; do
   echo "$opts"
   make clean > /dev/null
   # Throw away stdout, but not stderr so the compiler warnings show
   make --silent "CC=/usr/local/bin/gcc-10" "CMD_LINE=$opts $warn_flags" 2>&1 | grep -v 'ar: creating'
   ./qcbortest > /tmp/bb.$$
   grep SUMMARY /tmp/bb.$$
done < /tmp/b.$$

rm -f /tmp/b.$$ /tmp/bb.$$

