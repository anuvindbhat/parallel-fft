#ifndef UTILS_H
#define UTILS_H

#include <complex>
#include <cstdint>
#include <vector>

inline constexpr double pi = 3.14159265358979323846;

template <typename T> constexpr bool is_pow2(T v) {
  return v != 0 and (v & (v - 1)) == 0;
}

template <typename T> int32_t floor_log2(T n) {
  int32_t ret = 0;
  // n must be > 0
  while (n != 1) {
    ++ret;
    n >>= 1;
  }
  return ret;
}

template <typename T> int32_t ceil_log2(T n) {
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
to_complex_2d(const std::vector<std::vector<double>> &mat) {
  int32_t n = mat.size();
  if (n == 0) {
    return {};
  }
  std::vector<std::vector<std::complex<double>>> cmat(n);
  for (int32_t i = 0; i < n; ++i) {
    cmat[i] = to_complex(mat[i]);
  }
  return cmat;
}

inline std::vector<std::vector<double>>
to_reals_2d(const std::vector<std::vector<std::complex<double>>> &cmat) {
  int32_t n = cmat.size();
  if (n == 0) {
    return {};
  }
  std::vector<std::vector<double>> mat(n);
  for (int32_t i = 0; i < n; ++i) {
    mat[i] = to_reals(cmat[i]);
  }
  return mat;
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
