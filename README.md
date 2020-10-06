# simple_cpp_shuffle_benchmark
Simple benchmark to see how fast the standard C++ library can shuffle arrays.

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
