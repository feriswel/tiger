#!/bin/bash

N=399
trial=0
dir1=/home/tapeter/phase_field/V2/2D_nucleation_growth_v1/Output/PL_Data/T2_512/temp1
dir2=/home/tapeter/projects/tiger

while [ $trial -le $N ] ; do

    cd $dir1/$trial/
    rm ./mech_job.sh
    rm ./mech_out.txt

    echo '#!/bin/bash' >> mech_job.sh
    echo '#$ -V' >> mech_job.sh
    echo '#$ -cwd' >> mech_job.sh
    echo '#$ -N MECH_'$trial >> mech_job.sh
    echo '#$ -j y' >> mech_job.sh
    echo '#$ -o mech_out.txt' >> mech_job.sh
    echo 'setenv OMP_NUM_THREADS $NSLOTS' >> mech_job.sh
    echo 'setenv OMP_THREAD_LIMIT $NSLOTS' >> mech_job.sh
    echo $dir2'/tiger-opt -i '$dir1'/'$trial'/cement_mechanics.i' >> mech_job.sh

    qsub ./mech_job.sh
    rm -r $dir1/$trial/home

  let trial=$trial+1
done
