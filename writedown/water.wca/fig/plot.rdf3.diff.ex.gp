set term post eps enh color solid size 7.4cm,10cm font 20
set out 'fig-rdf3-diff-ex.eps'

#set border 4095 front linetype -1 linewidth 1.000
set view map
set samples 50, 50
set isosamples 50, 50
unset surface
set style data pm3d
set style function pm3d
set ticslevel 0
set xlabel "h_1 [nm]" 
set ylabel "h_2 [nm]"
# set xtics 0.1
# set ytics 1.0
set pm3d implicit at b
#set size ratio 0.88
set palette rgbformulae 22,13,-31
#set size ratio 0.5

set yrange [0:1.0]
set yrange [0.01:1.0]
set ytics .2
set mytics 2
set xrange [-1.1:1.7]
# set mxtics 2
# set yrange [-6:6]

# set cbrange [0.8:1.2]
set cbrange [-.1:.1]
set cbtics .05
set format cb "%.2f"

#set size 1.4,1.0
set origin -0.00, -0.0

set multiplot 

# set ratio 1
set size 0.98,0.4

set origin 0.01,0.66
unset xlabel
set ylabel 'r_{12} = 0.27 nm'
# set ylabel 'AT'
spl 'rerun.water.wca/rdf3.inter.9//diff.rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

set origin 0.01, 0.33
unset title
# set ylabel 'AdResS AT'
set ylabel 'r_{12} = 0.33 nm'
spl 'rerun.water.wca/rdf3.inter.9//diff.rdf3.375-425.00.330.out' u 1:2:($3) every 4 not

set origin 0.01, 0.00
unset title
# set ylabel 'HY I'
set ylabel 'r_{12} = 0.80 nm'
spl 'rerun.water.wca/rdf3.inter.9//diff.rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


unset multiplot
