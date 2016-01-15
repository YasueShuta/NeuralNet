set terminal wxt title "Figure" font 'Times New Roman,12' size 640,330
set title 'Noise - E_RMS' font 'Segoe UI,18'
set xlabel 'Noise Rate [%]' font 'Segoe UI,14'
set ylabel 'Error RMS' font 'Segoe UI,14'
set yrange [0:]
set key right bottom

plot '../../src/tmp/C01N10.dat' using 1:2 with lines lw 2 title 'middle_10',\
     '../../src/tmp/C01N20.dat' using 1:2 with lines lw 2 title 'middle_20',\
     '../../src/tmp/C01N30.dat' using 1:2 with lines lw 2 title 'middle_30',\
     '../../src/tmp/C01N50.dat' using 1:2 with lines lw 2 title 'middle_50'

set terminal 'png'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-mn-e.png'
replot

set terminal 'emf'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-mn-e.emf'
replot

set terminal wxt
set output