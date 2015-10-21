#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <stdatomic.h>

static double os_get_time(void) {
    struct timespec tms;
    clock_gettime(CLOCK_MONOTONIC, &tms);
    double seconds = (double)tms.tv_sec;
    seconds += ((double)tms.tv_nsec) / 1000000000.0;
    return seconds;
}

#define size 100000000
static int a[size];
static int b[size];
static int c[size];

typedef int v4si __attribute__ ((vector_size (16)));

int main(int argc, char *argv[]) {
    int seed = (argc >= 2) ? atoi(argv[1]) : 0;
    // prefill
    srand(seed);
    for (int i = 0; i < size; i += 1) {
        a[i] = (rand() / (float)RAND_MAX) * 1000;
        b[i] = (rand() / (float)RAND_MAX) * 1000;
    }

    double start = os_get_time();

    atomic_thread_fence(memory_order_relaxed);

#ifdef USE_SIMD
    for (int i = 0; i < size; i += 4) {
        v4si *a_ptr = (v4si *) &a[i];
        v4si *b_ptr = (v4si *) &b[i];
        v4si *c_ptr = (v4si *) &c[i];
        *c_ptr = *a_ptr + *b_ptr;
    }

#else
    for (int i = 0; i < size; i += 1) {
        c[i] = a[i] + b[i];
    }
#endif

    atomic_thread_fence(memory_order_relaxed);

    double end = os_get_time();

    double sum = 0.0;
    for (int i = 0; i < size; i += 1) {
        sum += c[i];
    }

    double elapsed = end - start;

    fprintf(stdout, "sum: %f  time: %f\n", sum, elapsed);

    return 0;
}
