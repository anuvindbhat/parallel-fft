#include "fft1d.h"
#include "fft2d.h"
#include "ppm.h"
#include "timer.h"
#include "utils.h"
#include <algorithm>
#include <cassert>
#include <chrono>
#include <complex>
#include <cstdint>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <string>
#include <vector>

void test1d() {
  int n = 1 << 22;
  std::vector<double> vec(n);
  std::iota(vec.begin(), vec.end(), 0);
  std::cout << "Initial: " << std::vector(vec.begin(), vec.begin() + 16)
            << "\n";
  // std::cout << "Initial: " << vec << "\n";
  auto cvec = to_complex(vec);
  // std::cout << "Before FFT: " << cvec << "\n";
  {
    Timer<std::chrono::milliseconds> t("FFT Timer");
    fft_rec<false>(cvec);
  }
  // std::cout << "After FFT: " << cvec << "\n";
  {
    Timer<std::chrono::milliseconds> t("FFT Inverse Timer");
    fft_rec<true>(cvec);
  }
  // std::cout << "After FFT-inv: " << cvec << "\n";
  vec = to_reals(cvec);
  std::cout << "Final: " << std::vector(vec.begin(), vec.begin() + 16) << "\n";
  // std::cout << "Final: " << vec << "\n";
}

void test2d() {
  int n = 1 << 2;
  std::vector<std::vector<double>> mat(n, std::vector<double>(n));
  for (int i = 0; i < n; ++i) {
    std::iota(mat[i].begin(), mat[i].end(), i * n);
  }
  // std::cout << "Initial: " << std::vector(mat[0].begin(), mat[0].begin() +
  // 16) << "\n";
  std::cout << "Initial: " << mat << "\n";
  auto cmat = to_complex_2d(mat);
  // std::cout << "Before FFT: " << cmat << "\n";
  {
    Timer<std::chrono::milliseconds> t("FFT Timer");
    fft2d_iter<false>(cmat);
  }
  std::cout << "After FFT: " << cmat << "\n";
  {
    Timer<std::chrono::milliseconds> t("FFT Inverse Timer");
    fft2d_iter<true>(cmat);
  }
  // std::cout << "After FFT-inv: " << cmat << "\n";
  mat = to_reals_2d(cmat);
  // std::cout << "Final: " << std::vector(mat[0].begin(), mat[0].begin() + 16)
  // << "\n";
  std::cout << "Final: " << mat << "\n";
}

void threshold_cmat(std::vector<std::vector<std::complex<double>>> &cmat,
                    double frac) {
  assert(frac >= 0 and frac <= 1);
  std::vector<double> mags;
  int rows = cmat.size();
  int cols = cmat[0].size();
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      mags.push_back(std::abs(cmat[i][j]));
    }
  }
  int total = rows * cols;
  int n =
      std::clamp(static_cast<int>(std::round(frac * total) + 0.5), 0, total);
  if (n == total) {
    return;
  }
  std::nth_element(mags.begin(), mags.begin() + n, mags.end(),
                   std::greater<>());
  double mag_thresh = mags[n];
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (std::abs(cmat[i][j]) <= mag_thresh) {
        cmat[i][j] = 0;
      }
    }
  }
}

void compress(const std::string &in, const std::string &out, double frac) {
  auto img = read_ppm(in);
  int num_rows = img.mat[0].size();
  int num_cols = img.mat[0][0].size();
  std::vector<std::vector<double>> rgb[3];
  std::vector<std::vector<std::complex<double>>> crgb[3];
  for (int k = 0; k < 3; ++k) {
    rgb[k].resize(num_rows);
    for (int i = 0; i < num_rows; ++i) {
      rgb[k][i].resize(num_cols);
      for (int j = 0; j < num_cols; ++j) {
        rgb[k][i][j] = img.mat[k][i][j];
      }
    }
    crgb[k] = to_complex_2d(rgb[k]);
    fft2d_iter<false>(crgb[k]);
    threshold_cmat(crgb[k], frac);
    fft2d_iter<true>(crgb[k]);
    rgb[k] = to_reals_2d(crgb[k]);
    for (int i = 0; i < num_rows; ++i) {
      for (int j = 0; j < num_cols; ++j) {
        img.mat[k][i][j] = std::clamp(
            static_cast<int>(std::round(rgb[k][i][j]) + 0.5), 0, 255);
      }
    }
  }
  write_ppm(out, img);
}

int main() {
  // disable nested parallelism
  omp_set_max_active_levels(1);
  // double ratio = 0.995;
  // compress("images/test.ppm", "images/testout" + std::to_string(ratio) +
  // ".ppm", 1 - ratio);
  test1d();
  return 0;
}
