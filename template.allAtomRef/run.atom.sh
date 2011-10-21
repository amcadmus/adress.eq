#!/bin/bash

nbox=8
scale=0.92

runtime_log=run.log
make_log=make.log

rm -f $runtime_log $make_log

make -C tools/gen.conf/ -j4 >> $make_log
cp tools/gen.conf/spc216.gro .

echo "# genconf by spc216     =================================================================="
genconf -nbox $nbox $nbox $nbox -f spc216.gro -o conf.gro &>> $runtime_log
echo "# end genconf by spc216 =================================================================="
echo "# add wall              =================================================================="
tools/gen.conf/bcc.wall -f conf.gro -o out.gro &>> $runtime_log
echo "# end add wall          =================================================================="
echo "# rescale box           =================================================================="
editconf -scale $scale $scale $scale -f out.gro -o conf.gro &>> $runtime_log
echo "# end rescale box       =================================================================="

cp tools/script.atom/*itp .
cp tools/script.atom/grompp.gro .

echo "# grompp                =================================================================="
grompp &>> $runtime_log
echo "# end grompp            =================================================================="
echo "# mdrun                 =================================================================="
mdrun &>> $runtime_log
echo "# end mdrun             =================================================================="
