set term post eps enh solid  font 16 size 8.5cm,6cm

set style line 10 lc 0 lw 3 pt 7
set style line 20 lt 0 lc 0 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 4 lw 3 pt 7
set style line 5 lc 5 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7

set out 'fig-rdf-375-425-1.eps'

unset xtics
unset ytics
unset xtics
unset ytics 
set xrange[.1:.8]
set yrange[0:3.5]
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
set key top right

# set arrow from .37, 2.8 to .31, 2.8 lw 1
# set arrow from .40, 1.3 to .36, 1.1 lw 1

pl 1 ls 20 not,\
'rdf.com.xvg' w l ls 1 t 'EX',\
'run.04_02_02.allCorr/rdf.inter.9/rdf.375-425.out' w l ls 2 t 'std. AdResS',\
'rerun.water.wca/rdf.inter.9/rdf.375-425.out' w l ls 3 t 'WCA AdResS'

