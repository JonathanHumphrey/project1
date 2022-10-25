#!bin/bash
rm sim.data

for((i = 10; i < 31; i++)); do
    ./sim 1 $i 0.04 0.01
    cp sim.data ./data/1-$i-004.data
done

for((i = 10; i < 31; i++)); do
    ./sim 2 $i 0.04 0.01
    cp sim.data ./data/2-$i-004.data
done