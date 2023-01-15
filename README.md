# `std::experimental::simd` experiment

Task: for string S find first position, so next 14 symbols from this position in string S are **unique**.
Consider using `vcpkg install benchmark` to run.

## Mac air m2 (128-bit NEON)

| Benchmark                               | Time    | CPU     | Iterations |
|-----------------------------------------|---------|---------|------------|
| BM_isUniqueSimple                       | 2824 ns | 2824 ns | 237633     |
| BM_isUniqueSimd                         | 1337 ns | 1336 ns | 523353     |
| BM_isUniqueSimd_vectorAligned           | 1328 ns | 1328 ns | 522454     |
| BM_isUniqueSimd_overAligned             | 1276 ns | 1276 ns | 547598     |
| BM_isUniqueSimd_singleSimd              | 1926 ns | 1919 ns | 368998     |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 1742 ns | 1742 ns | 402762     |

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
