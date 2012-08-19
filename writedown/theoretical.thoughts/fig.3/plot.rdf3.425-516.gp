set term post eps enh color solid size 17cm,11cm
set out 'fig-rdf3.425-516.eps'

#set border 4095 front linetype -1 linewidth 1.000
set view map
set samples 50, 50
set isosamples 50, 50
unset surface
set style data pm3d
set style function pm3d
set ticslevel 0
set xlabel "q [nm]" 
set ylabel "p [nm/ps]"
# set xtics 0.1
# set ytics 1.0
set pm3d implicit at b
#set size ratio 0.88
set palette rgbformulae 22,13,-31
set size ratio 1.0

set yrange [0:0.8]
set xrange [-1.1:1.7]
# set mxtics 2
# set yrange [-6:6]

set cbrange [0.8:1.2]
set format cb "%.2f"

#set size 1.4,1.0
set origin 0, 0.04

set multiplot 

# set ratio 1
set size 0.24,0.50

set origin 0.0,0.6
unset xlabel
set title 't = 0 ps'
# set title 'linear resp., t=00000 ps' font "tt,16"
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.425-516.00.270.out' u 1:2:($3) every 2 not

set origin 0.0, 0.3
unset title
#set title 'direct, t=00000 ps' font "tt,16"
spl 'run.04_02_02.allCorr/rdf3.inter.9//rdf3.425-516.00.270.out' u 1:2:($3) every 2 not

set origin 0.0, -0.0
unset title
set xlabel "q [nm]" 
#set title 'direct, t=00000 ps' font "tt,16"
spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.425-516.00.270.out' u 1:2:($3) every 2 not

unset ylabel


set origin 0.25, 0.6
unset xlabel
set title 't = 10 ps'
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.425-516.00.330.out' u 1:2:($3) every 2 not

set origin 0.5, 0.6
set title 't = 15 ps'
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.425-516.00.460.out' u 1:2:($3) every 2 not

set origin 0.75, 0.6
set title 't = 20 ps'
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.425-516.00.800.out' u 1:2:($3) every 2 not


unset title

set origin 0.25, 0.3
spl 'run.04_02_02.allCorr/rdf3.inter.9//rdf3.425-516.00.330.out' u 1:2:($3) every 2 not

set origin 0.5, 0.3
spl 'run.04_02_02.allCorr/rdf3.inter.9//rdf3.425-516.00.460.out' u 1:2:($3) every 2 not

set origin 0.75, 0.3
spl 'run.04_02_02.allCorr/rdf3.inter.9//rdf3.425-516.00.800.out' u 1:2:($3) every 2 not


set xlabel "q [nm]"

set origin 0.25, 0.0
spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.425-516.00.330.out' u 1:2:($3) every 2 not

set origin 0.5, 0.0
spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.425-516.00.460.out' u 1:2:($3) every 2 not

set origin 0.75, 0.0
spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.425-516.00.800.out' u 1:2:($3) every 2 not


unset multiplot
