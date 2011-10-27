#!/bin/bash

source env.sh
source functions.sh
source parameters.sh

dt=$dt_init
nsteps=$nsteps_init

nbox=4

output_dir=out.init

runtime_log=run.log
make_log=make.log

rm -f $runtime_log $make_log

rm -f spc216.gro; cp tools/script.init/spc216.gro .
echo "# genconf by spc216     ==================================================================" >> $runtime_log
genconf -nbox $nbox $nbox $nbox -f spc216.gro -o conf.gro &>> $runtime_log
echo "# end genconf by spc216 ==================================================================" >> $runtime_log

grocleanit
rm -f *.itp grompp.mdp
cp tools/script.init/*itp .
cp tools/script.init/grompp.mdp .
echo "#include \"system.itp\"" > topol.top
echo "[ molecules ]" >> topol.top
nsol=`grep OW conf.gro | wc -l`
echo "SOL $nsol" >> topol.top

echo "# set parameters        ==================================================================" >> $runtime_log
set_parameter grompp.mdp
echo "# end set parameters    ==================================================================" >> $runtime_log
echo "# grompp                ==================================================================" >> $runtime_log
grompp &>> $runtime_log
echo "# end grompp            ==================================================================" >> $runtime_log
echo "# mdrun                 ==================================================================" >> $runtime_log
mdrun -v &>> $runtime_log
echo "# end mdrun             ==================================================================" >> $runtime_log
echo "# post process          ==================================================================" >> $runtime_log
post_process $output_dir
echo "# end post process      ==================================================================" >> $runtime_log
