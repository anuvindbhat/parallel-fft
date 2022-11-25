#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <iostream>

int main() {
  {
    Timer<std::chrono::milliseconds> t("Hello Timer");
    std::vector<double> vec = {1, 2, 3, 4};
    std::cout << vec << "\n";
    auto cvec = to_complex(vec);
    std::cout << "Before FFT: " << cvec << "\n";
    fft_rec<false>(cvec);
    std::cout << "After FFT: " << cvec << "\n";
    fft_rec<true>(cvec);
    std::cout << "After FFT-inv: " << cvec << "\n";
    vec = to_reals(cvec);
    std::cout << "Final: " << vec << "\n";
  }
  return 0;
}
