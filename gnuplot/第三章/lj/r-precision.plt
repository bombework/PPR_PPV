set terminal svg lw 2 font "Times New Roman,18"
set output "r-precision.svg"

set xlabel "采样次数" font "Times New Roman,18"
set ylabel "精度" font "Times New Roman,18"

#set xtics ("1000" 250000,"2000" 500000,"4000" 1000000,"8000" 2000000)
set logscale x

set key right top


plot [9000:210000][0:1] \
"r.txt" using 1:5 title "Top-K扩容算法" with linespoints linecolor 7 linewidth 1 pointtype 11 pointsize 0.8,\
"r.txt" using 1:8 title "蒙特卡罗算法" with linespoints linecolor 0 linewidth 1 pointtype 12 pointsize 0.8