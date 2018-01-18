#!/bin/bash

N=499
trial=0
trial_dir1=0
dir1=/home/tapeter/phase_field/V2/2D_nucleation_growth_v1/Output/Mechanics/T_10_01/temp1
dir2=/home/tapeter/projects/tiger/runs/cement_mechanics

while [ $trial -le $N ] ; do

    mkdir $dir1/$trial
    mv $dir1/E$trial_dir1.gz $dir1/$trial/E$trial_dir1.gz
    cp $dir2/youngs_modulus_in_512.e $dir1/$trial/youngs_modulus_in.e
    cp $dir2/cement_mechanics_bulk_512.i $dir1/$trial/cement_mechanics_bulk.i
    cp $dir2/cement_mechanics_shear_512.i $dir1/$trial/cement_mechanics_shear.i
    cp $dir2/import_modulus.py $dir1/$trial/import_modulus.py

    cd $dir1/$trial

    #rm ./param.txt
    #rm ./job.sh
    #rm ./out.txt
    # rm ./cement_mechanics_s2.i
    # rm ./cement_mechanics_shear_out.e


    echo $trial >> param.txt

    echo '#!/bin/bash' >> job.sh
    echo '#$ -V' >> job.sh
    echo '#$ -cwd' >> job.sh
    echo '#$ -N COPY_'$trial >> job.sh
    echo '#$ -j y' >> job.sh
    echo '#$ -o out.txt' >> job.sh
    echo 'setenv OMP_NUM_THREADS $NSLOTS' >> job.sh
    echo 'setenv OMP_THREAD_LIMIT $NSLOTS' >> job.sh
    echo 'python ./import_modulus.py' >> job.sh

    qsub ./job.sh

  let trial=$trial+1
  let trial_dir1=$trial_dir1+10
done
