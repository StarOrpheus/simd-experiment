# `std::experimental::simd` experiment

Task: for string S find first position, so next 14 symbols from this position in string S are **unique**.
Consider using `vcpkg install benchmark` to run.

## Mac air m2 (128-bit NEON)

| Benchmark                               | Time   | CPU    | Iterations | UserCounters... | 
|-----------------------------------------|--------|--------|------------|-----------------|
| BM_isUniqueSimple                       | 771 ns | 771 ns | 906513     | 128.674M/s      |
| BM_isUniqueSimd                         | 397 ns | 387 ns | 1853534    | 256.342M/s      |
| BM_isUniqueSimd_vectorAligned           | 376 ns | 376 ns | 1813871    | 263.576M/s      |
| BM_isUniqueSimd_overAligned             | 370 ns | 370 ns | 1892608    | 268.219M/s      |
| BM_isUniqueSimd_singleSimd              | 581 ns | 581 ns | 1205400    | 170.794M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 522 ns | 522 ns | 1337460    | 189.974M/s      |
| BM_isUniqueSimd_neon_intrinsics         | 188 ns | 188 ns | 3715085    | 526.189M/s      |
| BM_isUniqueSimd_simde_avx2              | 192 ns | 192 ns | 3650530    | 517.113M/s      |


## Dell xps 9980hk

#### GNU C++ stdlibc++

| Benchmark                               | Time   | CPU    | Iterations |
|-----------------------------------------|--------|--------|------------|
| BM_isUniqueSimple                       | 299 ns | 298 ns | 2221935    |
| BM_isUniqueSimd                         | 387 ns | 386 ns | 1704151    |
| BM_isUniqueSimd_vectorAligned           | 380 ns | 379 ns | 1837064    |
| BM_isUniqueSimd_overAligned             | 371 ns | 370 ns | 1840378    |
| BM_isUniqueSimd_singleSimd              | 292 ns | 292 ns | 2396412    |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 238 ns | 238 ns | 2930495    |
| BM_isUniqueSimd_avx256_intrinsics       | 249 ns | 249 ns | 2750788    |
| BM_isUniqueSimd_sse_intrinsics          | 259 ns | 259 ns | 2585488    |


#### Clang + libc++

| Benchmark                               | Time   | CPU    | Iterations |
|-----------------------------------------|--------|--------|------------|
| BM_isUniqueSimple                       | 912 ns | 909 ns | 762447     |
| BM_isUniqueSimd                         | 703 ns | 702 ns | 999047     |
| BM_isUniqueSimd_vectorAligned           | 700 ns | 698 ns | 1035542    |
| BM_isUniqueSimd_overAligned             | 684 ns | 683 ns | 1044962    |
| BM_isUniqueSimd_singleSimd              | 681 ns | 681 ns | 1014382    |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 469 ns | 469 ns | 1379867    |
| BM_isUniqueSimd_avx256_intrinsics       | 543 ns | 543 ns | 1190226    |
| BM_isUniqueSimd_sse_intrinsics          | 483 ns | 483 ns | 1451006    |
