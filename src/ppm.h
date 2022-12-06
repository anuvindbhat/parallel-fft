#ifndef PPM_H
#define PPH_H

#include <cstdint>
#include <string>
#include <vector>

struct rgb_image {
  std::vector<std::vector<uint8_t>> mat[3];
};

rgb_image read_ppm(const std::string &file_name);
void write_ppm(const std::string &file_name, const rgb_image &img);

#endif
