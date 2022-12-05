#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>
#include <string.h>
#include <vector>

int allSizes[5] = {1 << 10, 1 << 12, 1 << 15, 1 << 20, 1 << 25};

std::vector<double> generate(int size, int seed) {
  std::uniform_real_distribution<double> unif(-100, 100);
  std::default_random_engine re;
  re.seed(re.default_seed + seed);
  std::vector<double> result;

  for (int i = 0; i < size; i++) {
    result.push_back(unif(re));
  }

  return result;
}

struct StartupOptions {
  int precentCompression = 10;
  std::string inputFile = "car.bmp";
};

StartupOptions parseOptions(int argc, const char **argv) {
  StartupOptions rs;
  for (int i = 1; i < argc; i++) {
    if (i < argc - 1) {
      if (strcmp(argv[i], "-p") == 0)
        rs.precentCompression = atoi(argv[i + 1]);
      else if (strcmp(argv[i], "-in") == 0)
        rs.inputFile = argv[i + 1];
    }
  }
  return rs;
}

int main(int argc, const char **argv) {

  StartupOptions options = parseOptions(argc, argv);

  // disable nested parallelism
  omp_set_max_active_levels(1);
  int currDataSet = 2;
  // int currThreadCount = 8; // Used only for generating and tracking output
  // files
  int codeBlock = 0; // which conditional code block to execute below
  // bool resultToFile = false;

  std::vector<double> input = generate(allSizes[currDataSet], 0);
  // loadFromFile("data_" + std::to_string(allSizes[currDataSet]) + ".txt");

  std::vector<std::complex<double>> newInput = to_complex(input);

  if (codeBlock == 0) {
    Timer<std::chrono::milliseconds> t("Parallelized DFT");

    dft<false>(newInput);
    // dft<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_DFT_T" + std::to_string(currThreadCount) +
    //                  ".txt",
    //              actual);

  } else if (codeBlock == 1) {
    Timer<std::chrono::milliseconds> t(
        "Parallelized Recursive FFT With Threshold");

    fft_rec<false>(newInput);
    // fft_rec<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_FFT_T" + std::to_string(currThreadCount) +
    //                  ".txt",
    //              actual);
  } else if (codeBlock == 2) {
    Timer<std::chrono::milliseconds> t("Parallelized Iterative FFT");

    fft_iter<false>(newInput);
    // fft_iter<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_FFT_T" + std::to_string(currThreadCount) +
    //                  ".txt",
    //              actual);
  }
  return 0;
}
