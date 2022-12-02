#include "fft1d.h"
#include "timer.h"
#include "utils.h"
#include "imgCompression.h"
#include <chrono>
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <random>
#include <vector>
#include<string.h>

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

std::vector<std::vector<double>> generate2D(long unsigned int size) {
  std::vector<std::vector<double>> result;
  
  for (long unsigned int j = 0; j < size; j++) {
    result.push_back(generate(size, j));
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

void saveToFile(std::string fileName, std::vector<std::vector<double>> values) {
  std::ofstream file("./src/data/" + fileName);
  if (!file) {
    std::cout << "error writing file \"" << fileName << "\"" << std::endl;
    return;
  }
  for (long unsigned int i = 0; i < values.size(); i++) {
    for (std::vector<double> value : values) {
      file << value[i] << ", ";
    }
    file << std::endl;
  }
  file.close();
  if (!file)
    std::cout << "error writing file \"" << fileName << "\"" << std::endl;
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
  // int currThreadCount = 8; // Used only for generating and tracking output files
  int codeBlock = 3;       // which conditional code block to execute below
  // bool resultToFile = false;

  std::vector<double> input = generate(allSizes[currDataSet], 0);
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
  } else if (codeBlock == 3) {
    // below are all files if mass testing is required
    // imgCompress("threeBlue.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("7x5Blue.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("5x7Blue.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("car.bmp", 10);
    //  std::cout << std::endl;
    // imgCompress("4x4green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("10green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("50green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("74green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("90green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("100green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("100x2green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("100x10green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("1080green.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("red.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("sevenBlue.bmp", 10);
    // std::cout << std::endl;
    // imgCompress("threeBlue.bmp", 10);

    imgCompress(options.inputFile, options.precentCompression);
  }
  return 0;
}
