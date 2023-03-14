# NMF  

This is a C implementation of Nonnegative Matrix Factorization using Itakura-Saito (IS) divergence criterion.
Multiplicative update rules are used for minimizing IS divergence.  

Input file: tab-delimited text with row headers and column headers.  

The Mersenne twister are used for a pseudorandom number generator. C codes are taken from http://www.math.sci.hiroshima-u.ac.jp/~m-mat/MT/MT2002/CODES/mt19937ar.c.  

This repository was originally forked from https://github.com/khigashi1987/NMF.  

## Installation and Set Up  

Below are the instructions for installing this application.  
*These instructions are valid as of 2023.03.14*  

### Environment Set Up
1. On your local machine, navigate to the directory to which this repository has been cloned, and run the following command.  
   `make`  

### Application Use  
1. To run the application using the test data, run the following command.  
   `./nmf -N 2 -I 50 ../iris_test_data.data`  
   This will create two files, one for W and one H named W.dat and H.dat respectively.  
   
2. To run test.sh script, run the following command.
   `./test.sh 2 50 iris_test_data.data 0`
   First Param is N, second is Iterations, third is data file name and last is 0=no threading, 1=threading
   This will generate or add to a test_output.txt file with results for 7 runs of each of 1-8 threads (if threading is activated)
   Default values are 2, 50, iris_test_data.data and 0 if none are passed in.