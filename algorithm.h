#ifndef ALGORITHM_H
#define ALGORITHM_H
#include <stdint.h>
#ifdef LARGE // use if width of integers being used is too large
typedef unsigned long long size_s;
#else
typedef unsigned int size_s;
#endif
#ifdef __APPLE__
#include <OpenCL/cl.h>
#else
#include <CL/cl.h>
#endif

typedef enum {
    Circular,
    HOR,
} AlignType;

void align(const char *sequence,
           size_s seq_len,
           size_s min_diagonal,
           size_s max_diagonal,
           size_s max_distance,
           size_s *best_alignment,
           float *best_identity);


#endif //ALGORITHM_H