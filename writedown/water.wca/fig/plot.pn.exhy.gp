set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'fig-pn-exhy.eps'

set style line 1 lc 1 lw 3 pt 2
set style line 2 lc 2 lw 3 pt 2
set style line 3 lc 3 lw 3 pt 2

set xrange [.97:1.03]
set yrange [0:0.06]
set mxtics 2
set mytics 2
set xlabel 'N_1/<N_1>'
set ylabel 'p(N_1/<N_1>)'

# pl\
# 'atom.16.02.02/number.distrib.out' u ($1/455.541):2 every 2 w lp ls 1 t 'EX',\
# 'rerun.16.02.02/number.distrib.out' u ($1/457.823):2 every 2 w lp ls 3 t 'WCA AdResS'

# pl\
# 'atom.08.02.02/number.distrib.out' u ($1/455.773):2 every 2 w lp ls 1 t 'EX',\
# 'rerun.08.02.02/number.distrib.out' u ($1/457.33):2 every 2 w lp ls 3 t 'WCA AdResS'
# # 'rerun.08.02.02/gaussiannumber.distrib.out' u ($1/457.33):2 w l ls 2

pl\
'atom.04.02.02/number.distrib.out' u ($1/1275.61):2 every 2 w lp ls 1 t 'EX',\
'rerun.long/number.distrib.out' u ($1/1277.69):2 every 2 w lp ls 3 t 'WCA AdResS'
