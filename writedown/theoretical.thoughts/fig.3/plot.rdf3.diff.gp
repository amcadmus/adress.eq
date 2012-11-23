set term post eps enh color solid size 24cm,18cm font 20
set out 'fig-rdf3-diff.eps'

#set border 4095 front linetype -1 linewidth 1.000
set view map
set samples 50, 50
set isosamples 50, 50
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

# set cbrange [0.8:1.2]
set cbrange [-.1:.1]
set cbtics .1
set format cb "%.1f"

#set size 1.4,1.0
set origin -0.00, -0.0

set multiplot 

# set ratio 1
set size 0.33,0.24

set origin -0.0,0.76
unset xlabel
set title 'r_{12} = 0.27 nm'
set cbrange [0.8:1.2]
set cbtics .1
set ylabel 'EX g^3'
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

set origin -0.0, 0.57
unset title
set cbrange [-.1:.1]
set cbtics .05
set format cb "%.2f"
set ylabel '{/Symbol D}g^3 in AT'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

set origin -0.0, 0.38
unset title
set ylabel '{/Symbol D}g^3 in HY I'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.270.out' u 1:2:($3) every 4 not

set origin -0.0, 0.19
unset title
set ylabel '{/Symbol D}g^3 in HY II'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.516-608.00.270.out' u 1:2:($3) every 4 not

set origin -0.0, 0.0
unset title
# set xlabel "h_1 [nm]" 
set ylabel '{/Symbol D}g^3 in HY III'
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.608-700.00.270.out' u 1:2:($3) every 4 not


# set origin 0.0, -0.0
# unset title
# #set title 'direct, t=00000 ps' font "tt,16"
# spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.375-425.00.270.out' u 1:2:($3) every 4 not

unset ylabel

set origin 0.33, 0.76
unset xlabel
set title 'r_{12} = 0.33 nm'
set cbrange [0.8:1.2]
set cbtics .1
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.76
set title 'r_{12} = 0.80 nm'
spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.800.out' u 1:2:($3) every 4 not

# set origin 0.75, 0.6
# set title 'r_{12} = 0.80 nm'
# spl 'run.04_02_02.atom/rdf3.inter.9//rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


unset title

set cbrange [-.1:.1]
set cbtics .05
set format cb "%.2f"
set origin 0.33, 0.57
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.57
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


set origin 0.33, 0.38
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.38
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.425-516.00.800.out' u 1:2:($3) every 4 not


set origin 0.33, 0.19
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.516-608.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.19
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.516-608.00.800.out' u 1:2:($3) every 4 not


# set xlabel "h_1 [nm]"
set origin 0.33, 0.0
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.608-700.00.330.out' u 1:2:($3) every 4 not

set origin 0.66, 0.0
spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.608-700.00.800.out' u 1:2:($3) every 4 not


# set origin 0.75, 0.3
# spl 'run.04_02_02.allCorr/rdf3.inter.9//diff.rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


# set xlabel "h_1 [nm]"

# set origin 0.25, 0.0
# spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.375-425.00.330.out' u 1:2:($3) every 4 not

# set origin 0.5, 0.0
# spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.375-425.00.460.out' u 1:2:($3) every 4 not

# set origin 0.75, 0.0
# spl 'run.04_02_02.old.noibi/rdf3.inter.9//rdf3.375-425.00.800.out' u 1:2:($3) every 4 not


unset multiplot
