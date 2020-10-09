# simple_cpp_shuffle_benchmark
Simple benchmark to see how fast the standard C++ library can shuffle arrays.

[The algorithm benchmarked](https://arxiv.org/abs/1805.10941) was adopted to accelerate random number generation
  * by the [standard C++ Linux library (GNU libstdc++)](https://gcc.gnu.org/git/?p=gcc.git;a=blobdiff;f=libstdc%2B%2B-v3/include/bits/uniform_int_dist.h;h=ecb8574864aee10b9ea164379fffef27c7bdb0df;hp=6e1e3d5fc5fe8f7f22e62a85b35dc8bfa4743372;hb=98c37d3bacbb2f8bbbe56ed53a9547d3be01b66b;hpb=6ce2cb116af6e0965ff0dd69e7fd1925cf5dc68c) to accelerate the std::uniform_int_distribution function,
 * by [Google's Abseil C++ Common Libraries](https://github.com/abseil/abseil-cpp),
 * by [the Swift standard library](https://github.com/apple/swift/pull/25286),
 * by the [Go language](https://github.com/golang/go/commit/a2dfe5d278eae0864397a046a8206342a426d2bd),
 * by the  [Julia language](https://github.com/JuliaLang/julia/pull/29240),
 * by the [Zig language](https://github.com/ziglang/zig/blob/98183e47436699f6e5eab200061c46eec342806e/std/rand.zig#L74-L118),
 * and by [Numpy](https://github.com/numpy/numpy/blob/6420e7f528a6c42422966544e453bdb2805ff620/numpy/random/generator.pyx) (Python).

## Requirements

Linux-like system with Make and GNU GCC.


## Usage

```
$ make
g++ -O3 -o benchmark benchmark.cpp -std=c++11 -Wall
$ ./benchmark
We benchmark arrays of 32-bit elements.

 void ShuffleBenchmark32(size_t, bool)
Shuffling arrays of size 1000000
Uses a Mersenne Twister pseudo-random generator of 32-bit numbers
Time reported in number of ns per array element.
repeat = 10
std::shuffle(testvalues, testvalues + size, g)              :  27992800 ns total,  27.99 ns per input key
warning: average 29.627110 vs min 27.992800 (5.516265 percent) for arrays of size 1000000

 void ShuffleBenchmark64(size_t, bool)
Shuffling arrays of size 1000000
Uses a Mersenne Twister pseudo-random generator of 64-bit numbers
Time reported in number of ns per array element.
repeat = 10
std::shuffle(testvalues, testvalues + size, g)              :  15834900 ns total,  15.83 ns per input key
warning: average 17.528509 vs min 15.834900 (9.662027 percent) for arrays of size 1000000
```

## Other code

For a large testing code base, see https://github.com/lemire/FastShuffleExperiments

## Reference

Daniel Lemire,  [Fast Random Integer Generation in an Interval](https://arxiv.org/abs/1805.10941), ACM Transactions on Modeling and Computer Simulation 29 (1), 2019
