#!/bin/bash

# parse args
ARGS=("$@")
ARGS_LENGTH=${#ARGS[@]}
DIMENSION1=${ARGS[0]}
DIMENSION2=${ARGS[1]}
RESULT_FILE=./test_data/${ARGS[2]}

RANGE1_X=1
RANGE1_Y=3
RANGE2_X=20
RANGE2_Y=24
RANGE3_X=37
RANGE3_Y=41

RANGE1=$(($RANGE1_Y-$RANGE1_X+1))
RANGE2=$(($RANGE2_Y-$RANGE2_X+1))
RANGE3=$(($RANGE3_Y-$RANGE3_X+1))

if [[ $ARGS_LENGTH < 3 ]]; then
  echo "Incorrect number of arguments provided"
  echo "Usage -- ./generateData.sh {n_units_in_y_dimension} {m_units_in_x_dimension} {result_file_name.extension}"
  echo "Example usage -- ./generateData.sh 5 3 test_data.dat"
  exit 1
fi

# Clean up workspace by removing file from previous run
rm -f $RESULT_FILE

echo "Generating $DIMENSION1 x $DIMENSION2 test data set at $RESULT_FILE"

# Set up file headers
printf "variant" >> $RESULT_FILE

for (( n=0 ; n<$DIMENSION2 ; n+=1 )); do
  printf "\tattribute_$n" >> $RESULT_FILE
done

printf "\n" >> $RESULT_FILE

# Calculate the point at which the data can be split into thirds
third_first=$(( $DIMENSION1 / 3 ))
third_second=$(( $DIMENSION1 / 3 * 2 ))

for (( i=0 ; i<$DIMENSION1 ; i+=1)); do
  # Add row header based on row position in data
  if [[ $i -lt $third_first ]]; then
    printf "variant_1" >> $RESULT_FILE
    for (( n=1 ; n<=$DIMENSION2 ; n+=1 )); do
      R=$(($(($RANDOM%$RANGE1))+$RANGE1_X))
      new_r=$((($R+$n)*$n))
      printf "\t$new_r" >> $RESULT_FILE
    done
    printf "\n" >> $RESULT_FILE
  elif [[ $i -lt $third_second ]]; then
    printf "variant_2" >> $RESULT_FILE
    for (( n=1 ; n<=$DIMENSION2 ; n+=1 )); do
      R=$(($(($RANDOM%$RANGE2))+$RANGE2_X))
      new_r=$((($R+$n)*$n))
      printf "\t$new_r" >> $RESULT_FILE
    done
    printf "\n" >> $RESULT_FILE
  else
    printf "variant_3" >> $RESULT_FILE
    for (( n=1 ; n<=$DIMENSION2 ; n+=1 )); do
      R=$(($(($RANDOM%$RANGE3))+$RANGE3_X))
      new_r=$((($R+$n)*$n))
      printf "\t$new_r" >> $RESULT_FILE
    done
    printf "\n" >> $RESULT_FILE
  fi

done
