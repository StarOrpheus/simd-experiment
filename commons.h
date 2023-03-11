#pragma once

#include <array>
#include <string>
#include <cstdint>

constexpr auto N = 128 * 1024;
constexpr auto BATCH = 14;
using ArrayT = std::array<std::uint8_t, 32>;

static_assert(BATCH <= 26);

constexpr bool simpleCheck(ArrayT const &s) noexcept {
  return std::all_of(s.begin(), s.end(),
                     [](auto x) { return (x & 0xfe) == 0; });
}

template <typename CheckT>
constexpr unsigned simpleAlgo(std::string const &s, CheckT &&check) noexcept {
  if (s.size() < BATCH)
    return s.size() + 1;
  alignas(32) std::array<uint8_t, 32> sum{};
  std::fill(std::begin(sum), std::end(sum), 0);
  for (size_t i = 0; i < BATCH; ++i)
    sum[s[i] - 'a']++;
  if (check(sum))
    return 0;

  for (size_t i = BATCH; i < s.size(); ++i) {
    sum[s[i] - 'a']++;
    sum[s[i - BATCH] - 'a']--;
    if (check(sum)) {
      return i - BATCH + 1;
    }
  }
  return s.size() + 1;
}