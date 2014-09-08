set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'fig-rho.eps'

set style line 10 lc 0 lw 3 pt 7
set style line 20 lt 0 lc 0 lw 3 pt 7
set style line 1  lc 1 lw 3 pt 7
set style line 11 lc 1 lw 3 pt 7 lt 0
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7
set style line 5 lc 4 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7

# set label 'AT' at -.1, 1040
# set label '{/Symbol D}' at 1.775, 1040
# set label 'CG' at 3.37, 1040

set label 'AT' at -0.1, 1033
set label 'HY I' at 0.75, 1033
# set label '{/Symbol D}' at 1.775, 1033
set label 'HY II + III' at 1.95, 1033
set label 'CG' at 3.34, 1033


set xrange [-.5:3.75]
set yrange [920:1060]
set mxtics 5
set mytics 2
set xlabel 'x [nm]'
set ylabel '{/Symbol r}(x) [ kg/m^3 ]'
set key bottom left

pl  995.77 t'EX' ls 1,\
'grid3.dat' w l ls 20 not,\
'run.04_02_02.allCorr/rho.out' u ($1-3.7500):2:3 w e ls 2 not , '' u ($1-3.7500):2 w l ls 2 t 'std. AdResS',\
'rerun.water.wca/rho.out' u ($1-3.7500):2:3 w e ls 3 not , '' u ($1-3.7500):2 w l ls 3 t 'WCA AdResS'

# 'rerun.04_02_02.step.004.tf/rho.out' u ($1-3.7500):2:3 w e ls 4 t 'step 4', '' u ($1-3.7500):2 w l ls 4 not

set out 'fig-rho-onlystd.eps'
pl  995.77 t'All-atom water' ls 1,\
'grid3.dat' w l ls 20 not,\
'run.04_02_02.allCorr/rho.out' u ($1-3.7500):2:3 w e ls 2 not , '' u ($1-3.7500):2 w l ls 2 t 'AdResS'

