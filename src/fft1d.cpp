#include "fft1d.h"
#include "utils.h"
#include <complex>
#include <utility>
#include <vector>

// 16 best for GHC, 64 for PSC
inline constexpr int recursion_threshold = 16;

template <bool inverse> void dft(std::vector<std::complex<double>> &vec) {
  int n = vec.size();
  std::vector<std::complex<double>> result(n, 0);
  constexpr int flag = inverse ? 1 : -1;
// TODO: Do we need a pure serial version to call from the recursive FFT
// implementation?
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      auto currw = std::polar(1.0, flag * 2 * pi * i * j / n);
      result[i] += vec[j] * currw;
    }
    if constexpr (inverse) {
      result[i] /= n;
    }
  }
  vec = std::move(result);
}

// instantiate here since the template definition is in the cpp file instead
// of the header file
template void dft<false>(std::vector<std::complex<double>> &vec);
template void dft<true>(std::vector<std::complex<double>> &vec);

template <bool inverse>
void fft_rec_helper(std::vector<std::complex<double>> &vec) {
  int n = vec.size();
  assert_pow2(n);
  if (n == 1) {
    return;
  }
  if (n <= recursion_threshold) {
    dft<inverse>(vec);
    return;
  }
  std::vector<std::complex<double>> even, odd;
#pragma omp task default(shared) untied
  {
    even.resize(n / 2);
    for (int i = 0; i < n / 2; ++i) {
      even[i] = vec[2 * i];
    }
    fft_rec_helper<inverse>(even);
  }
  odd.resize(n / 2);
  for (int i = 0; i < n / 2; ++i) {
    odd[i] = vec[2 * i + 1];
  }
  fft_rec_helper<inverse>(odd);
#pragma omp taskwait
  constexpr int flag = inverse ? 1 : -1;
  for (int i = 0; i < n / 2; ++i) {
    std::complex<double> currw = std::polar(1.0, flag * 2 * pi * i / n);
    vec[i] = even[i] + currw * odd[i];
    vec[n / 2 + i] = even[i] - currw * odd[i];
    if constexpr (inverse) {
      vec[i] /= 2;
      vec[n / 2 + i] /= 2;
    }
  }
}

template <bool inverse> void fft_rec(std::vector<std::complex<double>> &vec) {
#pragma omp parallel default(shared)
#pragma omp single nowait
  { fft_rec_helper<inverse>(vec); }
}

// instantiate here since the template definition is in the cpp file instead
// of the header file
template void fft_rec<false>(std::vector<std::complex<double>> &vec);
template void fft_rec<true>(std::vector<std::complex<double>> &vec);
