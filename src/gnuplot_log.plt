set terminal wxt title "Figure" font 'Times New Roman,18.000000' size 640,330
set title 'Noise - E_RMS' font 'Segoe UI,18'
set xlabel 'Noise Rate [%]' font 'Segoe UI,18'
set ylabel 'Error RMS' font 'Segoe UI,18'
set yrange [0:]
plot 'tmp/C01N10.dat' using 1:2 with lines lw 3 title 'middle_10',\
'tmp/C01N20.dat' using 1:2 with lines lw 3 title 'middle_20',\
'tmp/C01N30.dat' using 1:2 with lines lw 3 title 'middle_30',\
'tmp/C01N50.dat' using 1:2 with lines lw 3 title 'middle_50'
set terminal 'png'  font 'Times New Roman,18.000000' size 640,330
set output 'rec.png'
replot
set terminal wxt
set output
set terminal 'emf'  font 'Times New Roman,18.000000' size 640,330
set output 'rec.emf'
replot
set terminal wxt
set output
