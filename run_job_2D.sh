#!/bin/bash
#$ -V
#$ -cwd
#$ -N sim_concave
#$ -j y
#$ -o job_concave.txt
#$ -pe orte 4
# setenv OMP_NUM_THREADS $NSLOTS
# setenv OMP_THREAD_LIMIT $NSLOTS

# ./tiger-opt -i ./runs/cement_gelation/Test/cement_gelation_react.i
mpirun -np 4 ./tiger-opt -i ./runs/cement_gelation/Bazant_V1/cement_gelation_concave.i
