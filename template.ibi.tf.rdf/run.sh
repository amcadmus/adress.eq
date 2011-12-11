#!/bin/bash

source template.ibi/env.sh
source parameters.sh
source functions.sh

uname -n > host

old_ibi_iterations_max=$ibi_iterations_max
old_tf_iterations_max=$tf_iterations_max
ibi_iterations_max=`printf "%03d" $ibi_iterations_max`
tf_iterations_max=`printf "%03d" $tf_iterations_max`
ibi_resultDir=step_$ibi_iterations_max
tf_resultDir=step_$tf_iterations_max

for i in `seq 1 $rdf_iterations`
do
    thisNum=`printf "%03d" $i`
    ibiDir=step.${thisNum}.ibi
    tfDir=step.${thisNum}.tf
    if test -d $ibiDir; then
	echo "# dir $ibiDir exists, quit!"
	mv -f $ibiDir $ibiDir.`date +%s`
    fi
    if test -d $tfDir; then
	echo "# dir $tfDir exists, quit!"
	mv -f $tfDir $tfDir.`date +%s`
    fi

    cp -a template.ibi $ibiDir
    cd $ibiDir
    ibi_setting
    if test $i -eq 1; then
	./run.sh
    else
	rm -f tabletf.xvg tablerdf.xvg
	cp ../$last_tfDir/tabletf.xvg .
#	cp ../$last_ibiDir/$ibi_resultDir/tablerdf.xvg .
	cp ../$last_ibiDir/CG-CG.pot.new ./CG-CG.pot.in
	./run.sh
    fi
    ibi_last_step=`ls | grep step_ | tail -n 1`
    cp $ibi_last_step/CG-CG.pot.new .
    csg_call --ia-type C12 --options settings.xml convert_potential gromacs CG-CG.pot.new tablerdf.xvg &>> inverse.log
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
	cp ../$last_tfDir/SOL.pot.new ./SOL.pot.in
	./run.sh
    fi
    tf_last_step=`ls | grep step_ | tail -n 1`
    cp $tf_last_step/SOL.pot.new .
    csg_call --ia-type bonded --options settings.xml convert_potential gromacs SOL.pot.new tabletf.xvg &>> inverse.log
    clean_tf
    cd ..

    last_tfDir=$tfDir
    last_ibiDir=$ibiDir
done


