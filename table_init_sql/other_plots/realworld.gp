set size ratio 0.4
set terminal postscript color enhanced
set output 'realworld.ps'
unset title
set tmargin -3
set bmargin -2
set rmargin 0
set lmargin 12
set border 3 front linetype -1 linewidth 1.000
set boxwidth 0.95 absolute
set style fill solid 1.00 noborder
set linetype 1 lw 1 lc rgb "#222222"
set linetype 6 lw 1 lc rgb "#FF0000"
set linetype 2 lw 1 lc rgb "#FFDD11"
set linetype 5 lw 1 lc rgb "#0000FF"
set linetype 4 lw 1 lc rgb "#ffa500"
set linetype 3 lw 1 lc rgb "#55AAAA"
set linetype cycle 4
set grid nopolar
set grid noxtics nomxtics ytics nomytics noztics nomztics nox2tics nomx2tics noy2tics nomy2tics nocbtics nomcbtics
set grid layerdefault linetype 0 linewidth 3.000,  linetype 0 linewidth 1.000
set key nobox autotitles columnhead Left reverse left
set key font "Arial,26"
set key width 1
set key samplen 1
set key spacing 1
set key maxrows 2
set key at -0.5, 10
set style histogram clustered gap 1 title  offset character 2, -0.25, 1
set datafile missing '-'
set style data histograms
set xtics border in scale 0,0 nomirror   offset character 0.5, -0.5, 2 autojustify
set xtics norangelimit font ",24"
set xtics   ()
set xrange [ -0.5 : 3]
set ylabel "Runtime(s)"
set ylabel font "Arial,30"
set ylabel offset character -3, 0, 0
set yrange [ 0 : 10 ]
set ytics border in scale 0,0 mirror norotate  offset character 0, 0, 0 autojustify
set ytics font ",34"
set xlabel font "Arial,34"
set xlabel "Real world query"
set xlabel  offset character 0, -1, 0  norotate
plot 'realworld.csv' using 2 t col, '' using 3:xtic(1) t col, '' using 4 t col, '' using 5 t col