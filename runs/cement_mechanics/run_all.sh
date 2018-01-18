#!/bin/bash


dir1=/home/tapeter/phase_field/V2/2D_nucleation_growth_v1/Output/PL_Data/T2_512/temp5
dir2=/home/tapeter/projects/tiger
trial=399
N=0

while [ $trial -ge $N ] ; do

    $dir2/tiger-opt -i $dir1/$trial/cement_mechanics.i

  let trial=$trial-1
done
