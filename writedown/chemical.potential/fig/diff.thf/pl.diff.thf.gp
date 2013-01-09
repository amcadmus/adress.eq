set term post eps enh solid color font 16 size 8.5cm,6cm
set out 'diff-thf.eps'

set xrange [0.5:3.5]
set mxtics 5
set yrange [-5:50]
set mytics 5
set xlabel 'x [nm]'
set ylabel 'diff. of th. force [ kJ/(mol nm) ]'

set style line 1 lc 2 lw 3 pt 1
set style line 2 lc 1 lw 3 pt 1
set style line 3 lc 3 lw 3 pt 2


pl \
   './table.hd.xvg' u 1:($3-$5) w l ls 1 t '{/Symbol D} F_{th}', \
   './diff.d.dh.out' u 1:2 w p ls 3 t '< F_{rep} >_D',\
   './diff.h.hd.out' u 1:2 w p ls 2 t '< F_{rep} >_H'
