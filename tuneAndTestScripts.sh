#!/bin/bash

measureParallelThreshold() {
    for threshold in 1 2 4 8 16 32 48 64 128 256 512; do
        sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = $threshold/" src/fft1d.cpp

        make

        echo -ne "$threshold | " 

        ./fft.out
    done
}

measureParallelThresholdForRec() {
    export OMP_NUM_THREADS=8
    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    for threshold in 1 2 4 8 16 32 48 64 128 256 512; do
        sed -i -r "s/recursion_threshold = [0-9]*/recursion_threshold = $threshold/" src/fft1d.cpp

        printf "\nThreshold: $threshold | " >> threshold.txt
        
        sed -i -r "s/currDataSet = [0-9]*/currDataSet = 4/" src/main.cpp

        make clean
        
        make
        
        ./fft.out >> threshold.txt
    done
}


measureSpeedupFFT () {

    printf "All timing in ms\n" > timingRecursiveFFT.txt
    printf "1 << 15, 1 << 20, 1 << 25\n" >> timingRecursiveFFT.txt
    printf "=====================" >> timingRecursiveFFT.txt

    sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = 16/" src/fft1d.cpp # change to 64 for PSC

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timingRecursiveFFT.txt
        for dataSet in 0 1 2 3 4; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            printf "FFTRec: (Thread: $threads, DataSet: $dataSet)\n"

            make clean
            
            make
            
            ./fft.out >> timingRecursiveFFT.txt
        done
    done
}

measureSpeedupDFT () {

    printf "All timing in ms\n" > timingDFT.txt
    printf "1 << 15, 1 << 20, 1 << 25\n" >> timingDFT.txt
    printf "=====================" >> timingDFT.txt

    sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = 16/" src/fft1d.cpp # change to 64 for PSC

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 0/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timingDFT.txt
        for dataSet in 0 1 2; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            printf "DFT: (Thread: $threads, DataSet: $dataSet)\n"

            make clean
            
            make
            
            ./fft.out >> timingDFT.txt
        done
    done
}

measureSpeedupFFTIter () {

    printf "All timing in ms\n" > timingIterativeFFT.txt
    printf "1 << 15, 1 << 20, 1 << 25\n" >> timingIterativeFFT.txt
    printf "=====================" >> timingIterativeFFT.txt

    sed -i -r "s/ParallelRecusionThreshold = [0-9]*/ParallelRecusionThreshold = 16/" src/fft1d.cpp # change to 64 for PSC

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 2/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timingIterativeFFT.txt
        for dataSet in 0 1 2 3 4; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            printf "FFTIter: (Thread: $threads, DataSet: $dataSet)\n"

            make clean
            
            make
            
            ./fft.out >> timingIterativeFFT.txt
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

measureSpeedupFFTWithZeroThreshold () {

    printf "All timing in ms\n" > timingRecursiveFFTZeroThreshold.txt
    printf "1 << 15, 1 << 20, 1 << 25\n" >> timingRecursiveFFTZeroThreshold.txt
    printf "=====================" >> timingRecursiveFFTZeroThreshold.txt

    sed -i -r "s/recursion_threshold = [0-9]*/recursion_threshold = 1/" src/fft1d.cpp

    sed -i -r "s/codeBlock = [0-9]*/codeBlock = 1/" src/main.cpp

    for threads in 1 2 4 8; do # add 16 32 64 128 for PSC
        export OMP_NUM_THREADS=$threads

        printf "\n$threads | " >> timingRecursiveFFTZeroThreshold.txt
        for dataSet in 0 1 2; do
            sed -i -r "s/currDataSet = [0-9]*/currDataSet = $dataSet/" src/main.cpp

            sed -i -r "s/currThreadCount = [0-9]*/currThreadCount = $threads/" src/main.cpp

            printf "FFTRec: (Thread: $threads, DataSet: $dataSet)\n"

            make clean
            
            make
            
            ./fft.out >> timingRecursiveFFTZeroThreshold.txt
        done
    done
}


# Make sure to modify speedup functions when running on PSC

measureSpeedupFFTWithZeroThreshold
