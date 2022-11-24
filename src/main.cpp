#include "timer.h"
#include <chrono>
#include <iostream>

int main() {
  {
    Timer<std::chrono::milliseconds> t("Hello Timer");
    std::cout << "Hello World\n";
  }
  return 0;
}
