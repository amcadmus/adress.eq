#!/bin/bash

target=$1

if test ! -d $target; then
    echo no dir $target
    exit
fi

cwd=`pwd`
cd $target

for i in `ls | grep rdf | grep oh`;
do
    echo $i
    $cwd/rdfCorr/corr $i
done

for i in `ls | grep rdf | grep hh`;
do
    echo $i
    $cwd/rdfCorr/corr $i
done

cd $cwd
    
