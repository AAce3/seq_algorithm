#include "algorithm.h"
#include <stdlib.h>

#define triple_cmp(a, b, c) ((a >= b && a >= c) * -1 + (c >= b))
#define min(a, b) (a > b ? b : a)
static inline int packed_count(char *first, char *second, size_s len1, size_s len2);

size_s wavefront_advance(size_s x, size_s y, char *sequence, size_s seq_len);

void align(const char *sequence,
           size_s seq_len,
           size_s min_diagonal,
           size_s max_diagonal,
           size_s max_distance,
           size_s *best_alignment,
           float *best_identity)
{
    size_s relevant_length = max_diagonal - min_diagonal;
    size_s wf_width = relevant_length + 2;
    // Wavefronts store height
    size_s *curr_wf = calloc(wf_width, sizeof(*curr_wf));
    size_s *next_wf = calloc(wf_width, sizeof(*next_wf));
    size_s *matches = calloc(wf_width, sizeof(*matches));
    size_s *next_matches = calloc(wf_width, sizeof(*matches));
    size_s m = seq_len - 1;
    for (size_s distance = 0; distance < max_distance; distance++) {
        // swap pointers to current and next wavefront
        size_s *temp = curr_wf;
        curr_wf = next_wf;
        next_wf = temp;
        temp = matches;
        matches = next_matches;
        next_matches = temp;

        for (size_s diagonal = 1; diagonal <= relevant_length; diagonal++) {
            size_s y = curr_wf[diagonal];
            size_s x = y + diagonal + min_diagonal;
            size_s advance = wavefront_advance(x, y, sequence, seq_len);
            matches[diagonal] += advance;
            curr_wf[diagonal] += advance;
            if (x + advance >= m) {
                float identity = (float) matches[diagonal] / ((float) (matches[diagonal] + distance));
                if (identity > *best_identity) {
                    *best_identity = identity;
                    *best_alignment = diagonal + min_diagonal;
                }
            }
        }
        for (size_s diagonal = 1; diagonal <= relevant_length; diagonal++) {
            size_s values[3];

            values[0] = curr_wf[diagonal - 1]; // going from the left (gap)
            values[1] = curr_wf[diagonal] + 1; // going diagonally forward (mismatch)
            values[2] = curr_wf[diagonal + 1] + 1; // going from the top (gap)

            int best = triple_cmp(values[0], values[1], values[2]); // produces -1 if the first is the largest, 0 if
            // the second, 1 if the third
            size_s best_idx = diagonal + best;
            next_wf[diagonal] = values[best + 1];
            next_matches[diagonal] = matches[best_idx];
        }
    }
}




size_s wavefront_advance(size_s x, size_s y, char *sequence, size_s seq_len)
{
    size_s y_orig = y;
    size_s count = 8;
    while (seq_len > x && count >= 8) {
        size_s len1 = min(seq_len - x, 8);
        size_s len2 = min(seq_len - y, 8);
        count = packed_count(&sequence[x], &sequence[y], len1, len2);
        x += count;
        y += count;
    }
    return y - y_orig;
}
static inline int packed_count(char *first, char *second, size_s len1, size_s len2)
{
    uint64_t buffer1 = 0;
    uint64_t buffer2 = 0;
    // Copies portions of the strings to 64-bit integers
    // Then, XOR them together to
    memcpy(&buffer1, first, len1);
    memcpy(&buffer2, second, len2);
    buffer1 |= ~((1ull << (8 * len1)) - 1); // Fill the top bytes of one of the integers
    uint64_t diff = buffer1 ^ buffer2;
    return __builtin_ctzll(diff) / 8;
}