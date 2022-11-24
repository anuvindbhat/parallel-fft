#include "common.h"
#include "mpi.h"
#include "timing.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
#include <math.h>

void discreteFourierTransform(std::vector<std::complex<double>> &data, bool invert = false) {
    int inputSize = data.size();
    std::vector<std::complex<double>> result(inputSize);
    
    for (int i = 0; i < inputSize; i++) {
        std::complex<double> accumulator(0);
        for (int j = 0; j < inputSize; j++) {
            double exponent = 2 * M_PI * j * i * (invert ? 1 : -1)/ inputSize;
            std::complex<double> complexExponent(cos(exponent), sin(exponent));
            accumulator += complexExponent;
        }
        result[i] = accumulator * (invert ? 1.0/inputSize : 1);
    }
}