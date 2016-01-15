set term wxt "Times New Roman, 12"

set title "Noise - E_RMS" font "Segoe UI, 18"
set xlabel "Noise Rate [%]" font "Segoe UI, 14"
set ylabel "Error RMS" font "Segoe UI, 14"

plot "tmp/C01N05.dat" using 1:2 with lines lw 3 notitle

load "save.plt"