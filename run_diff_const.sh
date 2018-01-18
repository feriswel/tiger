#!/bin/bash
#$ -V
#$ -cwd
#$ -N sim_diff_const
#$ -j y
#$ -o job_diff_const.txt
#$ -pe orte 4
# setenv OMP_NUM_THREADS $NSLOTS
# setenv OMP_THREAD_LIMIT $NSLOTS

# ./tiger-opt -i ./runs/cement_gelation/Test/cement_gelation_react.i
mpirun -np 4 ./tiger-opt -i ./runs/cement_gelation/Test/cement_gelation_diff_const.i
