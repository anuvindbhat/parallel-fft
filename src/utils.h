#ifndef UTILS_H
#define UTILS_H

#include <cassert>
#include <complex>
#include <vector>

inline constexpr double pi = 3.14159265358979323846;

template <typename T> void assert_pow2(T v) {
  assert(v != 0 and (v & (v - 1)) == 0);
}

inline std::vector<std::complex<double>>
to_complex(const std::vector<double> &vec) {
  std::vector<std::complex<double>> cvec(vec.size());
  for (int i = 0; i < vec.size(); ++i) {
    cvec[i] = vec[i];
  }
  return cvec;
}

inline std::vector<double>
to_reals(const std::vector<std::complex<double>> &cvec) {
  std::vector<double> vec(cvec.size());
  for (int i = 0; i < vec.size(); ++i) {
    vec[i] = cvec[i].real();
  }
  return vec;
}

#endif
