#!/bin/bash

if test $# -ne 2; then
    echo "usage:"
    echo "./rdf2.diff.sh dir0 dir1"
    exit
fi

make -C ./tools/rdfdiff/ -j4

cwd=`pwd`
dir0=$1
dir1=$2

if test ! -d $1; then
    echo no dir $1, exit
    exit
fi
if test ! -d $2; then
    echo no dir $2, exit
    exit
fi

cd $dir0
targets=`ls | grep rdf | grep out | grep -v diff | grep -v alongx`
cd $cwd

for i in $targets
do
    j=`echo $i | sed -e 's/[0-9][0-9][0-9]-[0-9][0-9][0-9]/375-425/g'`
    echo "# diff $dir0/$i $dir1/$j"
    ./tools/rdfdiff/rdf2diff --input-0 $dir0/$i --input-1 $dir1/$j --output $dir0/diff.$i
done
