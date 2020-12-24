#! /usr/bin/python
import numpy as np
import sys

numImages = int(sys.argv[1])

for i in range(numImages):
    random_matrix = np.random.randint(0, 256, (i*1000, i*1000))
    f = open(str(i*1000)+"x"+str(i*1000), "w")
    f.write(str(i*1000))
    f.write("\n")
    f.write(str(i*1000))
    f.write("\n")

    for r in range(i*1000):
        for c in range(i*1000):
            val = random_matrix[r][c]
            f.write(str(val)+" ")
        f.write("\n")

    f.close()
