set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'rho.eps'

set style line 10 lc 0 lw 3 pt 7
set style line 20 lt 0 lc 0 lw 3 pt 7
set style line 1  lc 1 lw 3 pt 7
set style line 11 lc 1 lw 3 pt 7 lt 0
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7
set style line 5 lc 4 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7

set label 'AT' at 0.17, 1040
set label '{/Symbol D}' at 1.775, 1040
set label 'CG' at 3.37, 1040

set xrange [0:3.75]
set yrange [920:1060]
set mxtics 5
set mytics 2
set xlabel 'x [nm]'
set ylabel '{/Symbol r}(x) [ kg/m^3 ]'
set key bottom right

pl  995.77 t'EX' ls 5,\
'grid3.dat' w l ls 20 not,\
'rerun.04_02_02.step.001.ibi/rho.out' u ($1-3.7500):2:3 w e ls 1 not 'step 1', '' u ($1-3.7500):2 w l ls 11 t 'step 1, after IBI before TFI',\
'rerun.04_02_02.step.001.tf/rho.out' u ($1-3.7500):2:3 w e ls 1 not 'step 1', '' u ($1-3.7500):2 w l ls 1 t 'step 1',\
'rerun.04_02_02.step.002.tf/rho.out' u ($1-3.7500):2:3 w e ls 2 not 'step 2', '' u ($1-3.7500):2 w l ls 2 t 'step 2',\
'rerun.04_02_02.step.004.tf/rho.out' u ($1-3.7500):2:3 w e ls 3 not 'step 4', '' u ($1-3.7500):2 w l ls 3 t 'step 4'

# 'rerun.04_02_02.step.004.tf/rho.out' u ($1-3.7500):2:3 w e ls 4 t 'step 4', '' u ($1-3.7500):2 w l ls 4 not


