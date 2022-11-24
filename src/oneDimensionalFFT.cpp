#include "common.h"
#include "mpi.h"
#include "timing.h"
#include <cmath>
#include <vector>
#include <algorithm>
#include <complex>
#include <math.h>

void fastFourierTransform(std::vector<std::complex<double>> &data, bool invert = false) {
    int inputSize = data.size();
    if (inputSize == 1) return;
    std::vector<std::complex<double>> evenPosData(inputSize/2), oddPosData(inputSize/2);

    // Sorting even positioned data into evenVector and same for odd

    for(int i = 0; i < inputSize; i++) {
        if (i % 2 == 0) {
            evenPosData[i/2] = data[i];
        } else {
            oddPosData[i/2] = data[i];
        }
    }

    fastFourierTransform(evenPosData);
    fastFourierTransform(oddPosData);

    double exponent = -2 * M_PI / inputSize; // * -1
    if (invert) exponent *= -1;

    std::complex<double> complexExponent(cos(exponent), sin(exponent));
    std::complex<double> accumulator(1);


    for (int i = 0; i < inputSize/2; i++) {
        std::complex<double> evenVal(evenPosData[i]), oddVal(oddPosData[i]);
        
        data[i] = evenVal + accumulator * oddVal * (invert ? 0.5 : 1);
        data[i + inputSize/2] = evenVal - accumulator * oddVal * (invert ? 0.5 : 1);

        accumulator *= complexExponent;
    }
}