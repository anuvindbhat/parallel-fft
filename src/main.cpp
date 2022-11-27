#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include <chrono>
#include <iostream>
#include <numeric>
#include <vector>
#include <random>
#include <fstream>

int allSizes[8] = {1 << 3, 1 << 10, 1 << 12, 1 << 14, 1 << 15, 
                    1 << 17, 1 << 19, 1 << 20}; // 3 data sets for FFT only


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
  int currDataSet = 4;
  int currThreadCount = 8; // Used only for generating and tracking output files
  int codeBlock = 0; // which conditional code block to execute below
  bool resultToFile = false;

  std::vector<double> input = loadFromFile("data_" + std::to_string(allSizes[currDataSet]) + ".txt");

  std::vector<std::complex<double>> newInput = to_complex(input);

  if (codeBlock == 0) {
    Timer<std::chrono::milliseconds> t("Parallelized DFT");

    discreteFourierTransform(newInput);
    discreteFourierTransform(newInput, true);
    std::vector<double> actual = to_reals(newInput);

    if (resultToFile) saveToFile("data_" + std::to_string(allSizes[currDataSet]) + "_out_DFT_T" + std::to_string(currThreadCount) + ".txt", actual);


  } else if (codeBlock ==  1) {
    Timer<std::chrono::milliseconds> t("Parallelized Recursive FFT With Threshold");

    fft_rec<false>(newInput);
    fft_rec<true>(newInput);
    std::vector<double> actual = to_reals(newInput);

    if (resultToFile) saveToFile("data_" + std::to_string(allSizes[currDataSet]) + "_out_FFT_T" + std::to_string(currThreadCount) + ".txt", actual);
  }
  return 0;
}
