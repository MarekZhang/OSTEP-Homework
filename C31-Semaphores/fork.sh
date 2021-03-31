#!/bin/sh
gcc -o fork-join fork-join.c -Wall -pthread

n=10

while [ $n -gt 0 ]
do
  ./fork-join
  n=$((n-1))
  echo ""
done
