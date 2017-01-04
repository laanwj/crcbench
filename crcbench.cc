/**
 * ARM:
 * g++ crcbench.cc crc32c.cc perf.cpp -O2 -o crcbench -std=c++11 -march=armv8-a+crc -DBENCH_ARM
 * X86_64:
 * g++ crcbench.cc crc32c.cc perf.cpp -O2 -o crcbench -std=c++11 -msse4.2 -DBENCH_SSE42
 **/
#include <stdint.h>
#include <time.h>
#include <stdio.h>
#include <inttypes.h>
#include "crc32c.h"
#include "perf.h"
#ifdef BENCH_ARM
#include "crc32c_arm.h"
#endif
#ifdef BENCH_SSE42
#include "crc32c_sse42.h"
#endif

static const size_t testdata_size = 65536 + 4 + 2 + 1;
static const size_t iterations = 1000;
uint8_t testdata[testdata_size];

double timediff(struct timespec *b, struct timespec *e)
{
    return (e->tv_sec - b->tv_sec)*1.0 + (e->tv_nsec - b->tv_nsec)*1e-9;
}

void report(
    const char *description,
    struct timespec *tvm_b, struct timespec *tvm_e,
    struct timespec *tvc_b, struct timespec *tvc_e,
    uint64_t cycles_b, uint64_t cycles_e,
    size_t blocksize,
    size_t iterations)
{
    double mtime = timediff(tvm_b, tvm_e);
    double ctime = timediff(tvc_b, tvc_e);
    uint64_t cycles = cycles_e - cycles_b;
    uint64_t bytes = (uint64_t)blocksize*iterations;
    printf("[%s] %" PRId64 " bytes in %.fus monotonic %.fus CPU (%.1fMB/s) %.1f cycles/b\n", description,
            bytes, mtime*1e6, ctime*1e6, (bytes/1e6)/ctime,
            (double)cycles/bytes);
}

int main()
{
    struct timespec tvm_b, tvm_e;
    struct timespec tvc_b, tvc_e;
    uint64_t cycles_b, cycles_e;
    uint32_t state;
    perf_init();
    for (size_t i=0; i<testdata_size; ++i)
        testdata[i] = i;

    printf("CRC32C benchmarks\n");

    /** Optimized CPU algorithm */
    clock_gettime(CLOCK_MONOTONIC, &tvm_b);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_b);
    cycles_b = perf_cpucycles();
    state = 0;
    for (int i=0; i<iterations; ++i)
        state = crc32c::Extend(state, (const char*)testdata, testdata_size);
    cycles_e = perf_cpucycles();
    clock_gettime(CLOCK_MONOTONIC, &tvm_e);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_e);
    report("sw", &tvm_b, &tvm_e, &tvc_b, &tvc_e, cycles_b, cycles_e, testdata_size, iterations);
    uint32_t state_cpu = state;

#ifdef BENCH_ARM
    clock_gettime(CLOCK_MONOTONIC, &tvm_b);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_b);
    cycles_b = perf_cpucycles();
    state = 0;
    for (int i=0; i<iterations; ++i)
        state = crc32c_arm::Extend(state, (const char*)testdata, testdata_size);
    cycles_e = perf_cpucycles();
    clock_gettime(CLOCK_MONOTONIC, &tvm_e);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_e);
    report("hw-arm", &tvm_b, &tvm_e, &tvc_b, &tvc_e, cycles_b, cycles_e, testdata_size, iterations);

    if (state != state_cpu) {
        printf("Error: output was %08x instead of reference %08x\n", state, state_cpu);
    }
#endif

#ifdef BENCH_SSE42
    clock_gettime(CLOCK_MONOTONIC, &tvm_b);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_b);
    cycles_b = perf_cpucycles();
    state = 0;
    for (int i=0; i<iterations; ++i)
        state = crc32c_sse42::Extend(state, (const char*)testdata, testdata_size);
    cycles_e = perf_cpucycles();
    clock_gettime(CLOCK_MONOTONIC, &tvm_e);
    clock_gettime(CLOCK_THREAD_CPUTIME_ID, &tvc_e);
    report("hw-sse42", &tvm_b, &tvm_e, &tvc_b, &tvc_e, cycles_b, cycles_e, testdata_size, iterations);

    if (state != state_cpu) {
        printf("Error: output was %08x instead of reference %08x\n", state, state_cpu);
    }
#endif

    perf_fini();
}

