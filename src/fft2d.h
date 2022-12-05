#ifndef FFT2D_H
#define FFT2D_H

#include <complex>
#include <vector>

template <bool inverse>
void fft2d_iter(std::vector<std::vector<std::complex<double>>> &mat);

#endif
