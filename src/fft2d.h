#ifndef FFT2D_H
#define FFT2D_H

#include <complex>
#include <vector>

template <bool inverse>
void fft_rec_2d(std::vector<std::vector<std::complex<double>>> &vec);
template <bool inverse>
void fft_iter_2d(std::vector<std::vector<std::complex<double>>> &vec);

#endif
