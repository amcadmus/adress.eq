#!/bin/bash

source env.sh
source parameters.sh
source functions.sh

last_step_num=`printf "%03d" $last_step_num`
last_step_dir=step.$last_step_num.tf
last_iter=`printf "%03d" $tf_iterations_max`
last_iter=step_$last_iter

if test ! -d $last_step_dir; then
    echo "no dir $last_step_dir"
    exit
fi
if test ! -d $last_step_dir/$last_iter; then
    echo "no dir $last_step_dir/$last_iter"
    exit
fi
if test -d prod; then
    echo "exists dir prod, move"
    mv prod prod.`date +%s`
fi

function test_and_move () {
    fromfile=$1
    tofile=$2
    if test -f $fromfile; then
	rm -f $tofile
	cp $fromfile $tofile
    else
	echo "no file $fromfile"
	exit
    fi
}

cp -a template.prod prod
cd prod
rm -f prod.log
cp ../parameters.sh .
test_and_move ../$last_step_dir/$last_iter/confout.gro ./conf.gro
test_and_move ../$last_step_dir/$last_iter/index.ndx ./index.ndx
test_and_move ../$last_step_dir/grompp.mdp ./grompp.mdp
prod_grompp
test_and_move ../$last_step_dir/topol.top ./topol.top
test_and_move ../$last_step_dir/tablerdf.xvg ./tablerdf.xvg
test_and_move ../$last_step_dir/tabletf.xvg ./tabletf.xvg

grompp -n index.ndx &>> prod.log
mdrun -v &>> prod.log
cd ..
