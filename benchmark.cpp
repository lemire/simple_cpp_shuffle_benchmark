#include <algorithm>
#include <assert.h>
#include <chrono>
#include <math.h>
#include <random>
#include <stdint.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
typedef std::chrono::high_resolution_clock Clock;

// The code relies on quaint C code. It is not exactly
// a textbook exercise in how to write nice C++, but
// we do not care since the objective is one quick benchmark.

#define AVG_TIME_NS(test, pre, repeat, size, verbose)                          \
  do {                                                                         \
    if (verbose)                                                               \
      printf("%-60s: ", #test);                                                \
    assert(repeat >= 5);                                                       \
    fflush(NULL);                                                              \
    int64_t min_diff = INT64_MAX;                                              \
    int64_t sum_diff = 0;                                                      \
    for (int i = 0; i < repeat; i++) {                                         \
      pre;                                                                     \
      __asm volatile("" ::: /* pretend to clobber */ "memory");                \
      auto t0 = Clock::now();                                                  \
      test;                                                                    \
      auto t1 = Clock::now();                                                  \
      auto cycles_diff =                                                       \
          std::chrono::duration_cast<std::chrono::nanoseconds>(t1 - t0)        \
              .count();                                                        \
      sum_diff += cycles_diff;                                                 \
      if (cycles_diff < min_diff)                                              \
        min_diff = cycles_diff;                                                \
    }                                                                          \
    uint64_t S = size;                                                         \
    if (verbose)                                                               \
      printf(" %d ns total, ", (int)min_diff);                                 \
    float cycle_per_op = (min_diff) / (double)S;                               \
    float avg_cycle_per_op = (sum_diff) / ((double)S * repeat);                \
    if (verbose)                                                               \
      printf(" %.2f ns per input key ", cycle_per_op);                         \
    if (verbose)                                                               \
      printf("\n");                                                            \
    bool err = abs(avg_cycle_per_op - cycle_per_op) > 0.01 * avg_cycle_per_op; \
    double percent =                                                           \
        abs(avg_cycle_per_op - cycle_per_op) / avg_cycle_per_op * 100.0;       \
    if (err) {                                                                 \
      fprintf(stderr, "warning: average %f vs min %f (%f percent) for arrays " \
                      "of size %zu \n",                                        \
              avg_cycle_per_op, cycle_per_op, percent, size);                  \
    }                                                                          \
    if (!verbose)                                                              \
      printf("  %.2f ", avg_cycle_per_op);                                     \
    fflush(NULL);                                                              \
  } while(false);

int qsort_compare_uint32_t(const void *a, const void *b) {
  return (*(uint32_t *)a - *(uint32_t *)b);
}

uint32_t *create_sorted_array(size_t length) {
  uint32_t *array = (uint32_t *)malloc(length * sizeof(uint32_t));
  for (size_t i = 0; i < length; i++)
    array[i] = (uint32_t)rand();
  qsort(array, length, sizeof(*array), qsort_compare_uint32_t);
  return array;
}

uint32_t *create_random_array(size_t count) {
  uint32_t *targets = (uint32_t *)malloc(count * sizeof(uint32_t));
  for (size_t i = 0; i < count; i++)
    targets[i] = (uint32_t)(rand() & 0x7FFFFFF);
  return targets;
}

int sortAndCompare(uint32_t *shuf, uint32_t *orig, uint32_t size) {
  qsort(shuf, size, sizeof(uint32_t), qsort_compare_uint32_t);
  qsort(orig, size, sizeof(uint32_t), qsort_compare_uint32_t);
  for (uint32_t k = 0; k < size; ++k)
    if (shuf[k] != orig[k]) {
      printf("[bug]\n");
      return -1;
    }
  return 0;
}

void ShuffleBenchmark32(size_t size, bool verbose) {
  if (verbose) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Uses a Mersenne Twister pseudo-random generator of 32-bit numbers\n");
    printf("Time reported in number of ns per array element.\n");
  } else {
    printf("%zu ", size);
  }
  int repeat = 10;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 5;
  printf("repeat = %d \n", repeat);
  bool sortandcompare = size < 1000000;
  uint32_t *testvalues = create_random_array(size);

  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);

  std::random_device rd;
  // A Mersenne Twister pseudo-random generator of 32-bit numbers with a state size of 19937 bits.
  std::mt19937 g(rd());

  AVG_TIME_NS(std::shuffle(testvalues, testvalues + size, g), , repeat, size,
              verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

void ShuffleBenchmark64(size_t size, bool verbose) {
  if (verbose) {
    printf(" %s\n", __PRETTY_FUNCTION__);
    printf("Shuffling arrays of size %zu \n", size);
    printf("Uses a Mersenne Twister pseudo-random generator of 64-bit numbers\n");
    printf("Time reported in number of ns per array element.\n");
  } else {
    printf("%zu ", size);
  }
  int repeat = 10;
  if (size < 1000000)
    repeat = 50;
  if (size > 10000000)
    repeat = 5;
  printf("repeat = %d \n", repeat);
  bool sortandcompare = size < 1000000;
  uint32_t *testvalues = create_random_array(size);
  uint32_t *pristinecopy = (uint32_t *)malloc(size * sizeof(uint32_t));
  memcpy(pristinecopy, testvalues, sizeof(uint32_t) * size);
  std::random_device rd;
  // A Mersenne Twister pseudo-random generator of 64-bit numbers with a state size of 19937 bits.
  std::mt19937_64 g(rd());
  AVG_TIME_NS(std::shuffle(testvalues, testvalues + size, g), , repeat, size,
              verbose);
  if (sortandcompare && (sortAndCompare(testvalues, pristinecopy, size) != 0))
    return;
  free(testvalues);
  free(pristinecopy);
  printf("\n");
}

int main() {
  size_t N = 1000 * 1000;
  printf("We benchmark arrays of 32-bit elements.\n\n");
  ShuffleBenchmark32(N, true);
  ShuffleBenchmark64(N, true);
}
