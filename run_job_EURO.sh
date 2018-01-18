#!/bin/bash
#$ -V
#$ -cwd
#$ -N EURO_5
#$ -j y
#$ -o job_EURO_1.txt
#$ -pe orte 4
mpirun -np 4 ./tiger-opt -i ./runs/EURO_C/20170925_1/Cement_Micro.i
