#!/bin/bash

base_conf=tools/gen.conf/spc216.gro
n_base_block="12 2 2"
number_density=33.286      # 0.99577 g/cm^3

ex_region_r=1.0
hy_region_r=2.0

# md parameters
dt=0.002
nsteps=500000
nstxout=1000
nstvout=1000
nstenergy=100
nstxtcout=0

