#!/bin/bash

source functions.sh

# parameters
dt=0.002
nsteps=100

# input & output
input_dir=out.grad.temp
output_dir=out.gravity
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
make -C tools/gen.conf/ -j4 >> $make_log

if test ! -d $input_dir ; then
    echo "no file $input_dir, exit"
    exit;
fi

if test -f $init_conf; then
    if test -f $ref_conf; then
	echo "# both init_conf and ref_conf exist, generate conf.gro from them =========================" >> $runtime_log
	./tools/gen.conf/repair.wall -f $init_conf -r $ref_conf -o conf.gro
	if test $? -ne 0; then
	    echo "# command ./tools/gen.conf/repair.wall failed, exit. ====================================="  >> $runtime_log
	fi
	echo "# end gen conf ===========================================================================" >> $runtime_log
    else
	echo "# init_conf exists and ref_conf does not exist, use init_conf as conf.gro ================" >> $runtime_log
	rm -f $init_conf.tmptmptmp
	cp $init_conf $init_conf.tmptmptmp
	mv -f $init_conf.tmptmptmp ./conf.gro
	echo "# end gen conf ===========================================================================" >> $runtime_log
    fi
    if test ! -f $top; then
	echo "# no topology file, exit =================================================================" >> $runtime_log
	exit;
    fi
    rm -f $top.tmptmptmp
    cp $top $top.tmptmptmp
    mv -f $top.tmptmptmp ./topol.top
else
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
fi

grocleanit
rm -f *.itp grompp.mdp
cp tools/script.gravity/*itp .
cp tools/script.gravity/grompp.mdp .
cp conf.gro conf.start.gro

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
