#!/usr/bin/env python3

# THIS FILE IS DEPRECATED

import sys
import os
import re

# Usage: ./checker.py v1/v2 0/1
original_stdout = sys.stdout
threads = [1, 2, 4, 8]
dataSets = [0, 1, 2, 3, 4, 5]

f = open('timing.txt', 'a')
sys.stdout = f
print("All timing in ms")
print("0, 1, 2, 3")
print("==============================================")
sys.stdout = original_stdout
f.close()
for i, aThread in enumerate(threads):
    os.system(f"export OMP_NUM_THREADS={aThread}")
    alpha = open('timing.txt', 'a')
    sys.stdout = alpha
    print(f"\n{aThread} | ", end='')
    sys.stdout = original_stdout
    alpha.close()
    for j, aDataSet in enumerate(dataSets):

        fin = open(os.path.join(os.path.dirname(__file__), "src/main.cpp"), "rt")
        #read file contents to string
        data = fin.read()
        #replace all occurrences of the required string
        if j - 1 == -1:
            data = data.replace("currDataSet = 0", f"currDataSet = {aDataSet}")
            data = data.replace("currThreadCount = 0", f"currThreadCount = {aThread}")
        else:
            data = data.replace(f"currDataSet = {dataSets[j-1]}", f"currDataSet = {aDataSet}")
            data = data.replace(f"currThreadCount = {threads[j-1]}", f"currThreadCount = {aThread}")
        #close the input file
        fin.close()
        #open the input file in write mode
        fin = open(os.path.join(os.path.dirname(__file__), "src/main.cpp"), "wt")
        #overrite the input file with the resulting data
        fin.write(data)
        #close the file
        fin.close()
        
        # os.system(f"sed -i -r 's/currDataSet = [0-9]*/currDataSet = {aDataSet}/' src/main.cpp")

        # os.system(f"sed -i -r 's/currThreadCount = [0-9]*/currThreadCount = {aThread}/' src/main.cpp")

        os.system("make all")

        os.system("./fft.out >> timing.txt")
sys.stdout = original_stdout