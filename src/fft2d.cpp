#include "fft1d.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <complex>
#include <cstdint>
#include <vector>

template <typename T> void transpose(std::vector<std::vector<T>> &mat) {
  int32_t n = mat.size();
  if (n == 0) {
    return;
  }
  assert(n == static_cast<int32_t>(mat[0].size()));
#pragma omp parallel for collapse(2)
  for (int i = 0; i < n; ++i) {
    for (int j = i + 1; j < n; ++j) {
      std::swap(mat[i][j], mat[j][i]);
    }
  }
}

template <bool inverse>
void fft2d_iter(std::vector<std::vector<std::complex<double>>> &mat) {
  int32_t n = mat.size();
  assert(is_pow2(n));
  assert(n == static_cast<int32_t>(mat[0].size()));
#pragma omp parallel for schedule(static)
  for (auto &vec : mat) {
    fft_iter<inverse>(vec);
  }
  transpose(mat);
#pragma omp parallel for schedule(static)
  for (auto &vec : mat) {
    fft_iter<inverse>(vec);
  }
  transpose(mat);
}

// instantiate here since the template definition is in the cpp file instead
// of the header file
template void
fft2d_iter<false>(std::vector<std::vector<std::complex<double>>> &mat);
template void
fft2d_iter<true>(std::vector<std::vector<std::complex<double>>> &mat);
