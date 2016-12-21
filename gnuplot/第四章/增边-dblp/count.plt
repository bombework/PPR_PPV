set terminal svg lw 2 font "Times New Roman,18"
set output "plot/count.svg"

set xlabel "增加边数 [K]" font "Times New Roman,18"
set ylabel "迭代次数" font "Times New Roman,18"

#set xtics ("1000" 250000,"2000" 500000,"4000" 1000000,"8000" 2000000)
#set logscale x

set key right bottom


plot [:][:] \
"result.txt" using ($1)/1000:2 title "原始数据" with linespoints linecolor 0 linewidth 1 pointtype 11 pointsize 0.8,\
"result.txt" using ($1)/1000:4 title "增量计算" with linespoints linecolor 2 linewidth 1 pointtype 12 pointsize 0.8,\
"result.txt" using ($1)/1000:6 title "直接计算" with linespoints linecolor 7 linewidth 1 pointtype 13 pointsize 0.8