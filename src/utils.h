#ifndef UTILS_H
#define UTILS_H

#include <complex>
#include <concepts>
#include <cstdint>
#include <vector>

inline constexpr double pi = 3.14159265358979323846;

template <std::integral T> constexpr bool is_pow2(T v) {
  return v != 0 and (v & (v - 1)) == 0;
}

template <std::integral T> int32_t floor_log2(T n) {
  int32_t ret = 0;
  // n must be > 0
  while (n != 1) {
    ++ret;
    n >>= 1;
  }
  return ret;
}

template <std::integral T> int32_t ceil_log2(T n) {
  int32_t ret = 0;
  while ((1 << ret) < n) {
    ++ret;
  }
  return ret;
}

inline std::vector<std::complex<double>>
to_complex(const std::vector<double> &vec) {
  std::vector<std::complex<double>> cvec(vec.size());
  for (int32_t i = 0; i < static_cast<int32_t>(vec.size()); ++i) {
    cvec[i] = vec[i];
  }
  return cvec;
}

inline std::vector<double>
to_reals(const std::vector<std::complex<double>> &cvec) {
  std::vector<double> vec(cvec.size());
  for (int32_t i = 0; i < static_cast<int32_t>(vec.size()); ++i) {
    vec[i] = cvec[i].real();
  }
  return vec;
}

inline std::vector<std::vector<std::complex<double>>>
to_complex_2d(const std::vector<std::vector<double>> &vecs) {
  std::vector<std::vector<std::complex<double>>> cvecs;
  for (std::vector<double> vec : vecs) {
    cvecs.push_back(to_complex(vec));
  }
  return cvecs;
}

inline std::vector<std::vector<double>>
to_reals_2d(const std::vector<std::vector<std::complex<double>>> &cvecs) {
  std::vector<std::vector<double>> vecs;
  for (std::vector<std::complex<double>> cvec : cvecs) {
    vecs.push_back(to_reals(cvec));
  }
  return vecs;
}

template <typename T>
std::ostream &operator<<(std::ostream &os, const std::vector<T> &vec) {
  int32_t n = vec.size();
  os << '[';
  for (int32_t i = 0; i < n; ++i) {
    os << vec[i];
    if (i != n - 1) {
      os << ", ";
    }
  }
  os << ']';
  return os;
}

#endif
