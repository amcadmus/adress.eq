#!/bin/bash

source env.sh
source parameters.sh
source functions.sh

logfile=noibi.log
last_step_num=`printf "%03d" $last_step_num`
last_step_dir=step.000.tf/tf/step_$last_step_num

if test ! -d $last_step_dir; then
    echo "no dir $last_step_dir"
    exit
fi
if test -d noibi; then
    echo "exists dir noibi, move"
    mv noibi noibi.`date +%s`
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

cp -a template.prod noibi
cd noibi
rm -f $logfile
echo running on `uname -n` >> $logfile
echo with gromacs command `which mdrun` >> $logfile
cp ../parameters.sh .
test_and_move ../$last_step_dir/confout.gro ./conf.gro
test_and_move ../$last_step_dir/index.ndx ./index.ndx
test_and_move ../$last_step_dir/grompp.mdp ./grompp.mdp
prod_grompp
test_and_move ../$last_step_dir/topol.top ./topol.top
test_and_move ../$last_step_dir/tablerdf.xvg ./tablerdf.xvg
test_and_move ../$last_step_dir/tabletf.xvg ./tabletf.xvg

grompp -n index.ndx &>> $logfile
mdrun -v &>> $logfile
cd ..
