#!/bin/bash

source parameters.sh

mylog=gen.tf.log
makelog=make.log

# prepare conf.gro
## gen from base (default spc216.gro)
editconf -f $base_conf -o conf.gro -box $n_base_block &> $mylog
## resize to right density
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
natom=`head -n 2 conf.gro | tail -n 1`
nmol=`echo "$natom / 3" | bc`
now_density=`echo "$natom / 3 / ($boxx * $boxy * $boxz)" | bc -l`
scale=`echo "($number_density / $now_density)" | bc -l"`
editconf -f conf.gro -o out.gro -scale "$scale 1 1"
mv -f out.gro conf.gro
## warm run
cp -a tools/atom.template ./warmup
cd warmup
mv ../conf.gro .
sed "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top
grompp &> $mylog
mdrun -v &> $mylog
## add COM site
make -C tools/gen.conf/ clean &> $makelog
make -j4 -C tools/gen.conf/ &> $makelog
tools/gen.conf/