set term post eps enh solid  font 16 size 8.5cm,6cm
set out 'tf-and-int.eps'

set style line 10 lc 0 lw 3 pt 7
set style line 20 lt 0 lc 0 lw 3 pt 7
set style line 1 lc 1 lw 3 pt 7
set style line 2 lc 2 lw 3 pt 7
set style line 3 lc 3 lw 3 pt 7
set style line 4 lc 5 lw 3 pt 7
set style line 5 lc 4 lw 3 pt 7

set multiplot
set size 1, 0.53
set origin 0.0,0.0
 
set xrange [0:3.7500]
set xlabel 'x [nm]'
set mxtics 5
set key top right
set tmargin 0
set ylabel 'int F^{th} [ kJ/mol ]'
set yrange [-5:20]
set ytics ("-5" -5, "0" 0, "5" 5, "10" 10, "15" 15)

pl 0 ls 20 not,\
   'rerun.04_02_02.step.000.tf/tabletf.xvg' u 1:2 w l ls 10 t 'only TFI',\
   'rerun.04_02_02.step.001.tf/tabletf.xvg' u 1:2 w l ls 1  t 'step 1',\
   'rerun.04_02_02.step.002.tf/tabletf.xvg' u 1:2 w l ls 2  t 'step 2',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:2 w l ls 3  t 'step 3',\
   'grid1.dat' w l ls 20 not
   # 'step.010.tf/tabletf.xvg' u 1:2 w l ls 4  t 'step 10',\
   # 'step.024.tf/tabletf.xvg' u 1:2 w l ls 5  t 'step 25',\

set size 1, 0.47
set tmargin 1
set origin 0.0,0.53
unset xlabel
set format x ""
set bmargin 0
set yrange [-5:30]
set ylabel 'F^{th} [ kJ/(mol nm) ]'
set ytics ("-5" -5,\
"0" 0, "5" 5, "10" 10, "15" 15, "20" 20, "25" 25, "30" 30)

# set ytics 5
set mytics 5

set label 'AT' at 0.17, 25
set label '{/Symbol D}' at 1.775, 25
set label 'CG' at 3.37, 25

pl 0 ls 20 not,\
   'rerun.04_02_02.step.000.tf/tabletf.xvg' u 1:3 w l ls 10 not 'inital guess',\
   'rerun.04_02_02.step.001.tf/tabletf.xvg' u 1:3 w l ls 1  not 'step 1',\
   'rerun.04_02_02.step.002.tf/tabletf.xvg' u 1:3 w l ls 2  not 'step 2',\
   'rerun.04_02_02.step.003.tf/tabletf.xvg' u 1:3 w l ls 3  not 'step 3',\
   'grid2.dat' w l ls 20 not
   # 'step.010.tf/tabletf.xvg' u 1:3 w l ls 4  not 'step 10',\
   # 'step.024.tf/tabletf.xvg' u 1:3 w l ls 5  not 'step 25',\
