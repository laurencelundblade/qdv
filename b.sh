#!/bin/bash


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

make clean > /dev/null
make -f qdv/Makefile.min decode_min encode_min > /dev/null
qdv/sizes.sh decode_min 
qdv/sizes.sh encode_min

make clean > /dev/null
make -f qdv/Makefile.max decode_max encode_max > /dev/null
qdv/sizes.sh decode_max 
qdv/sizes.sh encode_max

# Compute the combinations of ifdefs and write to lines in a file
set="-DQCBOR_DISABLE_FLOAT_HW_USE -DQCBOR_DISABLE_PREFERRED_FLOAT -DQCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA -DQCBOR_DISABLE_ENCODE_USAGE_GUARDS -DQCBOR_DISABLE_INDEFINITE_LENGTH_STRINGS"
stringpermutations "" "$set" > /tmp/b.$$

# make and test each combination of ifdefs
while read opts; do
   echo "$opts"
   make clean > /dev/null
   # Throw away stdout, but not stderr because that's were compiler warnings and erros show
   # All the extra error check options here so they are run before releases
   make "CMD_LINE=$opts -Wall -pedantic-errors -Wextra -Wshadow -Wparentheses -Wconversion -xc -std=c99" 2>&1 >/dev/null | grep -v 'ar: creating'
   ./qcbortest > /tmp/bb.$$
   grep SUMMARY /tmp/bb.$$
done < /tmp/b.$$

rm -f /tmp/b.$$ /tmp/bb.$$

