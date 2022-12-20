#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <iostream>
#include <string>
#include <type_traits>

template <typename T> inline constexpr bool always_false = false;

template <typename T> class Timer {
public:
  Timer(std::string name)
      : m_name(std::move(name)), m_beg(std::chrono::steady_clock::now()) {}
  ~Timer() {
    auto end = std::chrono::steady_clock::now();
    auto dur = std::chrono::duration_cast<T>(end - m_beg);
    std::string suffix;
    if constexpr (std::is_same_v<T, std::chrono::nanoseconds>) {
      suffix = "ns";
    } else if constexpr (std::is_same_v<T, std::chrono::microseconds>) {
      suffix = "us";
    } else if constexpr (std::is_same_v<T, std::chrono::milliseconds>) {
      suffix = "ms";
    } else if constexpr (std::is_same_v<T, std::chrono::seconds>) {
      suffix = "s";
    } else {
      // static_assert false inside if constexpr is apparently ill formed
      // https://stackoverflow.com/questions/53945490
      static_assert(always_false<T>, "Unsupported duration");
    }
    std::cout << m_name << ": " << dur.count() << " " << suffix << "\n";
  }

private:
  std::string m_name;
  std::chrono::time_point<std::chrono::steady_clock> m_beg;
};

#endif
