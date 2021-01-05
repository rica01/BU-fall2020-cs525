#! /bin/bash

time ./ser data/reference_1kbp.txt data/reads_9216_100bp.txt 3 cpuout1k
time ./ser data/reference_3kbp.txt data/reads_9216_100bp.txt 3 cpuout3k
time ./ser data/reference_6kbp.txt data/reads_9216_100bp.txt 3 cpuout6k
time ./ser data/reference_12kbp.txt data/reads_9216_100bp.txt 3 cpuout12k
time ./ser data/reference_25kbp.txt data/reads_9216_100bp.txt 3 cpuout25k
time ./ser data/reference_50kbp.txt data/reads_9216_100bp.txt 3 cpuout50k
time ./ser data/reference_100kbp.txt data/reads_9216_100bp.txt 3 cpuout100k

time ./par data/reference_1kbp.txt data/reads_9216_100bp.txt 3 gpuout1k
time ./par data/reference_3kbp.txt data/reads_9216_100bp.txt 3 gpuout3k
time ./par data/reference_6kbp.txt data/reads_9216_100bp.txt 3 gpuout6k
time ./par data/reference_12kbp.txt data/reads_9216_100bp.txt 3 gpuout12k
time ./par data/reference_25kbp.txt data/reads_9216_100bp.txt 3 gpuout25k
time ./par data/reference_50kbp.txt data/reads_9216_100bp.txt 3 gpuout50k
time ./par data/reference_100kbp.txt data/reads_9216_100bp.txt 3 gpuout100k