set terminal wxt title "Figure" font 'Times New Roman,12' size 640,330
set title 'Noise - Correct' font 'Segoe UI,18'
set xlabel 'Noise Rate [%]' font 'Segoe UI,14'
set ylabel 'Percent of Correct [%]' font 'Segoe UI,14'
set yrange [0:]
set key right bottom

plot '../../src/tmp/C01N15.dat' using 1:3 with lines lw 2 title 'conv_0.001',\
     '../../src/tmp/C02N15.dat' using 1:3 with lines lw 2 title 'conv_0.002',\
     '../../src/tmp/C03N15.dat' using 1:3 with lines lw 2 title 'conv_0.003',\
     '../../src/tmp/C04N15.dat' using 1:3 with lines lw 2 title 'conv_0.004',\
     '../../src/tmp/C05N15.dat' using 1:3 with lines lw 2 title 'conv_0.005',\
     '../../src/tmp/C06N15.dat' using 1:3 with lines lw 2 title 'conv_0.006',\
     '../../src/tmp/C07N15.dat' using 1:3 with lines lw 2 title 'conv_0.007',\
     '../../src/tmp/C08N15.dat' using 1:3 with lines lw 2 title 'conv_0.008',\
     '../../src/tmp/C09N15.dat' using 1:3 with lines lw 2 title 'conv_0.009',\
     '../../src/tmp/C10N15.dat' using 1:3 with lines lw 2 title 'conv_0.010',\
     '../../src/tmp/C15N15.dat' using 1:3 with lines lw 2 title 'conv_0.015',\
     '../../src/tmp/C20N15.dat' using 1:3 with lines lw 2 title 'conv_0.020',\
     '../../src/tmp/C25N15.dat' using 1:3 with lines lw 2 title 'conv_0.025',\
     '../../src/tmp/C30N15.dat' using 1:3 with lines lw 2 title 'conv_0.030'

set terminal 'png'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-cv-c.png'
replot

set terminal 'emf'  font 'Times New Roman,12' size 640,330
set output 'ass2_noise-cv-c.emf'
replot

set terminal wxt
set output