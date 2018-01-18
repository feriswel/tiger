#!/bin/bash
#$ -V
#$ -cwd
#$ -N sim_diff
#$ -j y
#$ -o job_diff_1D.txt
# #$ -pe orte 4
setenv OMP_NUM_THREADS $NSLOTS
setenv OMP_THREAD_LIMIT $NSLOTS

./tiger-opt -i ./runs/cement_gelation/Test/cement_gelation_diff_1D.i
# mpirun -np 4 ./tiger-opt -i ./runs/cement_gelation/Test/cement_gelation_diff.i
