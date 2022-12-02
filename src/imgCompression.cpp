#include "imgCompression.h"
#include "fft2d.h"
#include "utils.h"
#include <fstream>
#include <iostream>
#include <numeric>
#include <omp.h>
#include <vector>
#include <algorithm>

using namespace std;

void loadImage(string fileName, vector<vector<double>> &red,
               vector<vector<double>> &blue, vector<vector<double>> &green,
               char *header) {
  // Only for 24 bit images (i.e. no
  // transparent backgrounds)

  ifstream inFile;
  inFile.open("./src/imgs/" + fileName);

  inFile.read(header, 54);

  signed int imgWidth = *(int *)&header[18];
  signed int imgHeight = *(int *)&header[22];

  int padding = (4 - ((imgWidth * 3) % 4)) % 4;

  short int bitsPerPixel = *(int *)&header[28];
  if (bitsPerPixel != 24) {
    cout << endl << "Image is not 24 bit colored" << endl;
    assert(bitsPerPixel == 24);
  }

  int colorDataSize = (((3 * imgWidth) + padding) * imgHeight); // B G R format, not R G B
  char *colorData = new char[colorDataSize];

  inFile.read(colorData, colorDataSize);

  inFile.close();

  int rowWidth = imgWidth * 3 + padding;
  int pixWidth = imgWidth * 3;

  for (int row = 0; row < imgHeight; row++) {
    vector<double> rowRed;
    vector<double> rowBlue;
    vector<double> rowGreen;
    for (int col = row * rowWidth; col < pixWidth + (row * rowWidth);
         col += 3) {
      rowBlue.push_back((double)colorData[col]);
      rowGreen.push_back((double)colorData[col + 1]);
      rowRed.push_back((double)colorData[col + 2]);
    }
    red.push_back(rowRed);
    blue.push_back(rowBlue);
    green.push_back(rowGreen);
  }
}

void saveImage(string fileName, vector<vector<double>> &red,
               vector<vector<double>> &blue, vector<vector<double>> &green,
               char *header) {
  // Only for 24 bit images (i.e. no transparent backgrounds), padding is not zeroed (it doesn't matter)

  ofstream oFile;
  oFile.open("./src/imgs/" + fileName);

  oFile.write(header, 54);

  signed int imgWidth = *(int *)&header[18];
  signed int imgHeight = *(int *)&header[22];

  int padding = (4 - ((imgWidth * 3) % 4)) % 4;


  short int bitsPerPixel = *(int *)&header[28];
  if (bitsPerPixel != 24) {
    cout << endl << "Image is not 24 bit colored" << endl;
    assert(bitsPerPixel == 24);
  }

  int colorDataSize = (((3 * imgWidth) + padding) * imgHeight); // B G R
  char *colorData = new char[colorDataSize];
  for (int i = 0; i < colorDataSize; i++) {
    colorData[i] = (char)0;
  }

  int rowWidth = imgWidth * 3 + padding;
  int pixWidth = imgWidth * 3;

  for (int row = 0; row < imgHeight; row++) {
    vector<double> rowRed = red[row];
    vector<double> rowBlue = blue[row];
    vector<double> rowGreen = green[row];
    
    int col;
    int colTracker = 0;
    for (col = row * rowWidth; col < pixWidth + (row * rowWidth);
         col += 3) {
      colorData[col] = (char)rowBlue[colTracker];
      colorData[col + 1] = (char)rowGreen[colTracker];
      colorData[col + 2] = (char)rowRed[colTracker];

      colTracker++;
    }
  }

  oFile.write(colorData, colorDataSize);
  oFile.close();
}

void imgCompress(string fileName, int compressionPercent) {

  vector<vector<double>> red;
  vector<vector<double>> blue;
  vector<vector<double>> green;

  char *header = new char[54];

  cout << compressionPercent; // for compiler

  loadImage(fileName, red, blue, green, header);

  // // perform 2d fft on red, blue, and green which will return to resRed, resBlue, resGreen.

  // vector<vector<complex<double>>> resRed;
  // vector<vector<complex<double>>> resBlue;
  // vector<vector<complex<double>>> resGreen;

  // vector<complex<double>> flatResRed;
  // vector<complex<double>> flatResBlue;
  // vector<complex<double>> flatResGreen;

  // unsigned long int flattenedSize = 0;
  
  // for (auto aVec : resRed) {
  //   flattenedSize += aVec.size();
  // }

  // flatResRed.reserve(flattenedSize);
  // flatResBlue.reserve(flattenedSize);
  // flatResGreen.reserve(flattenedSize);

  // // flattening
  // for (long unsigned int i = 0; i < resRed.size(); i++) {
  //   flatResRed.insert(flatResRed.end(), resRed[i].begin(), resRed[i].end());
  //   flatResBlue.insert(flatResBlue.end(), resBlue[i].begin(), resBlue[i].end());
  //   flatResGreen.insert(flatResGreen.end(), resGreen[i].begin(), resGreen[i].end());
  // }

  // vector<double> flatResRedMag;
  // vector<double> flatResBlueMag;
  // vector<double> flatResGreenMag;

  // flatResRedMag.reserve(flattenedSize);
  // flatResBlueMag.reserve(flattenedSize);
  // flatResGreenMag.reserve(flattenedSize);

  // for (long unsigned int i = 0; i < flattenedSize; i++) {
  //   flatResRedMag.push_back(std::abs(flatResRed[i]));
  //   flatResBlueMag.push_back(std::abs(flatResBlue[i]));
  //   flatResGreenMag.push_back(std::abs(flatResGreen[i]));
  // }

  // sort(flatResRedMag.begin(), flatResRedMag.end(), greater<double>());
  // sort(flatResBlueMag.begin(), flatResBlueMag.end(), greater<double>());
  // sort(flatResGreenMag.begin(), flatResGreenMag.end(), greater<double>());

  // int thresholdIndex = std::floor((compressionPercent/100.00) * flattenedSize);
  
  // double thresholdRed = flatResRedMag[thresholdIndex];
  // double thresholdBlue = flatResBlueMag[thresholdIndex];
  // double thresholdGreen = flatResGreenMag[thresholdIndex];

  // for (long unsigned int i = 0; i < resRed.size(); i++) {
  //   for (long unsigned int j = 0; j < resRed[0].size(); i++) {
  //     if (abs(resRed[i][j]) < thresholdRed) resRed[i][j] = complex((double)0);
  //     if (abs(resGreen[i][j]) < thresholdGreen) resGreen[i][j] = complex((double)0);
  //     if (abs(resBlue[i][j]) < thresholdBlue) resBlue[i][j] = complex((double)0);
  //   }
  // }

  // // perform inverse 2d fft on red, blue, and green which will return to inverseResRed, inverseResBlue, inverseResGreen.

  // vector<vector<double>> inverseResRed;
  // vector<vector<double>> inverseResBlue;
  // vector<vector<double>> inverseResGreen;

  // saveImage(fileName + "_result.bmp", inverseResRed, inverseResBlue, inverseResGreen, header);

  saveImage(fileName + "_result.bmp", red, blue, green, header);
}
