#!/bin/bash

source parameters.sh
source functions.sh

logfile=atom.log
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
if test -d atom; then
    echo "exists dir atom, move"
    mv atom atom.`date +%s`
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

cp -a template.atom atom
cd atom
rm -f $logfile
echo running on `uname -n` >> $logfile
echo with gromacs command `which mdrun` >> $logfile
cp ../parameters.sh .
prod_grompp
test_and_move ../$last_step_dir/$last_iter/confout.gro ./conf.gro
natom=`head conf.gro -n 2 | tail -n 1`
nmol=`echo "$natom / 4" | bc`
natom=`echo "$nmol * 3" | bc`
echo "" > tmp.gro
echo $natom >> tmp.gro
grep SOL conf.gro  | grep -v COM | grep -v CG >> tmp.gro
tail -n 1 conf.gro >> tmp.gro
mv -f tmp.gro conf.gro
sed "s/SOL .*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top

grompp &>> $logfile
mdrun -v &>> $logfile

cd ..
