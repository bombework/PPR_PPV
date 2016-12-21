set terminal svg lw 2 font "Times New Roman,18"
set output "accurcy-incr.svg"

set xlabel "增加边数 [K]" font "Times New Roman,18"
set ylabel "精度" font "Times New Roman,18"

#set xtics ("1000" 250000,"2000" 500000,"4000" 1000000,"8000" 2000000)
#set logscale x

set key right bottom


plot [:][0.85:1.05] \
"accuracy.txt" using ($1)/1000:6 title "DBLP" with linespoints linecolor 7 linewidth 1 pointtype 11 pointsize 0.8,\
"accuracy.txt" using ($1)/1000:7 title "LJ" with linespoints linecolor 8 linewidth 1 pointtype 12 pointsize 0.8,\
"accuracy.txt" using ($1)/1000:8 title "orkut" with linespoints linecolor 9 linewidth 1 pointtype 13 pointsize 0.8,\
"accuracy.txt" using ($1)/1000:9 title "Youtube" with linespoints linecolor 10 linewidth 1 pointtype 14 pointsize 0.8,\