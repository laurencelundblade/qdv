#!/bin/sh

# It is not possible to get the size of the last symbol because
# nm doesn't output the end of the last item in the list. 
# Also, the offsets of the static data variables don't mix
# with the offsets of program text, so they are omitted

nm -n -t d -U $1 |\
grep ' [Tt] ' |\
awk 'NR!=1{printf "%-40s %4s\n", name, $1 - offset }
     {offset=$1; name=$3}' |\
grep -v __mh_execute |\
sort -r -n  -k 2 | tee /tmp/sizee.$$

cat /tmp/sizee.$$ | awk '{sum += $2} END {print "total                                    " sum}'

rm -f /tmp/sizee.$$



#awk 'NR!=1{printf "%-40s %4s\n", $3,  offset - $1 }
#     {offset=$1}' |\
