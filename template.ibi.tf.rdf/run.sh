#!/bin/bash

source parameters.sh
source functions.sh

ibi_iterations_max=`printf "%03d" $ibi_iterations_max`
tf_iterations_max=`printf "%03d" $tf_iterations_max`
ibi_resultDir=step_$ibi_iterations_max
tf_resultDir=step_$tf_iterations_max

for i in `seq 1 $num_iter`
do
    thisNum=`printf "%03d" $i`
    ibiDir=step.${thisNum}.ibi
    tfDir=step.${thisNum}.tf

    cp -a template.ibi $ibiDir
    cd $ibiDir
    ibi_setting
    if test $i -eq 1; then
	./run.sh
    else
	rm -f tabletf.xvg tablerdf.xvg
	cp ../$last_tfDir/tabletf.xvg .
#	cp ../$last_ibiDir/$ibi_resultDir/tablerdf.xvg .
	cp ../$last_ibiDir/$ibi_resultDir/CG-CG.pot.new ./CG-CG.pot.in
	./run.sh
    fi
    csg_call --ia-type C12 --options settings.xml convert_potential gromacs $ibi_resultDir/CG-CG.pot.new tablerdf.xvg &>> inverse.log
    clean_ibi
    cd ..

    cp -a template.tf $tfDir
    cd $tfDir
    tf_setting
    rm -f tabletf.xvg tablerdf.xvg
    cp ../$ibiDir/tablerdf.xvg .
    if test $i -eq 1; then
	cp ../$ibiDir/SOL.pot.new ./SOL.pot.in
	./run.sh
    else
#	cp ../$last_tfDir/$tf_resultDir/tabletf.xvg .
	cp ../$last_tfDir/$tf_resultDir/SOL.pot.new ./SOL.pot.in
	./run.sh
    fi
    csg_call --ia-type bonded --options settings.xml convert_potential gromacs $tf_resultDir/SOL.pot.new tabletf.xvg &>> inverse.log
    clean_tf
    cd ..

    last_tfDir=$tfDir
    last_ibiDir=$ibiDir
done


