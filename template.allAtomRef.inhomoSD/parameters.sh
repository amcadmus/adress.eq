#!/bin/bash

dt_init=0.001
nsteps_init=10000

dt_grad_temp=0.002
nsteps_grad_temp=500000

dt_gravity=0.002
nsteps_gravity=500000
accelerate=-0.1

# md parameters
nstxout=1000
nstvout=1000
nstenergy=100
nstxtcout=0

# thermostat settings
T1=200
T2=600
thick=2.0
tau_t=0.5
