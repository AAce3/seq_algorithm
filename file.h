#ifndef SEQ_ALGORITHM__FILE_H_
#define SEQ_ALGORITHM__FILE_H_
#include <stdio.h>
#include <stdbool.h>
#include "algorithm.h"

typedef enum {
    Fasta,
    Fastq
} FileType;

void process(const char *input_path,
             FILE *output,
             AlignType type,
             FileType input_file_type,
             int thread_count,
             bool is_gzipped);

void identify(const char *input_path,
              FILE *output,
              FileType input_file_type,
              int thread_count,
              bool is_gzipped);
#endif //SEQ_ALGORITHM__FILE_H_
