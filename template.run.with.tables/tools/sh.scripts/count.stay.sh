#!/bin/bash

./tools/analyze.number/analyze.traj --x0 3.25 --x1 4.25 --yz0 0 --yz1 100 -b 4000 -e 5000 --time-of-stay 30 -m adress | grep -v \#
