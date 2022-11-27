#include "fft1d.h"

#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
#include <math.h>


void discreteFourierTransform(std::vector<std::complex<double>> &data, bool invert) {
    int inputSize = data.size();
    std::vector<std::complex<double>> result(inputSize);
    
    #pragma omp parallel for
    for (int i = 0; i < inputSize; i++) {
        std::complex<double> accumulator(0);

        for (int j = 0; j < inputSize; j++) {
            double exponent = 2 * M_PI * j * i * (invert ? 1 : -1) / inputSize;
            std::complex<double> complexExponent(cos(exponent), sin(exponent));
            accumulator += complexExponent * data[j];
        }
        std::complex<double> inverse(1.0/inputSize);
        std::complex<double> one(1.0);
        result[i] = accumulator * (invert ? inverse : one);
    }
    
    result.swap(data);
}