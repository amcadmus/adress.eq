#!/bin/bash

#intervals="3.75 4.25 5.16 6.08 7.00 7.50"
intervals="3.75 4.25 5.16 6.08 7.00 7.50"
#intervals="3.75 4.25 5.16"
refh=0.01

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
    command="./tools/analyze.rdf/cal.rdf3 -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $lower --x1 $upper --refh $refh -o rdf3.$print_lower-$print_upper.out --rdf3out rdf3.$print_lower-$print_upper"
    echo command is $command
#    ./tools/analyze.rdf/cal.rdf -u 1.1 -c 1.15 -m $method -b 100 -e $time --x0 $lower --x1 $upper -o rdf.$print_lower-$print_upper.out
    $command
    lower=$upper
done
interval_end=$i

rm -fr rdf3.inter.9
mkdir rdf3.inter.9
mv -f rdf3*out rdf3.inter.9

