#!/bin/bash

source env.sh
source parameters.sh

mylog=`pwd`/gen.tf.log
makelog=`pwd`/make.log

# prepare conf.gro
## gen from base (default spc216.gro)
genconf -f $base_conf -o conf.gro -nbox $n_base_block &> $mylog
## resize to right density
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
natom=`head -n 2 conf.gro | tail -n 1`
nmol=`echo "$natom / 3" | bc`
now_density=`echo "$natom / 3 / ($boxx * $boxy * $boxz)" | bc -l`
scale=`echo "($number_density / $now_density)" | bc -l`
editconf -f conf.gro -o out.gro -scale "$scale 1 1" &> $mylog
mv -f out.gro conf.gro
boxx=`tail conf.gro -n 1 | awk '{print $1}'`
boxy=`tail conf.gro -n 1 | awk '{print $2}'`
boxz=`tail conf.gro -n 1 | awk '{print $3}'`
## warm run
rm -f warmup
cp -a tools/atom.template ./warmup
cd warmup
mv ../conf.gro .
sed "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top
grompp &> $mylog
mdrun -v &> $mylog
rm -f ../conf.gro
if test ! -f confout.gro; then
    echo "no file confout.gro, exit!"
    exit
fi
cp confout.gro ../conf.gro
cd ..
## add COM site
make -C tools/gen.conf/ clean &> $makelog
make -j4 -C tools/gen.conf/ &> $makelog
tools/gen.conf/stupid.add.com -f conf.gro -o out.gro &> $mylog
mv -f out.gro conf.gro

# prepare dens.SOL.xvg
echo "0 0 0" > dens.SOL.xvg
tmp=`echo "$boxx/3.0" | bc -l`
echo "$tmp 0 0" >> dens.SOL.xvg
tmp=`echo "$boxx/3.0 * 2.0" | bc -l`
echo "$tmp 0 0" >> dens.SOL.xvg
echo "$boxx 0 0" >> dens.SOL.xvg

# copy dir
rm -f tf
cp -a tools/tf.template ./tf

# prepare grompp.mdp
rm -f grompp.mdp
cp tf/grompp.mdp .
half_boxx=`echo "$boxx/2.0" | bc -l`
half_boxy=`echo "$boxy/2.0" | bc -l`
half_boxz=`echo "$boxz/2.0" | bc -l`
sed -e "/^adress_ex_width/s/=.*/= $ex_region_r/g" grompp.mdp |\
sed -e "/^adress_hy_width/s/=.*/= $hy_region_r/g" |\
sed -e "/^adress_reference_coords/s/=.*/= $half_boxx $half_boxy $half_boxz/g" > grompp.mdp.tmp
mv -f grompp.mdp.tmp grompp.mdp

# prepare index file
echo "a COM" > command.tmp
echo "name 3 CG" >> command.tmp
echo "q" >> command.tmp
cat command.tmp  | make_ndx -f conf.gro
rm -f command.tmp

# prepare settings.xml
rm -f settings.xml
cp tf/settings.xml .
tf_min=`echo "$ex_region_r - $tf_extension" | bc -l`
tf_max=`echo "$ex_region_r + $hy_region_r + $tf_extension" | bc -l`
tf_spline_start=`echo "$ex_region_r - $tf_spline_extension" | bc -l`
tf_spline_end=`  echo "$ex_region_r + $hy_region_r + $tf_spline_extension" | bc -l`
half_box_1=`echo "$half_box + 1." | bc -l`
sed -e "s/<min>.*<\/min>/<min>$tf_min<\/min>/g" settings.xml |\
sed -e "s/<max>.*<\/max>/<max>$tf_max<\/max>/g" |\
sed -e "s/<step>.*<\/step>/<step>$tf_step<\/step>/g" |\
sed -e "s/<spline_start>.*<\/spline_start>/<spline_start>$tf_spline_start<\/spline_start>/g" |\
sed -e "s/<spline_end>.*<\/spline_end>/<spline_end>$tf_spline_end<\/spline_end>/g" |\
sed -e "s/<spline_step>.*<\/spline_step>/<spline_step>$tf_spline_step<\/spline_step>/g" |\
sed -e "s/<table_end>.*<\/table_end>/<table_end>$half_boxx_1<\/table_end>/g" |\
sed -e "s/<iterations_max>.*<\/iterations_max>/<iterations_max>$tf_iterations_max<\/iterations_max>/g" > settings.xml.tmp

# prepre topol.top
rm -f topol.top
cp tf/topol.top .
sed "s/SOL.*/SOL $nmol/g" topol.top > tmp.top
mv -f tmp.top topol.top

# copy all file to tf
rm -f  tf/conf.gro tf/dens.SOL.xvg tf/grompp.mdp tf/index.ndx tf/settings.xml tf/topol.top
cp conf.gro dens.SOL.xvg grompp.mdp index.ndx settings.xml topol.top tf/

