set term post eps enh font 22 size 8.5cm,8.5cm

set style line 10 lc 0 lw 4 pt 7
set style line 20 lt 0 lc 0 lw 4 pt 7
set style line 1 lc 1 lw 4 pt 7 lt 1
set style line 2 lc 2 lw 4 pt 7 lt 1
set style line 3 lc 3 lw 4 pt 7 lt 1
set style line 4 lc 4 lw 4 pt 7 lt 1
set style line 11 lc 1 lw 4 pt 7 lt 3
set style line 21 lc 2 lw 4 pt 7 lt 3
set style line 31 lc 3 lw 4 pt 7 lt 3
set style line 41 lc 4 lw 4 pt 7 lt 3
set style line 5 lc 5 lw 4 pt 7
set style line 6 lc 6 lw 4 pt 7

set out 'fig-rdf-hhoh-700-750.eps'

set clip two

set multiplot


set xtics font "Helvetica,10"
set ytics font "Helvetica,10"

# unset xlabel
# unset ylabel
# set clip two
# set size .4,0.22500
# set origin .40,0.49500
# set xrange [.26:.30]
# set yrange [2.5:3.1]
# set mxtics 4
# set mytics 2
# set xtics .02
# set ytics .2

# pl\
# 'run.04_02_02.atom/rdf.inter.9/rdf.hh.425-516.out' w l ls 1 not 'AT',\
# 'run.04_02_02.old.noibi/rdf.inter.9/rdf.hh.425-516.out' w l ls 2 not 'TFI old w',\
# 'rerun.04_02_02.step.000.tf/rdf.inter.9/rdf.hh.425-516.out' w l ls 3 not 'TFI new w',\
# 'run.04_02_02.allCorr/rdf.inter.9/rdf.hh.425-516.out' w l ls 4 not 'TFI-IBI new w'

# set size .4,0.22500
# set origin .4,.3
# set xrange [.28:.42]
# set yrange [.75:1.]
# set xtics .05
# set ytics .1
# set mxtics 5
# set mytics 2

# rep

set size 1., 0.75
set origin 0., 0.

unset xtics
unset ytics
unset xtics
unset ytics 
set xrange[.1:.8]
set yrange[0:2]
set xtics .1
set mxtics 5
set ytics .5
set mytics 5
set xtics nomirror
set xlabel 'r [nm]'
# set ylabel 'g(r)'
set tmargin 0
set lmargin 4.
set rmargin 2.
#unset ytics ("3.5" 3.5)
#set key "0.5,2"
set key bottom right

# set arrow from .37, 2.8 to .31, 2.8 lw 1
# set arrow from .40, 1.3 to .36, 1.1 lw 1

pl 1 ls 20 not,\
'run.04_02_02.atom/rdf.inter.9/rdf.hh.700-750.out' w l ls 1 t 'H-H AT',\
'run.04_02_02.atom/rdf.inter.9/rdf.oh.700-750.out' w l ls 11 t 'O-H AT',\
'run.04_02_02.allCorr/rdf.inter.9/rdf.hh.700-750.out' w l ls 2 t 'H-H AdResS',\
'run.04_02_02.allCorr/rdf.inter.9/rdf.oh.700-750.out' w l ls 21 t 'O-H AdResS',\
'rerun.water.wca/rdf.inter.9/rdf.hh.700-750.out' w l ls 3 t 'H-H WCA',\
'rerun.water.wca/rdf.inter.9/rdf.oh.700-750.out' w l ls 31 t 'O-H WCA'
#'plot.rdf.box.1' w l ls 10 not

# 'run.04_02_02.old.noibi/rdf.inter.9/rdf.hh.700-750.out' w l ls 2 t 'TFI old w',\
# 'run.04_02_02.old.noibi/rdf.inter.9/rdf.oh.700-750.out' w l ls 21 t 'TFI old w',\
# 'rerun.04_02_02.step.000.tf/rdf.inter.9/rdf.hh.700-750.out' w l ls 3 t 'TFI new w',\
# 'rerun.04_02_02.step.000.tf/rdf.inter.9/rdf.oh.700-750.out' w l ls 31 t 'TFI new w',\


unset arrow

set bmargin 0
set xrange [0:3.75]
set xtics .5
set mxtics 5
unset xlabel
# set xlabel "x"
# set format x ""
set ytics 1
set mytics 2
# set ylabel "w(x)"
set yrange [-.5:1.2]
set size 1., 0.20
set origin 0., 0.75
#set style fill pattern 8
set style fill transparent pattern 5
# set style fill pattern 0
set key top right
set label "CG" at 3.4,-.2

pl\
'print.w/grid.out' w l ls 20 not,\
'print.w/CG.out' w filledcurve lc 0 not,\
'print.w/new.w.out' w l ls 3 t'w(x)'

# 'print.w/old.w.out' w l ls 2 t'old w',\

unset multiplot





