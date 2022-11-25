#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>

int main() {
  {
    Timer<std::chrono::milliseconds> t("Hello Timer");
    int n = 1 << 20;
    std::vector<double> vec(n);
    std::iota(vec.begin(), vec.end(), 0);
    std::cout << "Initial: " << std::vector(vec.begin(), vec.begin() + 10)
              << "\n";
    auto cvec = to_complex(vec);
    // std::cout << "Before FFT: " << cvec << "\n";
    fft_rec<false>(cvec);
    // std::cout << "After FFT: " << cvec << "\n";
    fft_rec<true>(cvec);
    // std::cout << "After FFT-inv: " << cvec << "\n";
    vec = to_reals(cvec);
    std::cout << "Final: " << std::vector(vec.begin(), vec.begin() + 10)
              << "\n";
  }
  return 0;
}
