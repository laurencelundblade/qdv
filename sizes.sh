#!/bin/sh


fun=`nm libqcbor.a | awk '/ [TtSs] /{printf "%s|", $3 }'; echo xxxxxxxxxxxxxx`

# It is not possible to get the size of the last symbol because
# nm doesn't output the end of the last item in the list.
# Also, the offsets of the static data variables don't mix
# with the offsets of program text, so they are omitted

nm -n -t d -U $1 |\
grep ' [TtSs] ' |\
awk 'NR!=1{printf "%-40s %4s\n", name, $1 - offset }
     {offset=$1; name=$3}' |\
egrep $fun | sort -r -n  -k 2 | tee /tmp/sizes.$$

cat /tmp/sizes.$$ | awk '{sum += $2} END {print "total                                    " sum}'

rm -rf /tmp/sizes.$$




