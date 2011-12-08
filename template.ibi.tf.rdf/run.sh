#!/bin/bash

source parameters.sh
source functions.sh

ibi_iteration_max=`printf "%03d" $ibi_iteration_max`
tf_iteration_max=`printf "%03d" $tf_iteration_max`
ibi_resultDir=step_$ibi_iteration_max
tf_resultDir=step_$tf_iteration_max

for i in `seq 1 $num_iter`
do
    thisNum=`printf "%03d" $i`
    ibiDir=step.${thisNum}.00.ibi
    tfDir=step.${thisNum}.01.tf

    cp -a template.ibi $ibiDir
    cd $ibiDir
    ibi_setting
    if test $i -eq 1; then
	./run.sh
    else
	rm -f tabletf.xvg tablerdf.xvg
	cp ../$last_tfDir/$tf_resultDir/tabletf.xvg .
	cp ../$last_ibiDir/$ibi_resultDir/tablerdf.xvg .
	cp ../$last_ibiDir/$ibi_resultDir/CG-CG.pot.new ./CG-CG.pot.in
	./run.sh
    fi
    cd ..

    cp -a template.tf $tfDir
    cd $tfDir
    tf_setting
    rm -f tabletf.xvg tablerdf.xvg
    cp ../$ibiDir/$ibi_resultDir/tablerdf.xvg .
    if test $i -eq 1; then
	./run.sh
    else
	cp ../$last_tfDir/$tf_resultDir/tabletf.xvg .
	cp ../$last_tfDir/$tf_resultDir/SOL.pot.new ./SOL.pot.in
	./run.sh
    fi
    cd ..
done