#!/bin/bash

base_conf=tools/gen.conf/spc216.gro
n_base_block="12 2 2"
number_density=33.286      # 0.99577 g/cm^3

# gen.tf.sh parameters
ex_region_r=1.0
hy_region_r=2.0
tf_extension=0.2
tf_step=0.01
tf_spline_extension=0.6
tf_spline_step=0.4
tf_iterations_max=30


# run.tf.sh parameters
#run_dir=./run
tf_file=tabletf.xvg
dt=0.002
nsteps=500000
nstxout=0
nstvout=0
nstenergy=100
nstxtcout=1000

