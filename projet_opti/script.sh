#!/bin/bash

./a.out 100 0.95 1000 > log_100_0.95_1000.csv
echo 1 
./a.out 100 0.95 5000 > log_100_0.95_5000.csv
echo 2
./a.out 100 0.95 10000 > log_100_0.95_10000.csv
echo 3
./a.out 200 0.95 1000 > log_200_0.95_1000.csv
echo 4
./a.out 200 0.95 5000 > log_200_0.95_5000.csv
echo 5
./a.out 200 0.95 10000 > log_200_0.95_10000.csv
echo 6
./a.out 100 0.995 1000 > log_100_0.995_1000.csv
echo 7
./a.out 100 0.995 5000 > log_100_0.995_5000.csv
echo 8
./a.out 100 0.995 10000 > log_100_0.995_10000.csv
echo 9
