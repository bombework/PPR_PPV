set terminal svg lw 2 font "Times New Roman,18"
set output "plot/topk.svg"

set xlabel "topk" font "Times New Roman,18"
set ylabel "交集个数" font "Times New Roman,18"

set xtics ("top-100" 0,"top-500" 1,"top-1000" 2,"top-5000" 3,"top-10000" 4)
#set logscale x

set key left bottom
#set mxtics 0.2
#set size square 1

plot [-1:5][0.85:1.05] \
"topk.txt" using 1 title "1000" with linespoints linecolor 0 linewidth 1 pointtype 11 pointsize 0.8,\
"topk.txt" using 2 title "5000" with linespoints linecolor 1 linewidth 1 pointtype 12 pointsize 0.8,\
"topk.txt" using 3 title "10000" with linespoints linecolor 2 linewidth 1 pointtype 13 pointsize 0.8,\
"topk.txt" using 4 title "15000" with linespoints linecolor 3 linewidth 1 pointtype 14 pointsize 0.8,\
"topk.txt" using 5 title "20000" with linespoints linecolor 4 linewidth 1 pointtype 15 pointsize 0.8,\