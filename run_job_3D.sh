#!/bin/bash
#$ -V
#$ -cwd
#$ -N sim_3D
#$ -j y
#$ -o job_3D.txt
#$ -pe orte 64
# setenv OMP_NUM_THREADS $NSLOTS
# setenv OMP_THREAD_LIMIT $NSLOTS

mpirun -np 64 ./tiger-opt -i ./runs/cement_gelation/Bazant_V1/cement_gelation_concave2.i
