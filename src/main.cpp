#include <chrono>
#include <iostream>
#include "timer.h"

int main() {
    {
        Timer<std::chrono::milliseconds> t("Hello Timer");
        std::cout << "Hello World\n";
    }
    return 0;
}
