set term post eps enh solid  font 15 size 8.5cm,6cm
set out 'rdf.eps'

set style line 10 lc 0 lw 3 pt 7
set style line 20 lc 0 lt 0 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7
set style line 5 lc 4 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7
set style line 7 lc 7 lw 3 pt 7


set multiplot

set xtics font "Helvetica,10"
set ytics font "Helvetica,10"

unset xlabel
unset ylabel
set clip two
set size .4,.3
set origin .40,.66
set xrange [.26:.30]
set yrange [2.5:3.1]
set mxtics 4
set mytics 2
set xtics .02
set ytics .2

pl\
'rdf.com.xvg' u 1:2 w l ls 5 not 'AT' ,\
'rerun.04_02_02.step.000.tf//rdf.xvg' u 1:2 w l ls 10 not 'only TFI',\
'rerun.04_02_02.step.001.tf//rdf.xvg' u 1:2 w l ls 1 not 'step 1',\
'rerun.04_02_02.step.002.tf//rdf.xvg' u 1:2 w l ls 2 not 'step 2',\
'rerun.04_02_02.step.003.tf//rdf.xvg' u 1:2 w l ls 3 not 'step 3'

set size .4,.3
set origin .4,.40
set xrange [.28:.42]
set yrange [.75:1.]
set xtics .05
set ytics .1
set mxtics 5
set mytics 2

rep

set size 1,1
set origin 0,0
set xrange [.1:.8]
set xtics .1
set mxtics 5
set yrange [0:3.3]
set ytics .5
set mytics 5
set xlabel 'r [nm]'
set ylabel 'g(r)'

set key right bottom
set xtics font "Helvetica,15"
set ytics font "Helvetica,15"

#set arrow from .39, .5 to .37, .5
set arrow from .37, 2.8 to .31, 2.8 lw 1
set arrow from .40, 1.3 to .36, 1.1 lw 1

pl  1 ls 20 not,\
'rdf.com.xvg' u 1:2 w l ls 5 t 'AT' ,\
'rerun.04_02_02.step.000.tf//rdf.xvg' u 1:2 w l ls 10 t 'only TF',\
'rerun.04_02_02.step.001.tf//rdf.xvg' u 1:2 w l ls 1 t 'step 1',\
'rerun.04_02_02.step.002.tf//rdf.xvg' u 1:2 w l ls 2 t 'step 2',\
'rerun.04_02_02.step.004.tf//rdf.xvg' u 1:2 w l ls 3 t 'step 4',\
'plot.rdf.box.1' w l ls 10 not
# 'step.010.tf/tablerdf.xvg' u 1:6 w l ls 5 t 'step 10',\
# 'step.025.tf/tablerdf.xvg' u 1:6 w l ls 6 t 'step 25'



unset multiplot