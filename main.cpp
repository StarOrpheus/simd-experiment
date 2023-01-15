#include <iostream>
#include <string>
#include <algorithm>
#include <array>
#include <random>
#include <cassert>

#include <experimental/simd>

#include <benchmark/benchmark.h>

#ifdef ENABLE_AVX_INTRINSICS_TEST
  #include <immintrin.h>
#else
  #ifdef ENABLE_SSE_INTRINSICS_TEST
    #include <immintrin.h>
  #endif
#endif

namespace {

    constexpr auto N = 4 * 1024 * 1024;

    constexpr auto SEED = 2754;

    std::string genData(auto &randDevice) {
        std::uniform_int_distribution<char> d('a', 'z');
        std::string r;
        r.reserve(N);
        for (size_t i = 0; i < N; ++i)
            r += d(randDevice);
        return r;
    }

    using ArrayT = std::array<uint8_t, 32>;

    bool simpleCheck(ArrayT const &s) {
        return std::all_of(s.begin(), s.end(),
                           [](auto x) { return (x & 0xfe) == 0; });
    }

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

    template<typename CheckT>
    unsigned simpleAlgo(std::string const &s, CheckT &&check) {
        if (s.size() < 14)
            return s.size() + 1;
        alignas(32) std::array<uint8_t, 32> sum{};
        std::fill(std::begin(sum), std::end(sum), 0);
        for (size_t i = 0; i < 14; ++i)
            sum[s[i] - 'a']++;
        if (check(sum))
            return 0;

        for (size_t i = 14; i < s.size(); ++i) {
            sum[s[i] - 'a']++;
            sum[s[i - 14] - 'a']--;
            if (check(sum)) {
                return i - 14;
            }
        }
        return 0;
    }

    unsigned simdAlgo(std::string const &s) {
        std::experimental::fixed_size_simd<uint8_t, 26> vu;

        for (size_t i = 0; i < vu.size(); ++i)
            vu[i] = 0;

        for (size_t i = 0; i < 14; ++i)
            vu[s[i] - 'a']++;

        bool ok = true;
        for (size_t i = 0; i < vu.size() && ok; ++i)
            if ((vu[i] & 0xfe) != 0)
                ok = false;
        if (ok)
            return 0;

        for (size_t i = 14; i < s.size(); ++i) {
            vu[s[i] - 'a']++;
            vu[s[i - 14] - 'a']--;

            ok = true;
            for (size_t j = 0; j < vu.size() && ok; ++j)
                if ((vu[j] & 0xfe) != 0)
                    ok = false;

            if (ok)
                return i - 14;
        }

        return 0;
    }

    unsigned simdAlgoUnrolled(std::string const &s) {
        std::experimental::fixed_size_simd<uint8_t, 26> vu;

        for (size_t i = 0; i < vu.size(); ++i)
            vu[i] = 0;

        for (size_t i = 0; i < 14; ++i)
            vu[s[i] - 'a']++;

        bool ok = true;
        for (size_t i = 0; i < vu.size() && ok; ++i)
            if ((vu[i] & 0xfe) != 0)
                ok = false;
        if (ok)
            return 0;

#define singleStep(m)                                                           \
            vu[s[i + (m)] - 'a']++;                                             \
            vu[s[i + (m) - 14] - 'a']--;                                        \
            ok = true;                                                          \
            for (size_t j = 0; j < vu.size() && ok; ++j)                        \
                if ((vu[j] & 0xfe) != 0)                                        \
                    ok = false;                                                 \
            if (ok)                                                             \
                return i + (m) - 14;

        for (size_t i = 14; i + 7 < s.size(); i += 8) {
            singleStep(0)
            singleStep(1)
            singleStep(2)
            singleStep(3)
            singleStep(4)
            singleStep(5)
            singleStep(6)
            singleStep(7)
        }

        for (size_t i = s.size() - (s.size() % 4); i < s.size(); ++i) {
            singleStep(0)
        }
#undef singleStep

        return 0;
    }

