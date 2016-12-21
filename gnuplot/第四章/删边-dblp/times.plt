set terminal svg lw 2 font "Times New Roman,18"
set output "plot/times.svg"

set xlabel "删除边数 [K]" font "Times New Roman,18"
set ylabel "运行时间 [ms]" font "Times New Roman,18"


#set logscale x

set key right bottom


plot [:][:] \
"result.txt" using ($1)/1000:3 title "原始数据" with linespoints linecolor 0 linewidth 1 pointtype 11 pointsize 0.8,\
"result.txt" using ($1)/1000:5 title "增量计算" with linespoints linecolor 2 linewidth 1 pointtype 12 pointsize 0.8,\
"result.txt" using ($1)/1000:7 title "直接计算" with linespoints linecolor 7 linewidth 1 pointtype 13 pointsize 0.8