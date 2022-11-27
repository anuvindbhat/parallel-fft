#ifndef FFT1D_H
#define FFT1D_H

#include <complex>
#include <vector>

template <bool inverse> void dft(std::vector<std::complex<double>> &vec);
template <bool inverse> void fft_rec(std::vector<std::complex<double>> &vec);
template <bool inverse> void fft_iter(std::vector<std::complex<double>> &vec);

#endif
