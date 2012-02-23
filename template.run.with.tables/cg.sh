#!/bin/bash

source env.sh
source parameters.sh

mylog=`pwd`/atom.log
makelog=`pwd`/make.log
rm -f $mylog

echo "# running on `uname -n`"
echo "# mdrun command is `which mdrun`"

# prepare conf.gro
echo "# prepare conf.gro"
## gen from base (default spc216.gro)
echo "## gen from base (default spc216.gro)"
genconf -f $base_conf -o conf.gro -nbox $n_base_block &>> $mylog
## resize to right density step 1
echo "## resize to right density step 1"
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
natom=`head -n 2 conf.gro | tail -n 1`
nmol=`echo "$natom / 3" | bc`
now_density=`echo "$natom / 3 / ($boxx * $boxy * $boxz)" | bc -l`
scale=`echo "($now_density / $number_density)" | bc -l`
editconf -f conf.gro -o out.gro -scale $scale 1 1 &>> $mylog
mv -f out.gro conf.gro
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
## resize to right density step 2
echo "## resize to right density step 2"
newboxx=`printf "%.1f" $boxx`
scalex=`echo "$newboxx / $boxx" | bc -l`
scaleyz=`echo "sqrt(1./$scalex)" | bc -l`
editconf -f conf.gro -o out.gro -scale $scalex $scaleyz $scaleyz &>> $mylog
mv -f out.gro conf.gro
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
## warm up run"
echo "## warm up run"
rm -fr warmup
cp -a tools/atom.template ./warmup
cd warmup
mv ../conf.gro .
sed "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top
rand_seed=`date +%s`
sed -e "/^gen_seed/s/=.*/= $rand_seed/g" grompp.mdp > grompp.mdp.tmp
mv -f grompp.mdp.tmp grompp.mdp
$std_gromacs_install_dir/bin/grompp &>> $mylog
$std_gromacs_install_dir/bin/mdrun -v &>> $mylog
rm -fr ../conf.gro
if test ! -f confout.gro; then
    echo "no file confout.gro, exit!"
    exit
fi
cp confout.gro ../conf.gro
cd ..
## add COM site"
echo "## atom to CG"
make -C tools/gen.conf/ clean &> $makelog
make -j4 -C tools/gen.conf/ &> $makelog
tools/gen.conf/atom2cg -f conf.gro -o out.gro &>> $mylog
mv -f out.gro conf.gro

# prepare index file"
echo "# prepare index file"
echo "a CG" > command.tmp
echo "q" >> command.tmp
cat command.tmp  | make_ndx -f conf.gro &>> $mylog
rm -fr command.tmp

# prepare all tables
echo "# prepare all tables"
if test ! -d $table_dir; then
    echo "no table dir $table_dir"
    exit
fi
rm -f table*xvg
cp -L $table_dir/table_CG_CG.xvg .

# prepare grompp.mdp
echo "# prepare grompp.mdp"
rm -fr grompp.mdp
cp tools/cg.template/grompp.mdp .
sed -e "/^nsteps/s/=.*/= $gmx_nsteps/g" grompp.mdp |\
sed -e "/^nstenergy/s/=.*/= $gmx_nstenergy/g" |\
sed -e "/^gen_vel/s/=.*/= no/g" |\
sed -e "/^nstxtcout/s/=.*/= $gmx_nstxtcout/g" > grompp.mdp.tmp
mv -f grompp.mdp.tmp grompp.mdp

# prepare topol.top
echo "# prepare topol.top"
rm -fr topol.top 
cp tools/cg.template/topol.top .
sed "s/^SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top

# productive run
echo "# productive run"
$std_gromacs_install_dir/bin/grompp &>> $mylog
$std_gromacs_install_dir/bin/mdrun -v &>> $mylog

