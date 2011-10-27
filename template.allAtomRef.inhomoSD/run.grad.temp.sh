#!/bin/bash

source env.sh
source functions.sh
source parameters.sh

# parameters
dt=$dt_grad_temp
nsteps=$nsteps_grad_temp

# input & output
input_dir=out.init
output_dir=out.grad.temp
# configs
init_conf=$input_dir/confout.gro
ref_conf=$input_dir/conf.gro
top=$input_dir/topol.top

# options generating initial conf
nbox=4
scale=0.932

runtime_log=run.log
make_log=make.log

rm -f $runtime_log $make_log

if test ! -d $input_dir ; then
    echo "no file $input_dir, exit"
    exit;
fi

if test -f $init_conf; then
    echo "# use $init_conf as conf.gro =============================================================" >> $runtime_log
    rm -f $init_conf.tmptmptmp
    cp $init_conf $init_conf.tmptmptmp
    mv -f $init_conf.tmptmptmp ./conf.gro
    echo "# end gen conf ===========================================================================" >> $runtime_log
    if test ! -f $top; then
	echo "# no topology file, exit =================================================================" >> $runtime_log
	exit;
    fi
    rm -f $top.tmptmptmp
    cp $top $top.tmptmptmp
    mv -f $top.tmptmptmp ./topol.top
else
    echo no init config, exit
    exit
fi

grocleanit
rm -f *.itp grompp.mdp
cp tools/script.grad.temp/*itp .
cp tools/script.grad.temp/grompp.mdp .
rm -f conf.start.gro
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
