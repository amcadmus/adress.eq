#!/bin/bash

if [ $# -ne 1 ]; then
    echo "use: "
    echo "count.interval.sh method"
    exit
fi
method=$1
centers="4.00000 4.46429 4.92858 5.39287 5.85716 6.32145 6.78574 7.2500"
width=0.5
yz1=3.72

hwidth=`echo "$width / 2.0" | bc -l`

for i in $centers;
do
    x0=`echo "$i - $hwidth" | bc -l`
    x1=`echo "$i + $hwidth" | bc -l`
    line=`./tools/analyze.number/count.interval -b 1000 --x0 $x0 --x1 $x1 --yz1 $yz1 -m $method | grep -v \#`
    echo "$i $x0 $x1 $line"
done

    