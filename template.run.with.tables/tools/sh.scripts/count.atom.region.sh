#!/bin/bash

if [ $# -ne 1 ]; then
    echo "use: "
    echo "count.interval.sh method"
    exit
fi
method=$1

source parameters.sh
boxx=`tail -n 1 confout.gro | awk '{print $1}'`
boxxh=`echo "$boxx * 0.5" | bc -l`
x0=`echo "$boxxh - $ex_region_r" | bc -l`
x1=`echo "$boxxh +  $ex_region_r" | bc -l`
yz1=`tail -n 1 confout.gro | awk '{print $2}'`
yz1=3.70

command="./tools/analyze.number/count.interval -b 500 --x0 $x0 --x1 $x1 --yz1 $yz1 -m $method "
echo command is $command
$command


