set terminal svg lw 2 font "Times New Roman,18"
set output "plot/accurcy.svg"

set xlabel "删除边数 [K]" font "Times New Roman,18"
set ylabel "精度" font "Times New Roman,18"

#set xtics ("1000" 250000,"2000" 500000,"4000" 1000000,"8000" 2000000)
#set logscale x

set key left top


plot [:][0.99:1.005] \
"result.txt" using ($1)/1000:8 title "精度" with linespoints linecolor 7 linewidth 1 pointtype 11 pointsize 0.8