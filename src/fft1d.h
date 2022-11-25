#ifndef FFT1D_H
#define FFT1D_H

#include <complex>
#include <vector>

template <bool inverse> void fft_rec(std::vector<std::complex<double>> &vec);

std::vector<std::complex<double>> discreteFourierTransform(std::vector<std::complex<double>> &data, bool invert = false);

#endif
