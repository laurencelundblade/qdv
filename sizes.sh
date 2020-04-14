#!/bin/sh

fun=`nm libqcbor.a | awk '/ [TtSs] /{printf "%s|", $3 }'; echo xxxxxxxxxxxxxx`
nm -r -n -t d -U $1 |\
grep ' [TtSs] ' |\
awk '{printf "%-40s %4s\n", $3,  offset - $1 }
     {offset=$1}' |\
egrep $fun | sort -r -n  -k 2 | tee /tmp/foofoo.txt

cat /tmp/foofoo.txt | awk '{sum += $2} END {print "total                                    " sum}'




