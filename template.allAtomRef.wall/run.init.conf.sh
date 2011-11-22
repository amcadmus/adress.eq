#!/bin/bash

source functions.sh
source parameters.sh

dt=$dt_init
nsteps=$nsteps_init

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

echo "# set parameters        ==================================================================" >> $runtime_log
set_parameter grompp.mdp
echo "# end set parameters    ==================================================================" >> $runtime_log
echo "# grompp                ==================================================================" >> $runtime_log
grompp &>> $runtime_log
echo "# end grompp            ==================================================================" >> $runtime_log
echo "# mdrun                 ==================================================================" >> $runtime_log
echo running on `cat /etc/hostname ` pid is $$ > pid.log
mdrun -v &>> $runtime_log
echo "# end mdrun             ==================================================================" >> $runtime_log
echo "# post process          ==================================================================" >> $runtime_log
post_process $output_dir
echo "# end post process      ==================================================================" >> $runtime_log
