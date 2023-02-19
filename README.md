# `std::experimental::simd` experiment

Task: for string S find first position, so next 14 symbols from this position in string S are **unique**.

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

## Apple MacBook Pro 14" M1 Pro 8C CPU, 14C GPU (128-bit NEON)

| Benchmark                               | Time   | CPU    | Iterations | UserCounters... |
|-----------------------------------------|--------|--------|------------|-----------------|
| BM_isUniqueSimple                       | 749 ns | 749 ns | 928271     | 132.451M/s      |
| BM_isUniqueSimd                         | 416 ns | 416 ns | 1684162    | 238.67M/s       |
| BM_isUniqueSimd_vectorAligned           | 416 ns | 416 ns | 1684527    | 238.672M/s      |
| BM_isUniqueSimd_overAligned             | 404 ns | 404 ns | 1732099    | 245.449M/s      |
| BM_isUniqueSimd_singleSimd              | 563 ns | 563 ns | 1251363    | 176.258M/s      |
| BM_isUniqueSimd_singleSimd_loopUnrolled | 500 ns | 500 ns | 1400451    | 198.561M/s      |
| BM_isUniqueSimd_neon_intrinsics         | 226 ns | 226 ns | 3108058    | 439.404M/s      |
| BM_isUniqueSimd_simde_avx2              | 218 ns | 218 ns | 3216040    | 455.573M/s      |

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
