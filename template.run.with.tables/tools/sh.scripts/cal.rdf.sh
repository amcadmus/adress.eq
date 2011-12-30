#!/bin/bash

intervals="3.75 4.25 4.80 5.35 5.90 6.45 7 7.5"
intervals="3.75 4.25 5.16 6.08 7.00 7.50"
make -C ./tools/analyze.rdf/ -j4 &> /dev/null

c=0
for i in $intervals;
do
    echo $i
    if test $c -eq 0; then
	c=$(($c+1))
	lower=$i
	interval_start=$i
	continue
    fi
    upper=$i
    print_lower=`echo "$lower * 100" | bc -l`
    print_lower=`printf "%.0f" $print_lower`
    print_upper=`echo "$upper * 100" | bc -l`
    print_upper=`printf "%.0f" $print_upper`
    nstep=`grep gmx_nsteps parameters.sh | cut -d '=' -f 2`
    time=`echo "$nstep * 0.002" | bc -l`
    if grep COM conf.gro &> /dev/null ; then
	method=adress
    else
	method=atom
    fi
    echo "./tools/analyze.rdf/cal.rdf -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $lower --x1 $upper -o rdf.$print_lower-$print_upper.out"
    ./tools/analyze.rdf/cal.rdf -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $lower --x1 $upper -o rdf.$print_lower-$print_upper.out
    lower=$upper
done
interval_end=$i

echo "./tools/analyze.rdf/cal.rdf -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $interval_start --x1 $interval_end -o rdf.all.out"
./tools/analyze.rdf/cal.rdf -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $interval_start --x1 $interval_end -o rdf.all.out
echo 2 2| g_rdf -xvg none -bin .01 -b 100  -n index.ndx -dt 1 -rdf mol_com