#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>
#include <random>
#include <fstream>

int allSizes[4] = {100, 10000, 250000, 1000000};

void generate() {
  for (int number : allSizes) {
    std::ofstream MyFile("./src/data/data_" + std::to_string(number) + ".txt");
    std::uniform_real_distribution<double> unif(-100, 100);
    std::default_random_engine re;
    for (int i = 0; i < number; i++) {
      MyFile << unif(re) << std::endl;
    }

    MyFile.close();
  }
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
  {
    Timer<std::chrono::milliseconds> t("One Dimensional DFT");

    // Parallilized DFT below

    std::vector<double> input = loadFromFile("data_10000.txt");

    std::vector<std::complex<double>> newInput = to_complex(input);


    std::vector<std::complex<double>> result = discreteFourierTransform(newInput);

    std::vector<double> actual = to_reals(discreteFourierTransform(result, true));

    saveToFile("data_10000_out.txt", actual);


    // Recursive FFT below

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
