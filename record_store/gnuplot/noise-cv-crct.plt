set terminal wxt title "Figure" font 'Times New Roman,12' size 640,330
set title 'Noise - Correct' font 'Segoe UI,18'
set xlabel 'Noise Rate [%]' font 'Segoe UI,14'
set ylabel 'Percent of Correct [%]' font 'Segoe UI,14'
set yrange [0:]
set key right bottom

plot '../../src/tmp/C01N15.dat' using 1:3 with lines lw 2 title 'conv_0.001',\
     '../../src/tmp/C05N15.dat' using 1:3 with lines lw 2 title 'conv_0.005',\
     '../../src/tmp/C10N15.dat' using 1:3 with lines lw 2 title 'conv_0.010',\
     '../../src/tmp/C15N15.dat' using 1:3 with lines lw 2 title 'conv_0.015',\
     '../../src/tmp/C20N15.dat' using 1:3 with lines lw 2 title 'conv_0.020'

set terminal 'png'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-cv-c.png'
replot

set terminal 'emf'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-cv-c.emf'
replot

set terminal wxt
set output