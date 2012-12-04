set term post eps enh color solid size 17cm,9cm font 15
set out 'fig-rdf3-diff.eps'

#set border 4095 front linetype -1 linewidth 1.000
set view map
set samples 50, 25
set isosamples 50, 25
unset surface
set style data pm3d
set style function pm3d
set ticslevel 0
# set xlabel "h_1 [nm]" 
# set ylabel "h_2 [nm]"
# set xtics 0.1
# set ytics 1.0
set pm3d implicit at b
#set size ratio 0.88
set palette rgbformulae 22,13,-31
#set size ratio 0.5

set yrange [0.01:1.0]
# set yrange [0.00:1.0]
set ytics .2
set mytics 2
set xrange [-1.1:1.7]
# set mxtics 2
# set yrange [-6:6]

# set cbrange [-1:1]
set cbrange [-.1:.1]
set cbtics .05
set format cb "%.1f"

#set size 1.4,1.0
set origin -0.00, -0.0

set multiplot 

# set ratio 1
# set size 0.33,0.24

set origin -0.0,0.6
unset xlabel
set title 'r_{12} = 0.27 nm'
set cbrange [-1:1]
set cbtics .5
set ylabel 'EX C^{(3)}'
set size 0.33,0.40
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.270.out' u 1:2:($3) every 2 not
set size 0.33,0.37

set origin -0.0, 0.3
unset title
set cbrange [-.1:.1]
set cbtics .05
set format cb "%.2f"
set ylabel '{/Symbol D}C^{(3)} in AT'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

set origin -0.0, 0.0
unset title
set ylabel '{/Symbol D}C^{(3)} in HY I'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.270.out' u 1:2:($3) every 4 not


# set origin 0.0, -0.0
# unset title
# #set title 'direct, t=00000 ps' font "tt,16"
# spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

unset ylabel

set origin 0.33, 0.6
unset xlabel
set title 'r_{12} = 0.33 nm'
set cbrange [-1:1]
set cbtics .5
set size 0.33,0.40
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.330.out' u 1:2:($3) every 2 not
set size 0.33,0.37

set origin 0.66, 0.6
set title 'r_{12} = 0.80 nm'
set size 0.33,0.40
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.800.out' u 1:2:($3) every 2 not
set size 0.33,0.37

# set origin 0.75, 0.6
# set title 'r_{12} = 0.80 nm'
# spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


unset title

set cbrange [-.1:.1]
set cbtics .05
set format cb "%.2f"
set origin 0.33, 0.3
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.3
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


set origin 0.33, 0.0
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.0
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.800.out' u 1:2:($3) every 4 not


unset multiplot
