# `std::experimental::simd` experiment

Task: for string S find first position, so next 14 symbols from this position in string S are **unique**.
Consider using `vcpkg install benchmark` to run.

### Mac air m2 (128-bit NEON)

| Benchmark | Time | CPU     | Iterations |
|-----------|------|---------|--------|
| BM_isUniqueSimple | 2824 ns | 2824 ns | 237633 |
| BM_isUniqueSimd | 1337 ns | 1336 ns | 523353 |
| BM_isUniqueSimd_vectorAligned | 1328 ns | 1328 ns | 522454 |
| BM_isUniqueSimd_overAligned | 1276 ns | 1276 ns | 547598 |
| BM_isUniqueSimd_singleSimd | 1926 ns | 1919 ns | 368998 |
| BM_isUniqueSimd_singleSimd_loopUnrolled  | 1742 ns | 1742 ns | 402762 |
