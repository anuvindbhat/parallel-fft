#include "ppm.h"
#include <string>
#include <vector>
#include <cstdint>
#include <cassert>
#include <fstream>

rgb_image read_ppm(const std::string &file_name) {
  std::string ppm_type, width, height, channel_size;
  std::ifstream ifs(file_name, std::ios::in | std::ios::binary);
  ifs >> ppm_type >> width >> height >> channel_size;
  assert(ppm_type == "P6" and channel_size == "255");
  char dummy;
  ifs.read(&dummy, 1); // discard new line character
  int num_rows = std::stoi(height);
  int num_cols = std::stoi(width);
  rgb_image img;
  for (int i = 0; i < 3; ++i) {
    img.mat[i].resize(num_rows);
    for (int j = 0; j < num_rows; ++j) {
      img.mat[i][j].resize(num_cols);
    }
  }
  std::string pixel(3, '\0');
  for (int i = 0; i < num_rows; ++i) {
    for (int j = 0; j < num_cols; ++j) {
      ifs.read(pixel.data(), 3);
      for (int k = 0; k < 3; ++k) {
        img.mat[k][i][j] = pixel[k];
      }
    }
  }
  ifs.close();
  return img;
}

void write_ppm(const std::string &file_name, const rgb_image &img) {
  int height = img.mat[0].size();
  int width = img.mat[0][0].size();
  std::ofstream ofs(file_name, std::ios::out | std::ios::binary);
  ofs << "P6\n";
  ofs << std::to_string(width) << " " << std::to_string(height) << "\n";
  ofs << "255\n";
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      for (int k = 0; k < 3; ++k) {
        ofs << img.mat[k][i][j];
      }
    }
  }
  ofs.close();
}
