#include "fft1d.h"
#include "utils.h"
#include <algorithm>
#include <complex>
#include <utility>
#include <vector>

// 16 best for GHC, 64 for PSC
inline constexpr int recursion_threshold = 16;

template <bool inverse> void dft(std::vector<std::complex<double>> &vec) {
  int n = vec.size();
  std::vector<std::complex<double>> result(n, 0);
  constexpr int flag = inverse ? 1 : -1;
  // precompute the roots since it is much more efficient to retrieve
  // precomputed values than compute them repeatedly on the fly especially
  // when the data fits in the cache
  // since DFT is quadratic, we'll only be using it for small inputs anyway
  std::vector<std::complex<double>> w(n);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    w[i] = std::polar(1.0, flag * 2 * pi * i / n);
  }
// Won't have any effect when called from fft_rec since nested parallelism is
// disabled
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    for (int j = 0; j < n; ++j) {
      // auto currw = std::polar(1.0, flag * 2 * pi * i * j / n);
      auto currw = w[(i * j) % n];
      result[i] += vec[j] * currw;
    }
    if constexpr (inverse) {
      result[i] /= n;
    }
  }
  vec = std::move(result);
}

template <bool inverse>
void fft_rec_helper(std::vector<std::complex<double>> &vec) {
  int n = vec.size();
  assert_pow2(n);
  if (n == 1) {
    return;
  }
  if (n <= recursion_threshold) {
    // switch to quadratic DFT for small arrays since it has better cache
    // locality
    dft<inverse>(vec);
    return;
  }
  std::vector<std::complex<double>> even, odd;
#pragma omp task default(shared) untied
  {
    // initialize even array inside the task for better parallelism/speedup
    // at the expense of more total work
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
    auto currw = std::polar(1.0, flag * 2 * pi * i / n);
    vec[i] = even[i] + currw * odd[i];
    vec[n / 2 + i] = even[i] - currw * odd[i];
    if constexpr (inverse) {
      vec[i] /= 2;
      vec[n / 2 + i] /= 2;
    }
    // it's more performant to iteratively update currw i.e. currw starts at
    // 1.0 and multiply by wn every iteration where wn is
    // std::polar(1.0, flag * 2 * pi / n)
    // however, the numerical precision is much worse than using std::polar
    // every iteration
  }
}

template <bool inverse> void fft_rec(std::vector<std::complex<double>> &vec) {
#pragma omp parallel default(shared)
#pragma omp single nowait
  { fft_rec_helper<inverse>(vec); }
}

inline int floor_log2(int n) {
  int ret = -1;
  while (n != 0) {
    ++ret;
    n >>= 1;
  }
  return ret;
}

inline int bit_reverse(int n, int total_bits) {
  int rev = 0;
  for (int i = 0; i < total_bits; ++i) {
    if (((1 << i) & n) != 0) {
      rev |= 1 << (total_bits - 1 - i);
    }
  }
  return rev;
}

template <bool inverse> void fft_iter(std::vector<std::complex<double>> &vec) {
  int n = vec.size();
  assert_pow2(n);
  // the total number of bits required to represent 0..n-1 since n is a power
  // of 2
  int total_bits = floor_log2(n);
#pragma omp parallel for schedule(static)
  for (int i = 0; i < n; ++i) {
    int rev_i = bit_reverse(i, total_bits);
    // since bit_reverse(bit_reverse(i)) == i, i and bit_reverse(i) just need
    // to swap places to get the bit_reverse-sorted array
    if (i < rev_i) {
      std::swap(vec[i], vec[rev_i]);
    }
  }
  constexpr int flag = inverse ? 1 : -1;
  // we can precompute the powers of the different primitive roots of unity
  // that are required instead of computing them repeatedly on the fly
  // NOTE: tested and verified that it's more efficient to retrieve the
  //   precomputed value than recompute every time; w is accessed with good
  //   spatial locality
  std::vector<std::complex<double>> w(n - 1);
  if constexpr (false) {
    // this reference implementation isn't easy to parallelize well; just left
    // it here for reference
    for (int len = 2; len <= n; len *= 2) {
      for (int i = 0; i < len / 2; ++i) {
        w[len / 2 + i - 1] = std::polar(1.0, flag * 2 * pi * i / len);
      }
    }
  }
#pragma omp parallel for schedule(static)
  for (int j = 1; j < n; ++j) {
    int temp = 1 << floor_log2(j);
    int len = 2 * temp;
    int i = j - temp;
    w[j - 1] = std::polar(1.0, flag * 2 * pi * i / len);
  }
  auto loop_body = [&vec, &w](int len, int j, int i) {
    // auto currw = std::polar(1.0, flag * 2 * pi * i / len);
    auto currw = w[len / 2 + i - 1];
    int even_i = j + i;
    int odd_i = j + i + len / 2;
    // we can overwrite in-place since result[i] and result[i + n / 2] only
    // depend on input[i] and input[i + n / 2] i.e. an iteration of the i
    // loop only depends on the values that it modifies
    auto even = vec[even_i];
    auto odd = vec[odd_i];
    vec[even_i] = even + currw * odd;
    vec[odd_i] = even - currw * odd;
    // we can also just divide each element by n at the very end
    // we divide here to mimic the recursive implementation
    if constexpr (inverse) {
      vec[even_i] /= 2;
      vec[odd_i] /= 2;
    }
  };
  // how many elements of vec and w fit in the L1 cache (possibly L2)
  int cache_size = std::min(n, 1 << 14);
#pragma omp parallel for schedule(static)
  for (int jb = 0; jb < n; jb += cache_size) {
    // for chunks that fit in the cache, perform FFT for the entire chunk
    // before moving to the next chunk (good cache reuse since the chunk only
    // needs to be loaded in once)
    for (int len = 2; len <= cache_size; len *= 2) {
      for (int j = jb; j < jb + cache_size; j += len) {
        for (int i = 0; i < len / 2; ++i) {
          loop_body(len, j, i);
        }
      }
    }
  }
  // perform regular bottom-up FFT for the entire array once the chunks no
  // longer fit in the cache since bottom-up FFT has good spatial locality
  for (int len = 2 * cache_size; len <= n; len *= 2) {
#pragma omp parallel for schedule(static)
    for (int j = 0; j < n; j += len) {
      for (int i = 0; i < len / 2; ++i) {
        loop_body(len, j, i);
      }
    }
  }
  // could alternatively have done this instead of dividing by 2 at every
  // stage (this is obviously faster)
  if constexpr (false) {
#pragma omp parallel for schedule(static)
    for (auto &v : vec) {
      v /= n;
    }
  }
}

// instantiate here since the template definition is in the cpp file instead
// of the header file
template void dft<false>(std::vector<std::complex<double>> &vec);
template void dft<true>(std::vector<std::complex<double>> &vec);
template void fft_rec<false>(std::vector<std::complex<double>> &vec);
template void fft_rec<true>(std::vector<std::complex<double>> &vec);
template void fft_iter<false>(std::vector<std::complex<double>> &vec);
template void fft_iter<true>(std::vector<std::complex<double>> &vec);
