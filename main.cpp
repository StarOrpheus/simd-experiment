#include <algorithm>
#include <array>
#include <cassert>
#include <fstream>
#include <iostream>
#include <optional>
#include <random>
#include <string>

#include "commons.h"

#ifndef WIN32
#include <experimental/simd>
#endif

#include <benchmark/benchmark.h>

#ifdef __AVX2__
#define ENABLE_AVX_INTRINSICS_TEST
#include <immintrin.h>
#endif

#ifdef __SSE__
#define ENABLE_SSE_INTRINSICS_TEST
#include <immintrin.h>
#endif

#ifdef __ARM_NEON
#define ENABLE_NEON_INTRINSICS_TEST
#include <arm_neon.h>
#endif

#include "simde/x86/avx2.h"
#include "simde/x86/sse4.2.h"

namespace {

std::optional<std::string> loadData() {
  std::ifstream in;
  in.exceptions(std::ifstream::badbit | std::ifstream::failbit);
  std::string R;
  R.resize(N);
  try {
    in.open("result.txt");
    in.read(R.data(), N + 1); // 1 for \0
  } catch (std::exception const &e) {
    std::cerr << "Exception: " << e.what() << std::endl;
  }

  if (R.size() == N)
    return {std::move(R)};

  return std::nullopt;
}

#ifndef WIN32
bool simdCheckO(ArrayT const &s, auto &&flag) {
  std::experimental::fixed_size_simd<uint8_t, 32> values;
  values.copy_from(s.data(), flag);
  uint8_t mask = 0xfe;

  for (size_t j = 0; j < values.size(); ++j) {
    if ((values[j] & mask) != 0) {
      assert(!simpleCheck(s));
      return false;
    }
  }
  assert(simpleCheck(s));
  return true;
}

bool simdCheck(ArrayT const &s) {
  return simdCheckO(s, std::experimental::element_aligned);
}

bool simdCheckVectorAligned(ArrayT const &s) {
  return simdCheckO(s, std::experimental::vector_aligned);
}

bool simdCheckOverAligned(ArrayT const &s) {
  return simdCheckO(s, std::experimental::overaligned<32>);
}
#endif

template <typename F> void doBenchmark(benchmark::State &state, F &&func) {
  auto s = loadData();
  if (!s) {
    state.SetLabel("NO DATA");
    return;
  }
  auto &str = *s;
  unsigned R = 0xfefefefe;
  for (auto _ : state) {
    benchmark::DoNotOptimize(R = func(str));
  }
  assert(R == simpleAlgo(str, simpleCheck));
  state.SetBytesProcessed(R * state.iterations());
}

#ifndef WIN32
unsigned simdAlgo(std::string const &s) {
  std::experimental::fixed_size_simd<uint8_t, 26> vu;

  for (size_t i = 0; i < vu.size(); ++i)
    vu[i] = 0;

  for (size_t i = 0; i < BATCH; ++i)
    vu[s[i] - 'a']++;

  bool ok = true;
  for (size_t i = 0; i < vu.size() && ok; ++i)
    if ((vu[i] & 0xfe) != 0)
      ok = false;
  if (ok)
    return s.size() + 1;

  for (size_t i = BATCH; i < s.size(); ++i) {
    vu[s[i] - 'a']++;
    vu[s[i - BATCH] - 'a']--;

    ok = true;
    for (size_t j = 0; j < vu.size() && ok; ++j)
      if ((vu[j] & 0xfe) != 0)
        ok = false;

    if (ok)
      return i - BATCH + 1;
  }

  return s.size() + 1;
}

unsigned simdAlgoUnrolled(std::string const &s) {
  std::experimental::fixed_size_simd<uint8_t, 26> vu;

  if (s.size() < BATCH)
    return s.size() + 1;

  for (size_t i = 0; i < vu.size(); ++i)
    vu[i] = 0;

  for (size_t i = 0; i < BATCH; ++i)
    vu[s[i] - 'a']++;

  bool ok = true;
  for (size_t i = 0; i < vu.size() && ok; ++i)
    if ((vu[i] & 0xfe) != 0)
      ok = false;
  if (ok)
    return 0;

#define singleStep(m)                                                          \
  do {                                                                         \
    vu[s[i + m] - 'a']++;                                                      \
    vu[s[i + m - BATCH] - 'a']--;                                              \
    ok = true;                                                                 \
    for (size_t j = 0; j < vu.size(); ++j)                                     \
      if ((vu[j] & 0xfe) != 0)                                                 \
        ok = false;                                                            \
    if (ok)                                                                    \
      return i + m - BATCH + 1;                                                \
  } while (false)

  unsigned last = 0;
  for (size_t i = BATCH; i + 7 < s.size(); i += 8) {
    singleStep(0);
    singleStep(1);
    singleStep(2);
    singleStep(3);
    singleStep(4);
    singleStep(5);
    singleStep(6);
    singleStep(7);
    last = i + 7;
  }

  for (size_t i = last + 1; i < s.size(); ++i) {
    singleStep(0);
  }
#undef singleStep

  return s.size() + 1;
}
#endif

void BM_isUniqueSimple(benchmark::State &state) {
  doBenchmark(state, [](std::string const &str) {
    return simpleAlgo(str, simpleCheck);
  });
}

BENCHMARK(BM_isUniqueSimple);

#ifndef WIN32
void BM_isUniqueSimd(benchmark::State &state) {
  doBenchmark(
      state, [](std::string const &str) { return simpleAlgo(str, simdCheck); });
}

BENCHMARK(BM_isUniqueSimd);

void BM_isUniqueSimd_vectorAligned(benchmark::State &state) {
  doBenchmark(state, [](std::string const &str) {
    return simpleAlgo(str, simdCheckVectorAligned);
  });
}

BENCHMARK(BM_isUniqueSimd_vectorAligned);

void BM_isUniqueSimd_overAligned(benchmark::State &state) {
  doBenchmark(state, [](std::string const &str) {
    return simpleAlgo(str, simdCheckOverAligned);
  });
}

BENCHMARK(BM_isUniqueSimd_overAligned);

void BM_isUniqueSimd_singleSimd(benchmark::State &state) {
  doBenchmark(state, [](std::string const &str) { return simdAlgo(str); });
}

BENCHMARK(BM_isUniqueSimd_singleSimd);

void BM_isUniqueSimd_singleSimd_loopUnrolled(benchmark::State &state) {
  doBenchmark(state,
              [](std::string const &str) { return simdAlgoUnrolled(str); });
}

BENCHMARK(BM_isUniqueSimd_singleSimd_loopUnrolled);

#endif

#ifdef ENABLE_AVX_INTRINSICS_TEST
bool simdCheckIntrinsicsAVX(ArrayT const &s) {
  alignas(32) constexpr static uint64_t Mask[] = {
      0x0101010101010101, 0x0101010101010101, 0x0101010101010101,
      0x0101010101010101};

  auto V1 = _mm256_load_si256(reinterpret_cast<const __m256i *>(s.data()));
  auto MaskV = _mm256_load_si256(reinterpret_cast<const __m256i *>(Mask));
  auto Result = !!_mm256_testc_si256(MaskV, V1);

  assert(Result == simpleCheck(s));
  return Result;
}

void BM_isUniqueSimd_avx256_intrinsics(benchmark::State &state) {
  doBenchmark(state, [](auto &&str) {
    return simpleAlgo(str, simdCheckIntrinsicsAVX);
  });
}

BENCHMARK(BM_isUniqueSimd_avx256_intrinsics);
#endif

#ifdef ENABLE_SSE_INTRINSICS_TEST
bool simdCheckIntrinsicsSSE(ArrayT const &s) {
  alignas(16) constexpr static uint64_t Mask[] = {0x0101010101010101,
                                                  0x0101010101010101};

  auto V1 = _mm_load_si128(reinterpret_cast<__m128i const *>(s.data()));
  auto MaskV = _mm_load_si128(reinterpret_cast<const __m128i *>(Mask));
  auto Result = !!_mm_testc_si128(MaskV, V1);

  V1 = _mm_load_si128(reinterpret_cast<__m128i const *>(s.data() + 16));
  Result &= !!_mm_testc_si128(MaskV, V1);

  assert(Result == simpleCheck(s));
  return Result;
}

void BM_isUniqueSimd_sse_intrinsics(benchmark::State &state) {
  doBenchmark(state, [](auto &&str) {
    return simpleAlgo(str, simdCheckIntrinsicsSSE);
  });
}

BENCHMARK(BM_isUniqueSimd_sse_intrinsics);
#endif

#ifdef ENABLE_NEON_INTRINSICS_TEST

bool simdCheckIntrinsicsNeon(ArrayT const &s) {
  constexpr static uint8_t Mask = 0xfe;

  auto x = vld1q_u8(s.data());
  auto y = vld1q_u8(s.data() + 16);
  x = vorrq_u8(x, y);
  y = vld1q_dup_u8(&Mask);
  x = vandq_u8(x, y);

  auto v = vreinterpret_u32_u8(vorr_u8(vget_low_u8(x), vget_high_u8(x)));
  auto R = vget_lane_u32(vpmax_u32(v, v), 0) == 0;
  assert(R == simpleCheck(s));
  return R;
}

void BM_isUniqueSimd_neon_intrinsics(benchmark::State &state) {
  doBenchmark(state, [](auto &&str) {
    return simpleAlgo(str, simdCheckIntrinsicsNeon);
  });
}

BENCHMARK(BM_isUniqueSimd_neon_intrinsics);

#endif

bool simdCheckSimdeAvx2(ArrayT const &s) {
  alignas(32) constexpr static uint64_t Mask[] = {
      0x0101010101010101, 0x0101010101010101, 0x0101010101010101,
      0x0101010101010101};

  auto V1 =
      simde_mm256_loadu_si256(reinterpret_cast<simde__m256i const *>(s.data()));
  auto MaskV =
      simde_mm256_load_si256(reinterpret_cast<const simde__m256i *>(Mask));
  auto Result = !!simde_mm256_testc_si256(MaskV, V1);

  assert(Result == simpleCheck(s));
  return Result;
}

void BM_isUniqueSimd_simde_avx2(benchmark::State &state) {
  doBenchmark(state,
              [](auto &&str) { return simpleAlgo(str, simdCheckSimdeAvx2); });
}

BENCHMARK(BM_isUniqueSimd_simde_avx2);

} // namespace