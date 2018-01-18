#!/bin/bash

N=499
trial=0
dir1=/home/tapeter/phase_field/V2/2D_nucleation_growth_v1/Output/Mechanics/T_10_01/temp1
# dir2=/home/tapeter/projects/tiger/runs/cement_mechanics

while [ $trial -le $N ] ; do

    cd $dir1/$trial

    # rm ./mech_out_bulk.txt
    # rm ./mech_job_bulk.sh
    # rm ./mech_out_shear.txt
    # rm ./mech_job_shear.sh

    echo '#!/bin/bash' >> mech_job_bulk.sh
    echo '#$ -V' >> mech_job_bulk.sh
    echo '#$ -cwd' >> mech_job_bulk.sh
    echo '#$ -N MECH_B'$trial >> mech_job_bulk.sh
    echo '#$ -j y' >> mech_job_bulk.sh
    echo '#$ -o mech_out_bulk.txt' >> mech_job_bulk.sh
    #echo '#$ -pe orte 4' >> mech_job_bulk.sh
    echo 'setenv OMP_NUM_THREADS $NSLOTS' >> mech_job_bulk.sh
    echo 'setenv OMP_THREAD_LIMIT $NSLOTS' >> mech_job_bulk.sh
    #echo 'mpirun -np 4 /home/tapeter/projects/tiger/tiger-opt -i '$dir1'/'$trial'/cement_mechanics_bulk.i' >> mech_job_bulk.sh
    echo '/home/tapeter/projects/tiger/tiger-opt -i '$dir1'/'$trial'/cement_mechanics_bulk.i' >> mech_job_bulk.sh

    qsub ./mech_job_bulk.sh

    echo '#!/bin/bash' >> mech_job_shear.sh
    echo '#$ -V' >> mech_job_shear.sh
    echo '#$ -cwd' >> mech_job_shear.sh
    echo '#$ -N MECH_S'$trial >> mech_job_shear.sh
    echo '#$ -j y' >> mech_job_shear.sh
    echo '#$ -o mech_out_shear.txt' >> mech_job_shear.sh
    #echo '#$ -pe orte 4' >> mech_job_shear.sh
    echo 'setenv OMP_NUM_THREADS $NSLOTS' >> mech_job_shear.sh
    echo 'setenv OMP_THREAD_LIMIT $NSLOTS' >> mech_job_shear.sh
    echo '/home/tapeter/projects/tiger/tiger-opt -i '$dir1'/'$trial'/cement_mechanics_shear.i' >> mech_job_shear.sh

    qsub ./mech_job_shear.sh

  let trial=$trial+1 #skip 4 trials to reduce resolution and save time
done
