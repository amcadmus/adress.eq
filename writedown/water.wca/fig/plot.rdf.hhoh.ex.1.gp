set term post eps enh font 16 size 8.5cm,6cm

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

set out 'fig-rdf-hhoh-375-425-1.eps'

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
# set xtics nomirror
set xlabel 'r [nm]'
set ylabel 'g(r)'
# set tmargin 0
# set lmargin 4.
# set rmargin 2.
#unset ytics ("3.5" 3.5)
#set key "0.5,2"
set key bottom right


pl 1 ls 20 not,\
'run.04_02_02.atom/rdf.inter.9/rdf.hh.375-425.out' w l ls 1 t 'H-H EX',\
'run.04_02_02.atom/rdf.inter.9/rdf.oh.375-425.out' w l ls 11 t 'O-H EX',\
'run.04_02_02.allCorr/rdf.inter.9/rdf.hh.375-425.out' w l ls 2 t 'H-H std. AdResS',\
'run.04_02_02.allCorr/rdf.inter.9/rdf.oh.375-425.out' w l ls 21 t 'O-H std. AdResS',\
'rerun.water.wca/rdf.inter.9/rdf.hh.375-425.out' w l ls 3 t 'H-H WCA AdResS',\
'rerun.water.wca/rdf.inter.9/rdf.oh.375-425.out' w l ls 31 t 'O-H WCA AdResS'
#'plot.rdf.box.1' w l ls 10 not

# 'run.04_02_02.old.noibi/rdf.inter.9/rdf.hh.375-425.out' w l ls 2 t 'TFI old w',\
# 'run.04_02_02.old.noibi/rdf.inter.9/rdf.oh.375-425.out' w l ls 21 t 'TFI old w',\
# 'rerun.04_02_02.step.000.tf/rdf.inter.9/rdf.hh.375-425.out' w l ls 3 t 'TFI new w',\
# 'rerun.04_02_02.step.000.tf/rdf.inter.9/rdf.oh.375-425.out' w l ls 31 t 'TFI new w',\





