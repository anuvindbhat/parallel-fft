#include "ppm.h"
#include <cassert>
#include <cstdint>
#include <fstream>
#include <string>
#include <vector>

rgb_image read_ppm(const std::string &file_name) {
  std::string ppm_type, width, height, channel_size;
  std::ifstream ifs(file_name, std::ios::in | std::ios::binary);
  ifs >> ppm_type >> width >> height >> channel_size;
  assert(ppm_type == "P6" and channel_size == "255");
  char dummy;
  ifs.read(&dummy, 1); // discard new line character
  int32_t num_rows = std::stoi(height);
  int32_t num_cols = std::stoi(width);
  rgb_image img;
  for (int32_t i = 0; i < 3; ++i) {
    img.mat[i].resize(num_rows);
    for (int32_t j = 0; j < num_rows; ++j) {
      img.mat[i][j].resize(num_cols);
    }
  }
  std::string pixel(3, '\0');
  for (int32_t i = 0; i < num_rows; ++i) {
    for (int32_t j = 0; j < num_cols; ++j) {
      ifs.read(pixel.data(), 3);
      for (int32_t k = 0; k < 3; ++k) {
        img.mat[k][i][j] = pixel[k];
      }
    }
  }
  ifs.close();
  return img;
}

void write_ppm(const std::string &file_name, const rgb_image &img) {
  int32_t height = img.mat[0].size();
  int32_t width = img.mat[0][0].size();
  std::ofstream ofs(file_name, std::ios::out | std::ios::binary);
  ofs << "P6\n";
  ofs << std::to_string(width) << " " << std::to_string(height) << "\n";
  ofs << "255\n";
  for (int32_t i = 0; i < height; ++i) {
    for (int32_t j = 0; j < width; ++j) {
      for (int32_t k = 0; k < 3; ++k) {
        ofs << img.mat[k][i][j];
      }
    }
  }
  ofs.close();
}
