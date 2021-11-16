#!/bin/bash
program="oddEvenSort"
gcc -O0 -fopenmp ${program}.c -o ${program}
current_time=$(date "+%Y.%m.%d-%H.%M.%S")
echo "Current Time : $current_time"
fileName="results_"${program}_${current_time}
touch ${fileName}.csv
powers=($(seq 10 20))
echo "10,11,12,13,14,15,16,17" >> ${fileName}.csv
for threadNum in 0 1 2 4 8 16
do
    i=0
    for power in 10 11 12 13 14 15 16 17
    do
        length=$((2**${power}))
        output=$(./${program} ${length} ${threadNum})
        echo ${output}
        time[i]=${output##*= }
        #echo ${time[i]}
        i=$((i+1))
    done
    echo "${time[0]},${time[1]},${time[2]},${time[3]},${time[4]},${time[5]},${time[6]},${time[7]}" >> ${fileName}.csv
done