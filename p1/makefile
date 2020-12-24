# ********************
# Ricardo Roman-Brenes
# 22001125
# oct-2020
#
# Bilkent University
# CS525
# *********************

CC=gcc
C_FLAGS=-O0 -g
LIBS=

MPICC=mpicc
MPIRUN=mpirun


build:
	$(CC) $(C_FLAGS) range-serial.c aux.c -o range-serial
	$(MPICC) $(C_FLAGS) range-mpi-ppv1.c aux.c -o range-mpi-ppv1
	$(MPICC) $(C_FLAGS) range-mpi-ppv2.c aux.c -o range-mpi-ppv2
	$(CC) $(C_FLAGS) sort_aggr-serial.c aux.c -o sort_aggr-serial
	$(MPICC) $(C_FLAGS) sort_aggr-mpi.c aux.c -o sort_aggr-mpi
	
run:
	./range-serial input_range
	$(MPIRUN) ./range-mpi-ppv1 input_range
	./sort_aggr-serial input_sort_aggr output_sort_aggr-serial
	$(MPIRUN) ./sort_aggr-mpi input_sort_aggr output_sort_aggr-mpi

clean:
	rm -rf *.o
	rm -rf *.log
	rm -rf range-serial range-mpi-ppv1 range-mpi-ppv2 ssort_aggr-serial sort_aggr-mpi