set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'tf.eps'

set style line 10 lc 9 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7

set xrange [0:3.7500]
set xlabel 'x'
set mxtics 5
set ylabel 'F^{th} [ kJ/(mol nm) ]'
set mytics 5
set key top right
pl \
   'rerun.04_02_02.step.000.tf/tabletf.xvg' u 1:3 w l ls 10 t 'inital guess',\
   'rerun.04_02_02.step.001.tf/tabletf.xvg' u 1:3 w l ls 1  t 'step 1',\
   'rerun.04_02_02.step.002.tf/tabletf.xvg' u 1:3 w l ls 2  t 'step 2',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:3 w l ls 3  t 'step 3',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:3 w l ls 4  t 'step 4'
   
set out 'inte.tf.eps'
set ylabel 'integral F^{th} [ kJ/mol ]'
set mytics 5
pl \
   'rerun.04_02_02.step.000.tf/tabletf.xvg' u 1:2 w l ls 10 t 'inital guess',\
   'rerun.04_02_02.step.001.tf/tabletf.xvg' u 1:2 w l ls 1  t 'step 1',\
   'rerun.04_02_02.step.002.tf/tabletf.xvg' u 1:2 w l ls 2  t 'step 2',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:2 w l ls 3  t 'step 3',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:2 w l ls 4  t 'step 4'
