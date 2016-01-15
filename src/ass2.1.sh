#!/bin/bash

for i in 3 4
do
./NeuralNet_main -t 10000 -c 0.001 -n 15 -r 0.5
cp ../record/rec_main.txt tmp/C01N15R50-${i}.txt
mkdir ../record_store/ass2.1-C01N15-${i}
mv ../record/* ../record_store/ass2.1-C01N15-${i}
done

for i in {0..4}
do
./NeuralNet_main -t 10000 -c 0.015 -n 15 -r 0.5
cp ../record/rec_main.txt tmp/C15N15R50-${i}.txt
mkdir ../record_store/ass2.1-C15N15-${i}
mv ../record/* ../record_store/ass2.1-C15N15-${i}
done

