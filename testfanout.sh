#!/bin/bash

function foo {
    local prefix=$1 # the prefix is the first argument
    local theset=$2 # the set left to process is the second argument
  
    # Output the new prefix
    echo "OUT1: $prefix"

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
           foo "$newprefix" "$theset"
        else
           # The set is empty, just output the new prefix
           echo "OUT2: $newprefix"
        fi

        #echo "set:" $set
        #echo "pre:" $pre
    done
}

set="QCBOR_DISABLE_FLOAT_HW_USE QCBOR_DISABLE_PREFERRED_FLOAT QCBOR_CONFIG_DISABLE_EXP_AND_MANTISSA"

foo "" "$set"

