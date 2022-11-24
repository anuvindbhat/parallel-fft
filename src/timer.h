#ifndef TIMER_H
#define TIMER_H

#include <chrono>
#include <string>
#include <type_traits>

template <typename T> inline constexpr bool always_false = false;

template <typename T> class Timer {
public:
  Timer(std::string name)
      : m_name(std::move(name)), m_beg(std::chrono::steady_clock::now()) {}
  ~Timer() {
    using namespace std;
    auto end = chrono::steady_clock::now();
    auto dur = chrono::duration_cast<T>(end - m_beg);
    std::string suffix;
    if constexpr (std::is_same_v<T, chrono::nanoseconds>) {
      suffix = "ns";
    } else if constexpr (std::is_same_v<T, chrono::microseconds>) {
      suffix = "us";
    } else if constexpr (std::is_same_v<T, chrono::milliseconds>) {
      suffix = "ms";
    } else if constexpr (std::is_same_v<T, chrono::seconds>) {
      suffix = "s";
    } else {
      // static_assert false is apparently ill formed
      // https://stackoverflow.com/questions/53945490
      static_assert(always_false<T>, "Unsupported duration");
    }
    cout << m_name << ": " << dur.count() << " " << suffix << "\n";
  }

private:
  std::string m_name;
  std::chrono::time_point<std::chrono::steady_clock> m_beg;
};

#endif
