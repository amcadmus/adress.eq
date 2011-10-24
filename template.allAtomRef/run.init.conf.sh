#!/bin/bash

nbox=4
scale=0.932

output_dir=out.init

runtime_log=run.log
make_log=make.log

rm -f $runtime_log $make_log

make -C tools/gen.conf/ -j4 >> $make_log
rm -f spc216.gro; cp tools/gen.conf/spc216.gro .

echo "# genconf by spc216     ==================================================================" >> $runtime_log
genconf -nbox $nbox $nbox $nbox -f spc216.gro -o conf.gro &>> $runtime_log
echo "# end genconf by spc216 ==================================================================" >> $runtime_log
echo "# add wall              ==================================================================" >> $runtime_log
tools/gen.conf/bcc.wall -f conf.gro -o out.gro &>> $runtime_log
echo "# end add wall          ==================================================================" >> $runtime_log
echo "# rescale box           ==================================================================" >> $runtime_log
editconf -scale $scale $scale $scale -f out.gro -o conf.gro &>> $runtime_log
echo "# end rescale box       ==================================================================" >> $runtime_log

grocleanit
rm -f *.itp grompp.mdp
cp tools/script.atom/*itp .
cp tools/script.atom/grompp.mdp .
cp conf.gro conf.start.gro

echo "# grompp                ==================================================================" >> $runtime_log
grompp &>> $runtime_log
echo "# end grompp            ==================================================================" >> $runtime_log
echo "# mdrun                 ==================================================================" >> $runtime_log
mdrun -v &>> $runtime_log
echo "# end mdrun             ==================================================================" >> $runtime_log

test ! -d $output_dir && mkdir $output_dir
rm -f $output_dir/*
cp conf.gro	$output_dir/
cp confout.gro	$output_dir/
cp topol.top	$output_dir/
cp grompp.mdp   $output_dir/
cp *itp		$output_dir/

