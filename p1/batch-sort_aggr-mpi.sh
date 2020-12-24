echo 1 threads

echo 250K
time mpirun -n 1 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_250K /dev/null

echo 500K
time mpirun -n 1 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 1 ./sort_aggr-serial s_500K /dev/null

echo 1M
time mpirun -n 1 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 1 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 1 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 1 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 1 ./sort_aggr-serial s_1M /dev/null


echo 2 threads

echo 250K
time mpirun -n 2 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_250K /dev/null

echo 500K
time mpirun -n 2 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 2 ./sort_aggr-serial s_500K /dev/null

echo 1M
time mpirun -n 2 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 2 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 2 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 2 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 2 ./sort_aggr-serial s_1M /dev/null


echo 4 threads

echo 250K
time mpirun -n 4 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_250K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_250K /dev/null

echo 500K
time mpirun -n 4 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_500K /dev/null
time mpirun -n 4 ./sort_aggr-serial s_500K /dev/null

echo 1M
time mpirun -n 4 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 4 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 4 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 4 ./sort_aggr-serial s_1M /dev/null
time mpirun -n 4 ./sort_aggr-serial s_1M /dev/null
