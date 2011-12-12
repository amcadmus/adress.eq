#!/bin/bash

function clean_tf () {
    total_num_step=`ls | grep step_ | wc -l`
    count=1
    for dirName in `ls | grep step_`;
    do
        if test $count -eq $total_num_step; then
            break
        fi
        echo "# clean $dirName"
        cd $dirName
        if test -f traj.xtc; then
            echo 2 | g_density -b 10 -d X -xvg none -nice 0 &> g_density.log
            rm -f traj.xtc
        fi
        cd ..
        count=$(($count+1))
    done
    echo "# clean $dirName"
    cd $dirName
    if test -f traj.xtc; then
        echo 2 | g_density -b 10 -d X -xvg none -nice 0 &> g_density.log
        echo 2 2 | g_rdf -b 10 -rdf mol_com -xvg none -nice 0 -bin 0.01 -dt 1 &> g_rdf.log
    fi
    cd ..
}


clean_tf

# rm  step_0[0-5]*/traj.xtc -f
