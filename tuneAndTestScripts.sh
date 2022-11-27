#!/bin/bash

measureParallelThreshold() {
    for threshold in 1 2 4 8 16 32 48 64 128 256 512; do
        echo $threshold 

        sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = $threshold/" src/fft1d.cpp

        make

        ./fft.out
    done
}


measureSpeedupFFT () {

    printf "All timing in ms\n" > timingRecursive.txt
    printf "1 << 3, 1 << 10, 1 << 12, 1 << 14, 1 << 15, 1 << 17, 1 << 19, 1 << 20\n" >> timingRecursive.txt
    printf "=====================" >> timingRecursive.txt

    sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = 16/" src/fft1d.cpp

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timingRecursive.txt
        for dataSet in 0 1 2 3 4 5 6 7; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            make clean
            
            make
            
            ./fft.out >> timingRecursive.txt
        done
    done
}

measureSpeedupDFT () {

    printf "All timing in ms\n" > timing.txt
    printf "1 << 3, 1 << 10, 1 << 12, 1 << 14, 1 << 15\n" >> timing.txt
    printf "=====================" >> timing.txt

    sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = 16/" src/fft1d.cpp

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 0/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timing.txt
        for dataSet in 0 1 2 3 4; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            make clean
            
            make
            
            ./fft.out >> timing.txt
        done
    done
}

# deprecated
measureCacheAgainstSize () {
    export OMP_NUM_THREADS=8
    for dataSet in 0 1 2 3 4 5 6 7; do
        sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

        sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = 8/" src/main.cpp

        make clean
            
        make

        printf "DataSet: $dataSet\n=====================\n" >> cacheRecursive.txt

        perf stat -e cache-misses -e cache-references -o cacheRecursive.txt --append ./fft.out
    done
}

# deprecated
measureCacheAgainstSizeDFT () {
    export OMP_NUM_THREADS=8
    for dataSet in 0 1 2 3 4; do
        sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

        sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = 8/" src/main.cpp

        make clean
            
        make

        printf "DataSet: $dataSet\n=====================\n" >> cache.txt

        perf stat -e cache-misses -e cache-references -o cache.txt --append ./fft.out
    done
}

measureCacheAgainstParThreshold () {
    export OMP_NUM_THREADS=8

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = 8/" src/main.cpp

    sed -i -r "s/currDataSet = [0-9]*/currDataSet = 7/" src/main.cpp

    # printf "" > cacheRecursive.txt

    for threshold in 1 2 4 8 16 32 48 64 128 256 512 1024; do

        sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = $threshold/" src/fft1d.cpp

        make clean

        make

        printf "Threshold: $threshold\n=====================\n" >> cacheRecursive.txt

        perf stat -e cache-misses -e cache-references -o cacheRecursive.txt --append ./fft.out
    done
}

measureCacheAgainstParThresholdPSCversion () {
    export OMP_NUM_THREADS=128

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = 128/" src/main.cpp

    sed -i -r "s/currDataSet = [0-9]*/currDataSet = 7/" src/main.cpp

    # printf "" > cacheRecursive.txt

    for threshold in 1 2 4 8 16 32 48 64 128 256 512 1024; do

        sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = $threshold/" src/fft1d.cpp

        make clean

        make

        printf "Threshold: $threshold\n=====================\n" >> cacheRecursive.txt

        perf stat -e L1-dcache-load-misses -e cache-references -o cacheRecursive.txt --append ./fft.out
    done
}

measureSpeedupDFT
