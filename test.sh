#!/bin/bash

SUB_STR="time = "
SUFFIX=" (sec)"
OUTPUT_FILE="test_output.txt"
n_input=$1
iteration_input=$2
file_input=$3
threshold_input=$4
use_threading_input=$4 # 0 = no threading, 1 = threading

# default n_input to 2
if [ -z "$n_input" ]; then
 n_input="6"
fi
 
# default iteration_input to 50
if [ -z "$iteration_input" ]; then
 iteration_input="10000"
fi

# default file_input to iris_test_data.data
if [ -z "$file_input" ]; then
 file_input=./med_data.dat
fi

# default threshold_input to 2
if [ -z "$threshold_input" ]; then
 threshold_input="2"
fi

if [ "$use_threading_input" == "1" ]; then
 iter_loop=(1 2 3 4 5 6 7 8)
else
 iter_loop=(1)
fi

for thread_cnt in "${iter_loop[@]}"
 do
  export OMP_NUM_THREADS="$thread_cnt"
  echo "$OMP_NUM_THREADS threads for NMF N: $n_input, Iteration: $iteration_input, T: $threshold_input, file: $file_input" | tee -a $OUTPUT_FILE
  for iter in 1 2 3 4 5 6 7 # loop through 7 different iterations to get a good average of timing.
   do
    ret=$(./nmf -N $n_input -I $iteration_input -T $threshold_input $file_input)
    stripped_time=`echo $ret | sed -e "s/.*${SUB_STR}//;s/${SUFFIX}.*//"`
    echo -n "${stripped_time}, " | tee -a $OUTPUT_FILE
   done
   echo "" | tee -a $OUTPUT_FILE
 done