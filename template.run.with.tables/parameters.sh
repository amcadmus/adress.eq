#!/bin/bash

base_conf=tools/gen.conf/spc216.gro
n_base_block="4 2 2"
number_density=33.286      # 0.99577 g/cm^3

table_dir=04_02_02

ex_region_r=0.5

gmx_nsteps=500000
gmx_nstenergy=200
gmx_nstxtcout=200
