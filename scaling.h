#!/bin/bash -x
rm -f ./a.out; mpiicc -O0 nBody-mpi.c || exit -1
for k in 3 5 10 20 50;do  mpirun -np $k ./a.out > $k.txt;done
