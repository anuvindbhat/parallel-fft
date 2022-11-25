#include "fft1d.h"
#include "utils.h"
#include <complex>
#include <vector>

template <bool inverse>
void fft_rec_helper(std::vector<std::complex<double>> &vec) {
  auto n = vec.size();
  assert_pow2(n);
  if (n == 1) {
    return;
  }
  std::vector<std::complex<double>> even(n / 2), odd(n / 2);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n / 2; ++i) {
    even[i] = vec[2 * i];
    odd[i] = vec[2 * i + 1];
  }
#pragma omp task default(shared) untied
  { fft_rec_helper<inverse>(even); }
  fft_rec_helper<inverse>(odd);
#pragma omp taskwait
  constexpr int flag = inverse ? 1 : -1;
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n / 2; ++i) {
    std::complex<double> currw = std::polar(1., flag * 2 * pi * i / n);
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
