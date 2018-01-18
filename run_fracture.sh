#!/bin/bash
#$ -V
#$ -cwd
#$ -N tiger_mech
#$ -j y
#$ -o job_frac.txt
#$ -pe orte 4
mpirun -np 4 ./tiger-opt -i ./runs/cement_fracture/Cement_Fracture.i
