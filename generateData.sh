#!/bin/bash

# parse args
ARGS=("$@")
ARGS_LENGTH=${#ARGS[@]}
DIMENSION1=${ARGS[0]}
DIMENSION2=${ARGS[1]}
RESULT_FILE=${ARGS[2]}

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
  if [[ $i < $third_first ]]; then
    printf "variant_1" >> $RESULT_FILE
  elif [[ $i < $third_second ]]; then
    printf "variant_2" >> $RESULT_FILE
  else
    printf "variant_3" >> $RESULT_FILE
  fi

  # Add randomly generated numbers to matrix
  for (( n=0 ; n<$DIMENSION2 ; n+=1 )); do
    printf "\t$RANDOM" >> $RESULT_FILE
  done

  # Add new line at the end of each row
  printf "\n" >> $RESULT_FILE
done
