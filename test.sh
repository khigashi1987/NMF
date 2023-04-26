#!/bin/bash

SUB_STR="time = "
SUFFIX=" (sec)"
OUTPUT_FILE="test_output.txt"
n_input=$1
iteration_input=$2
file_input=$3
threshold_input=$4
use_threading_input=$5 # 0 = no threading, 1 = threading
block_size=$6
program_name=$7

# default n_input to 6
if [ -z "$n_input" ]; then
 n_input="6"
fi
 
# default iteration_input to 10000
if [ -z "$iteration_input" ]; then
 iteration_input="10000"
fi

# default file_input to med_data.dat
if [ -z "$file_input" ]; then
 file_input=./med_data.dat
fi

# default threshold_input to 2
if [ -z "$threshold_input" ]; then
 threshold_input="2"
fi

# default block_size to 1
if [ -z "$block_size" ]; then
 block_size="1"
fi

# default program_name to nmf
if [ -z "$program_name" ]; then
 program_name="nmf"
fi

if [ "$use_threading_input" == "1" ]; then
 iter_loop=(1 2 3 4 5 6 7 8)
else
 iter_loop=(1)
fi

for thread_cnt in "${iter_loop[@]}"
 do
  export OMP_NUM_THREADS="$thread_cnt"
  echo "$OMP_NUM_THREADS threads for $program_name -N $n_input -I $iteration_input -T $threshold_input -B $block_size $file_input" | tee -a $OUTPUT_FILE
  for iter in 1 2 3 4 5 6 7 # loop through 7 different iterations to get a good average of timing.
   do
    ret=$(./$program_name -N $n_input -I $iteration_input -T $threshold_input -B $block_size $file_input)
    stripped_time=`echo $ret | sed -e "s/.*${SUB_STR}//;s/${SUFFIX}.*//"`
    echo -n "${stripped_time}, " | tee -a $OUTPUT_FILE
   done
   echo "" | tee -a $OUTPUT_FILE
 done