#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>
#include <vector>

int allSizes[5] = {1 << 10, 1 << 12, 1 << 15, 1 << 20, 1 << 25};

std::vector<double> generate(int size) {
  std::uniform_real_distribution<double> unif(-100, 100);
  std::default_random_engine re;
  std::vector<double> result(size);
  
  for (int i = 0; i < size; i++) {
    result.push_back(unif(re));
  }

  return result;
}

std::vector<double> loadFromFile(std::string fileName) {
  std::vector<double> values;
  std::ifstream inFile;
  inFile.open("./src/data/" + fileName);

  std::string line;
  while (std::getline(inFile, line)) {
    double temp;
    std::stringstream sstream(line);
    std::string str;
    std::getline(sstream, str, ' ');
    temp = (double)atof(str.c_str());
    values.push_back(temp);
  }
  inFile.close();
  return values;
}

void saveToFile(std::string fileName, std::vector<double> values) {
  std::ofstream file("./src/data/" + fileName);
  if (!file) {
    std::cout << "error writing file \"" << fileName << "\"" << std::endl;
    return;
  }
  for (double value : values) {
    file << value << std::endl;
  }
  file.close();
  if (!file)
    std::cout << "error writing file \"" << fileName << "\"" << std::endl;
}

int main() {
  // disable nested parallelism
  omp_set_max_active_levels(1);
  int currDataSet = 2;
  // int currThreadCount = 8; // Used only for generating and tracking output files
  int codeBlock = 1;       // which conditional code block to execute below
  // bool resultToFile = false;

  std::vector<double> input = generate(allSizes[currDataSet]);
      // loadFromFile("data_" + std::to_string(allSizes[currDataSet]) + ".txt");

  std::vector<std::complex<double>> newInput = to_complex(input);

  if (codeBlock == 0) {
    Timer<std::chrono::microseconds> t("Parallelized DFT");

    dft<false>(newInput);
    // dft<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_DFT_T" + std::to_string(currThreadCount) + ".txt",
    //              actual);

  } else if (codeBlock == 1) {
    Timer<std::chrono::microseconds> t(
        "Parallelized Recursive FFT With Threshold");

    fft_rec<false>(newInput);
    // fft_rec<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_FFT_T" + std::to_string(currThreadCount) + ".txt",
    //              actual);
  } else if (codeBlock == 2) {
    Timer<std::chrono::microseconds> t(
        "Parallelized Iterative FFT");

    fft_iter<false>(newInput);
    // fft_iter<true>(newInput);
    // std::vector<double> actual = to_reals(newInput);

    // if (resultToFile)
    //   saveToFile("data_" + std::to_string(allSizes[currDataSet]) +
    //                  "_out_FFT_T" + std::to_string(currThreadCount) + ".txt",
    //              actual);
  }
  return 0;
}
