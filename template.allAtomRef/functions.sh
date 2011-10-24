#!/bin/bash

function set_parameter () {
    file=$1
    seed=`date +%s`
    sed -e "/^nsteps /s/=.*/= $nsteps/g" $file |\
    sed -e "/^dt /s/=.*/= $dt/g" > tmp
    mv -f tmp $file
}

function post_process () {
    output_dir=$1
    rm -fr $output_dir
    mkdir  $output_dir
    cp conf.gro		$output_dir/
    cp confout.gro	$output_dir/
    cp topol.top	$output_dir/
    cp grompp.mdp	$output_dir/
    cp *itp		$output_dir/
    cp *log		$output_dir/
}

