#!/bin/bash

function set_parameter () {
    file=$1
    seed=`date +%s`
    sed -e "/^nsteps /s/=.*/= $nsteps/g" $file |\
    sed -e "/^dt /s/=.*/= $dt/g" > tmp
    mv -f tmp $file
}


