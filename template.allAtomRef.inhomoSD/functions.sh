#!/bin/bash

function set_parameter () {
    file=$1
    seed=`date +%s`
    sed -e "/^nsteps /s/=.*/= $nsteps/g" $file |\
    sed -e "/^nstxout /s/=.*/= $nstxout/g" |\
    sed -e "/^nstvout /s/=.*/= $nstvout/g" |\
    sed -e "/^nstenergy /s/=.*/= $nstenergy/g" |\
    sed -e "/^nstxtcout /s/=.*/= $nstxtcout/g" |\
    sed -e "/^userreal1 /s/=.*/= $T1/g" |\
    sed -e "/^userreal2 /s/=.*/= $T2/g" |\
    sed -e "/^userreal3 /s/=.*/= $thick/g" |\
    sed -e "/^userreal4 /s/=.*/= $tau_t/g" |\
    sed -e "/^accelerate /s/=.*/= 0.0 0.0 $accelerate/g" |\
    sed -e "/^dt /s/=.*/= $dt/g" > tmp
    mv -f tmp $file
}

function post_process () {
    output_dir=$1
    rm -fr $output_dir
    mkdir  $output_dir
    cp conf.gro		$output_dir/
    cp confout.gro	$output_dir/
    cp traj.trr		$output_dir/
    cp topol.top	$output_dir/
    cp grompp.mdp	$output_dir/
    cp *itp		$output_dir/
    cp *log		$output_dir/
}

