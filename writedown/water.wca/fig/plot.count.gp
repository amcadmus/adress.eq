set term post eps enh solid  font 16 size 8.5cm,6cm

set style line 10 lc 0 lw 3 pt 7
set style line 20 lt 0 lc 0 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 4 lw 3 pt 7
set style line 5 lc 5 lw 3 pt 7
set style line 6 lc 6 lw 3 pt 7

set out 'fig-count.eps'

# set style line 11 lc 1 lw 3 pt 5
# set style line 12 lc 2 lw 3 pt 5
# set style line 13 lc 3 lw 3 pt 5
# set style line 14 lc 4 lw 3 pt 5
# set style line 15 lc 5 lw 3 pt 5
# set style line 16 lc 6 lw 3 pt 5
# set style line 17 lc 7 lw 3 pt 5
# set style line 18 lc 8 lw 3 pt 5

set yrange [.12:.17]
set format y '%.2f'
set xrange [-.5:3.75]
set xlabel 'x [nm]'
set ylabel '(<N^2> - <N>^2)/<N>'
set key bottom left
set mxtics 5
set mytics 5

set label 'AT' at -0.1, .16
set label 'HY I' at 0.75, .16
# set label '{/Symbol D}' at 1.775, .16
set label 'HY II + III' at 1.95, .16
set label 'CG' at 3.34, .16

pl\
'grid4.dat' w l ls 20 not,\
'./run.04_02_02.atom.long/count.out'		u ($1-3.75):6:($7*2) w e ls 1 not, '' u ($1-3.75):6 w l ls 1 t'EX',\
'./run.04_02_02.allCorr.m50.long.step15/count.out' u ($1-3.75):6:($7*2) w e ls 2 not, '' u ($1-3.75):6 w l ls 2 t'std. AdResS',\
'./rerun.water.wca/count.out'			u ($1-3.75):6:($7*2) w e ls 3 not, '' u ($1-3.75):6 w l ls 3 t'WCA AdResS'

# './run.04_02_02.allCorr.m50.long.step15/count.out' u ($1-3.75):6:($7*2) w e ls 5 not, '' u ($1-3.75):6 w l ls 5 t'TFI-IBI new w',\
#'./run.04_02_02.allCorr.m50.long.step04/count.out' u ($1-3.75):6:($7*2) w e ls 5 not, '' u ($1-3.75):6 w l ls 5 t'TFI-IBI new w'

#0.150062 lw 3 lc 0 not,\
# './run.04_02_02.allCorr.step10.long/count.out'	u 1:6:($7*2) w e ls 16 not, '' u 1:6 w l ls 16 t'10',\
# './run.04_02_02.cg.long/count.out'		u 1:6:($7*2) w e ls 12 not, '' u 1:6 w l ls 12,\
# './run.04_02_02.noibi.long/count.out'		u 1:6:($7*2) w e ls 14 not, '' u 1:6 w l ls 14,\
# './run.04_02_02.allCorr.step4.long/count.out'   u 1:6:($7*2) w e ls 15 not, '' u 1:6 w l ls 15 t'4',\
# './run.04_02_02.allCorr.m10.long.step08/count.out'	u 1:6:($7*2) w e ls 17 not, '' u 1:6 w l ls 17 t'm10',\
