#!/bin/bash

source env.sh
source parameters.sh

function set_param () {
    targetfile=$1
    sed -e "s,base_conf=.*,base_conf=$base_conf,g" $targetfile |
    sed -e "s/n_base_block=.*/n_base_block=\"$n_base_block\"/g" |
    sed -e "s/number_density=.*/number_density=$number_density/g" |
    sed -e "s/ex_region_r=.*/ex_region_r=$ex_region_r/g" |
    sed -e "s/hy_region_r=.*/hy_region_r=$hy_region_r/g" |
    sed -e "s/tf_extension=.*/tf_extension=$tf_extension/g" |
    sed -e "s/tf_step=.*/tf_step=$tf_step/g" |
    sed -e "s/tf_spline_extension=.*/tf_spline_extension=$tf_spline_extension/g" |
    sed -e "s/tf_spline_step=.*/tf_spline_step=$tf_spline_step/g" |
    sed -e "s/tf_iterations_max=.*/tf_iterations_max=$tf_iterations_init/g" > tmp.param
    mv -f tmp.param $targetfile
}

if test -d step.000.tf; then
    echo "dir step.000.tf exits, backup"
    mv step.000.tf step.000.tf.`date +%s`
fi
rm -f step.000.tf

cp -a template.tf.init step.000.tf
rm -f step.000.tf/env.sh
cp env.sh step.000.tf

cd step.000.tf
set_param parameters.sh
./gen.tf.sh
cd ..

