set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'force-rdf.eps'

set multiplot

set origin 0,0
set size 1,1
set xrange [.1:.8]
set mxtics 5
set yrange [-16:20]
set mytics 5
set xlabel 'r [nm]'
set ylabel 'U^{rdf}(r) [ kJ/mol ]'

set style line 10 lc 0 lt 0 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7
set style line 5 lc 4 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7
set style line 7 lc 7 lw 3 pt 7

set key right bottom

pl 0 w l ls 10 not,\
'rerun.04_02_02.step.001.tf/tablerdf.xvg' u 1:($6*4) w l ls 1 t 'step 1',\
'rerun.04_02_02.step.002.tf/tablerdf.xvg' u 1:($6*4) w l ls 2 t 'step 2',\
'rerun.04_02_02.step.004.tf/tablerdf.xvg' u 1:($6*4) w l ls 3 t 'step 4'
#'rerun.04_02_02.step.003.tf/tablerdf.xvg' u 1:6 w l ls 3 t 'step 3',\
# 'step.010.tf/tablerdf.xvg' u 1:6 w l ls 5 t 'step 10',\
# 'step.025.tf/tablerdf.xvg' u 1:6 w l ls 6 t 'step 25'

set origin 0.45,0.55
set size 0.5,0.35

unset xlabel
unset ylabel
unset xrange
unset yrange
#set xlabel "step of interation"
#set ylabel "P_{EX}V - P_{CG}V [ KJ/mol ]" font "Helvetica,10"
set ylabel 'W_{/Symbol D}  [ kJ/mol ]' font "Helvetica,12"
set xrange [-.3:4.3]
set yrange [13:16]
set xtics 1
set mxtics 1
#unset ytics
set ytics 1
set mytics 2

pl \
1.4472600000e+01 lw 3 lt 1 not,\
'tf.diff.out' u 0:2 w p pt 4 lc 1 not
unset multiplot