    void BM_isUniqueSimple(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);
        for (auto _: state) {
            benchmark::DoNotOptimize(simpleAlgo(s, simpleCheck));
        }
    }

    BENCHMARK(BM_isUniqueSimple);

    void BM_isUniqueSimd(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);
        for (auto _: state) {
            assert(simpleAlgo(s, simdCheck) == simpleAlgo(s, simpleCheck));
            benchmark::DoNotOptimize(simpleAlgo(s, simdCheck));
        }
    }

    BENCHMARK(BM_isUniqueSimd);

    void BM_isUniqueSimd_vectorAligned(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);

        for (auto _: state) {
            assert(simpleAlgo(s, simdCheckOverAligned) == simpleAlgo(s, simpleCheck));
            benchmark::DoNotOptimize(simpleAlgo(s, simdCheckVectorAligned));
        }
    }

    BENCHMARK(BM_isUniqueSimd_vectorAligned);

    void BM_isUniqueSimd_overAligned(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);

        for (auto _: state) {
            assert(simpleAlgo(s, simdCheckOverAligned) == simpleAlgo(s, simpleCheck));
            benchmark::DoNotOptimize(simpleAlgo(s, simdCheckOverAligned));
        }
    }

    BENCHMARK(BM_isUniqueSimd_overAligned);

    void BM_isUniqueSimd_singleSimd(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);

        for (auto _: state) {
            assert(simdAlgo(s) == simpleAlgo(s, simpleCheck));
            benchmark::DoNotOptimize(simdAlgo(s));
        }
    }

    BENCHMARK(BM_isUniqueSimd_singleSimd);

    void BM_isUniqueSimd_singleSimd_loopUnrolled(benchmark::State &state) {
        std::mt19937 rd(SEED);
        auto s = genData(rd);

        for (auto _: state) {
            assert(simdAlgoUnrolled(s) == simpleAlgo(s, simpleCheck));
            benchmark::DoNotOptimize(simdAlgoUnrolled(s));
        }
    }

    BENCHMARK(BM_isUniqueSimd_singleSimd_loopUnrolled);

#ifdef ENABLE_AVX_INTRINSICS_TEST
  bool simdCheckIntrinsicsAVX(ArrayT const& s) {
    auto V1 = _mm256_loadu_si256(reinterpret_cast<__m256i_u const*>(s.data()));
    uint32_t Mask = 0x01010101;
    auto MaskV = _mm256_broadcastd_epi32(_mm_loadu_si32(&Mask));
    auto Result =  !!_mm256_testc_si256(MaskV, V1);

    assert(Result == simpleCheck(s));
    return Result;
  }

  void BM_isUniqueSimd_avx256_intrinsics(benchmark::State &state) {
    std::mt19937 rd(SEED);
    auto s = genData(rd);
    for (auto _: state) {
      benchmark::DoNotOptimize(simpleAlgo(s, simdCheckIntrinsicsAVX));
    }
  }

  BENCHMARK(BM_isUniqueSimd_avx256_intrinsics);
#endif

#ifdef ENABLE_SSE_INTRINSICS_TEST
  bool simdCheckIntrinsicsSSE(ArrayT const& s) {
    auto V1 = _mm_load_si128(reinterpret_cast<__m128i const*>(s.data()));
    uint32_t Mask = 0x01010101;
    auto MaskV = _mm_broadcastd_epi32(_mm_loadu_si32(&Mask));
    auto Result =  !!_mm_testc_si128(MaskV, V1);

    V1 = _mm_load_si128(reinterpret_cast<__m128i const*>(s.data() + 16));
    Result &= !!_mm_testc_si128(MaskV, V1);

    assert(Result == simpleCheck(s));
    return Result;
  }

  void BM_isUniqueSimd_sse_intrinsics(benchmark::State &state) {
    std::mt19937 rd(SEED);
    auto s = genData(rd);
    for (auto _: state) {
      benchmark::DoNotOptimize(simpleAlgo(s, simdCheckIntrinsicsSSE));
    }
  }

  BENCHMARK(BM_isUniqueSimd_sse_intrinsics);
#endif
}

BENCHMARK_MAIN();