set term post eps enh color font 20 size 8.5cm,7cm
set out 'fig-tpi.eps'

set style line 11 lc 1 lw 3 pt 2 lt 1
set style line 12 lc 1 lw 3 pt 2 lt 2
set style line 21 lc 2 lw 3 pt 2 lt 1
set style line 31 lc 3 lw 3 pt 2 lt 1

set ylabel "{/Symbol m} [kJ/mol]"

set multiplot

# set bmargin 3
# set tmargin 0
# set lmargin 12
# set rmargin 2

set size 1,.58
set origin 0,0.0
set xrange [-0.1:8]
set yrange [-30:-5]
set xlabel 't [ns]'
set mxtics 2
set ytics 10
set mytics 5
pl \
-24.6 ls 12 not,\
'comp.atom.pme.tpi/tpi.xvg.bk2' u ($1/1000):2 w l ls 11 t'EX water'
unset xlabel

set size 1,.48
set origin 0,0.5
set format x ""
set ytics 5
set yrange [15:25]
set key right bottom
pl\
'comp.wca.tpi/tpi.xvg' u ($1/1000):2 w l ls 31 t 'WCA',\
'comp.cg.tpi/tpi.xvg' u ($1/1000):2 w l ls 21 t 'CG water'

unset multiplot
