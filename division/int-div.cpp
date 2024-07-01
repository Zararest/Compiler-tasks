#include <iostream>
#include <cstdint>
#include <limits>

template <typename T>
struct extender {
  using type = void;
};

template <>
struct extender<uint32_t> {
  using type = uint64_t;
};

template <>
struct extender<uint16_t> {
  using type = uint32_t;
};

template <>
struct extender<uint8_t> {
  using type = uint16_t;
};

template <typename uint_t>
consteval size_t bitSize() {
  constexpr auto CharBits = 8u;
  return sizeof(uint_t) * CharBits;
}

template <typename uint_t>
uint_t div(uint_t X, uint_t Y) {
  uint_t Res = 0u;
  constexpr auto BitSize = bitSize<uint_t>();
  for (size_t i = 0; i < BitSize; ++i) { 
    size_t CurDigit = BitSize - 1 - i;
    typename extender<uint_t>::type Z = Y << CurDigit;
    if (Z <= X) {
      Res += (1 << CurDigit);
      X -= Z;
    }
    
  }
  return Res;
}

void test() {
  using uint_t = uint8_t;
  for (uint_t X = 0; X < std::numeric_limits<uint_t>::max(); ++X)
    for (uint_t Y = 1; Y < std::numeric_limits<uint_t>::max(); ++Y)
      if (div(X, Y) != (X / Y)) {
        std::cout << "Invalid division: " << X << " / " << Y << " == "
                  << div(X, Y) << std::endl;
        exit(1);
      }
}

int main() {
  std::cout << div<uint32_t>(10, 5) << std::endl;
  test();
  std::cout << "All tests have passed" << std::endl;
}