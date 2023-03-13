# `std::experimental::simd` experiment

Task: for string S find first position, so next 14 symbols from this position in string S are **unique**.

## Benchmark description
| Benchmark                                  | Desciption                                                                                                            |
|--------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| BM_isUniqueSimple                          | Naive implementation, often well-optimized by compilers as is                                                         |
| BM_isUniqueSimple_unrolled                 | Same algo with explicit main loop unrolling                                                                           | 
| BM_isUniqueSimd                            | Naive implementation with `std::experimental::simd`                                                                   | 
| BM_isUniqueSimd_vectorAligned              | Same algo, but uses `std::experimental::vector_aligned`                                                               |
| BM_isUniqueSimd_overAligned                | Same algo, but uses `std::experimental::over_aligned`                                                                 |
| BM_isUniqueSimd_singleSimd                 | Uses single `std::experimental::simd` variable as array during                                                        | 
| BM_isUniqueSimd_singleSimd_loopUnrolled    | Same algo, main loop unrolled                                                                                         |
| BM_isUniqueSimd_avx256_intrinsics          | Simple algo that uses check-function written on **AVX2** intrinsics                                                   |
| BM_isUniqueSimd_avx256_intrinsics_unrolled | Same algo, main loop unrolled                                                                                         | 
| BM_isUniqueSimd_sse_intrinsics             | Simple algo that uses check-function written on **SSE** intrinsics                                                    | 
| BM_isUniqueSimd_sse_intrinsics_unrolled    | Same algo, main loop unrolled                                                                                         |
| BM_isUniqueSimd_neon_intrinsics            | Simple algo that uses check-function written on **NEON** intrinsics                                                   |
| BM_isUniqueSimd_simde_avx2                 | Uses [simde](https://github.com/simd-everywhere/simde) -- a cross-platform implementaion of avx2, avx, sse intrinsics | 
| BM_isUniqueSimd_simde_avx2_unrolled        | Same algo, main loop unrolled                                                                                         |


## Dell XPS 9980HK

#### clang + libc++

| Benchmark                               | Time      | CPU       | Iterations | UserCounters... |
|-----------------------------------------|-----------|-----------|------------|-----------------|
| BM_isUniqueSimple                       | 740931 ns | 739695 ns | 883        | 168.97M/s       |
| BM_isUniqueSimd                         | 882380 ns | 880121 ns | 762        | 142.011M/s      |
| BM_isUniqueSimd_vectorAligned           | 853987 ns | 852266 ns | 813        | 146.652M/s      |
| BM_isUniqueSimd_overAligned             | 851090 ns | 849997 ns | 840        | 147.044M/s      |
| BM_isUniqueSimd_singleSimd              | 709946 ns | 708973 ns | 982        | 176.293M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 554343 ns | 553906 ns | 1228       | 225.646M/s      |
| BM_isUniqueSimd_avx256_intrinsics       | 701450 ns | 700319 ns | 977        | 178.471M/s      |
| BM_isUniqueSimd_sse_intrinsics          | 635244 ns | 634152 ns | 1029       | 197.093M/s      |
| BM_isUniqueSimd_simde_avx2              | 696675 ns | 695728 ns | 984        | 179.649M/s      |

#### clang + libc++ with PGO & LTO

| Benchmark                               | Time      | CPU       | Iterations | UserCounters... |
|-----------------------------------------|-----------|-----------|------------|-----------------|
| BM_isUniqueSimple                       | 654181 ns | 653163 ns | 1147       | 191.356M/s      |
| BM_isUniqueSimd                         | 669992 ns | 668978 ns | 1051       | 186.832M/s      |
| BM_isUniqueSimd_vectorAligned           | 704174 ns | 703034 ns | 951        | 177.782M/s      |
| BM_isUniqueSimd_overAligned             | 663965 ns | 662862 ns | 1078       | 188.556M/s      |
| BM_isUniqueSimd_singleSimd              | 698118 ns | 697639 ns | 965        | 179.157M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 527081 ns | 526851 ns | 1279       | 237.234M/s      |
| BM_isUniqueSimd_avx256_intrinsics       | 663761 ns | 663269 ns | 1010       | 188.44M/s       |
| BM_isUniqueSimd_sse_intrinsics          | 630535 ns | 630210 ns | 1047       | 198.325M/s      |
| BM_isUniqueSimd_simde_avx2              | 653591 ns | 653262 ns | 1000       | 191.327M/s      |

#### GCC + stdlibc++

| Benchmark                               | Time       | CPU        | Iterations | UserCounters... |
|-----------------------------------------|------------|------------|------------|-----------------|
| BM_isUniqueSimple                       | 964821 ns  | 964070 ns  | 718        | 129.645M/s      |
| BM_isUniqueSimd                         | 995915 ns  | 994741 ns  | 675        | 125.647M/s      |
| BM_isUniqueSimd_vectorAligned           | 1036866 ns | 1035530 ns | 658        | 120.698M/s      |
| BM_isUniqueSimd_overAligned             | 1002603 ns | 1001126 ns | 706        | 124.846M/s      |
| BM_isUniqueSimd_singleSimd              | 811088 ns  | 810025 ns  | 875        | 154.3M/s        |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 1291444 ns | 1289993 ns | 530        | 96.8894M/s      |
| BM_isUniqueSimd_avx256_intrinsics       | 711045 ns  | 710259 ns  | 944        | 175.973M/s      |
| BM_isUniqueSimd_sse_intrinsics          | 841723 ns  | 840965 ns  | 781        | 148.623M/s      |
| BM_isUniqueSimd_simde_avx2              | 703181 ns  | 702370 ns  | 909        | 177.95M/s       |

## Apple MacBook air 2022 M2

| Benchmark                               | Time      | CPU       | Iterations | UserCounters... |
|-----------------------------------------|-----------|-----------|------------|-----------------|
| BM_isUniqueSimple                       | 735419 ns | 735426 ns | 721        | 169.951M/s      |
| BM_isUniqueSimd                         | 531041 ns | 531022 ns | 1313       | 235.37M/s       |
| BM_isUniqueSimd_vectorAligned           | 533283 ns | 533273 ns | 1308       | 234.377M/s      |
| BM_isUniqueSimd_overAligned             | 531557 ns | 531552 ns | 1316       | 235.135M/s      |
| BM_isUniqueSimd_singleSimd              | 537060 ns | 537050 ns | 1309       | 232.728M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 745713 ns | 745706 ns | 938        | 167.609M/s      |
| BM_isUniqueSimd_neon_intrinsics         | 213495 ns | 213489 ns | 3269       | 585.448M/s      |
| BM_isUniqueSimd_simde_avx2              | 215076 ns | 215071 ns | 3252       | 581.141M/s      |


## Apple MacBook Pro 14" M1 Pro 8C CPU, 14C GPU (128-bit NEON)

| Benchmark                               | Time      | CPU       | Iterations | UserCounters... |
|-----------------------------------------|-----------|-----------|------------|-----------------|
| BM_isUniqueSimple                       | 778140 ns | 778048 ns | 897        | 160.641M/s      |
| BM_isUniqueSimd                         | 484649 ns | 484617 ns | 1446       | 257.908M/s      |
| BM_isUniqueSimd_vectorAligned           | 484755 ns | 484733 ns | 1455       | 257.846M/s      |
| BM_isUniqueSimd_overAligned             | 478927 ns | 478907 ns | 1453       | 260.983M/s      |
| BM_isUniqueSimd_singleSimd              | 575833 ns | 575791 ns | 1216       | 217.069M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 793748 ns | 793662 ns | 878        | 157.481M/s      |
| BM_isUniqueSimd_neon_intrinsics         | 263453 ns | 263444 ns | 2657       | 474.433M/s      |
| BM_isUniqueSimd_simde_avx2              | 262107 ns | 262071 ns | 2667       | 476.919M/s      |

## 5950x

#### GCC + stdlibc++

| Benchmark                               | Time       | CPU        | Iterations | UserCounters... |
|-----------------------------------------|------------|------------|------------|-----------------|
| BM_isUniqueSimple                       | 1458237 ns | 1458175 ns | 482        | 85.7144M/s      |
| BM_isUniqueSimd                         | 1102816 ns | 1102793 ns | 652        | 113.336M/s      |
| BM_isUniqueSimd_vectorAligned           | 1107866 ns | 1107850 ns | 630        | 112.819M/s      |
| BM_isUniqueSimd_overAligned             | 1080201 ns | 1080188 ns | 648        | 115.708M/s      |
| BM_isUniqueSimd_singleSimd              | 673325 ns  | 673320 ns  | 1026       | 185.627M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 1366174 ns | 1366153 ns | 515        | 91.488M/s       |
| BM_isUniqueSimd_avx256_intrinsics       | 761587 ns  | 761574 ns  | 916        | 164.116M/s      |
| BM_isUniqueSimd_sse_intrinsics          | 845782 ns  | 845762 ns  | 827        | 147.78M/s       |
| BM_isUniqueSimd_simde_avx2              | 763712 ns  | 763692 ns  | 915        | 163.661M/s      |

#### clang + libc++

| Benchmark                               | Time       | CPU        | Iterations | UserCounters... |
|-----------------------------------------|------------|------------|------------|-----------------|
| BM_isUniqueSimple                       | 494705 ns  | 494701 ns  | 1347       | 252.651M/s      |
| BM_isUniqueSimd                         | 988379 ns  | 988366 ns  | 708        | 126.458M/s      |
| BM_isUniqueSimd_vectorAligned           | 1008420 ns | 1008401 ns | 708        | 123.945M/s      |
| BM_isUniqueSimd_overAligned             | 1034570 ns | 1034558 ns | 676        | 120.812M/s      |
| BM_isUniqueSimd_singleSimd              | 512789 ns  | 512781 ns  | 1365       | 243.743M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 822544 ns  | 822531 ns  | 849        | 151.954M/s      |
| BM_isUniqueSimd_avx256_intrinsics       | 945362 ns  | 945347 ns  | 740        | 132.212M/s      |
| BM_isUniqueSimd_sse_intrinsics          | 978315 ns  | 978286 ns  | 714        | 127.761M/s      |
| BM_isUniqueSimd_simde_avx2              | 945276 ns  | 945253 ns  | 740        | 132.226M/s      |