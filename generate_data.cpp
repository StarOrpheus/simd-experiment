#include <fstream>
#include <random>
#include <iostream>
#include <algorithm>

#include "commons.h"


namespace {

constexpr auto MINIMAL = 128;
constexpr auto RESULT_MULT = N / MINIMAL;

static_assert(N % MINIMAL == 0);

std::string genData(auto &randDevice) {
  std::uniform_int_distribution<int> d('a', 'z');
  std::string r;
  r.reserve(MINIMAL);
  for (size_t i = 0; i < MINIMAL; ++i)
    r += d(randDevice);
  return r;
}

} // namespace

int main() try {
  std::random_device randomDevice;
  std::mt19937_64 mt(randomDevice());

  std::string Result;

  while (true) {
    std::string D = genData(mt);
    if (simpleAlgo(D, simpleCheck) < MINIMAL)
      continue;
    Result.clear();
    for (size_t i = 0; i < RESULT_MULT; ++i) {
      Result += D;
    }
    if (simpleAlgo(Result, simpleCheck) >= N)
      break;
  }

  for (size_t i = 0; i < BATCH; ++i)
    Result.pop_back();
  for (uint8_t i = 0; i < BATCH; ++i)
    Result.push_back(static_cast<char>('a' + i));

  std::ofstream out;
  out.exceptions(std::ofstream::failbit | std::ofstream::badbit);
  out.open("result.txt");
  out.write(Result.data(), Result.size() + 1);
} catch (std::exception const& e) {
  std::cerr << "Exception: " << e.what() << std::endl;
  return -1;
}